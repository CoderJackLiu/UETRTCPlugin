#include "TRTCMediaTracks.h"

#include "IMediaBinarySample.h"
#include "IMediaOverlaySample.h"
#include "MediaHelpers.h"
#include "TRTCLogMacro.h"
#include "TRTC/TRTCTypeDef.h"
#include "MediaSamples.h"
#include "TRTCMediaAudioSample.h"
#include "TRTCMediaTextureSample.h"

//todo liuqi 完善所有实现

FTRTCMediaTracks::FTRTCMediaTracks()
	: Player(nullptr)
	  , Samples(new FMediaSamples)
	  , AudioChannels(0)
	  , AudioSampleFormat(EMediaAudioSampleFormat::Int16)
	  , AudioSamplePool(new FTRTCMediaAudioSamplePool)
	  , AudioSampleRate(0)
	  , AudioSampleSize(0)
	  , CurrentTime(FTimespan::Zero())
	  , VideoBufferDim(FIntPoint::ZeroValue)
	  , VideoBufferStride(0)
	  , VideoOutputDim(FIntPoint::ZeroValue)
	  , VideoSampleFormat(EMediaTextureSampleFormat::CharAYUV)
	  , VideoSamplePool(new FTRTCMediaTextureSamplePool)
	  , CurrentState()
	  , CurrentRate(30.f)
	  , ShouldLoop(false)
{
}

void FTRTCMediaTracks::Initialize(liteav::ue::TRTCCloud& InPlayer, FString& OutInfo)
{
	Player = &InPlayer;
	//todo liuqi OutInfo
}

// void FTRTCMediaTracks::TickInput(FTimespan DeltaTime, FTimespan Timecode)
// {
// 	TargetTime = Timecode;
//
// 	double time = Timecode.GetTotalSeconds();
// 	UE_LOG(LogTRTCMedia, Verbose, TEXT("Tracks: %p: TimeCode %.3f"), this, static_cast<float>(time));
// }

void FTRTCMediaTracks::Shutdown()
{
	if (Player == nullptr)
	{
		return;
	}
	Player = nullptr;

	// unregister callbacks

	AudioSamplePool->Reset();
	VideoSamplePool->Reset();

	CurrentTime = FTimespan::Zero();
	Player = nullptr;
}

bool FTRTCMediaTracks::GetAudioTrackFormat(int32 TrackIndex, int32 FormatIndex, FMediaAudioTrackFormat& OutFormat) const
{
	if (!AudioTracks.IsValidIndex(TrackIndex) || (FormatIndex != 0))
	{
		return false;
	}

	// @todo liuqi: fix audio format
	OutFormat.BitsPerSample = 0;
	OutFormat.NumChannels = 2;
	OutFormat.SampleRate = 44100;
	OutFormat.TypeName = TEXT("PCM");
	return true;
}

int32 FTRTCMediaTracks::GetNumTracks(EMediaTrackType TrackType) const
{
	switch (TrackType)
	{
	case EMediaTrackType::Audio:
		return AudioTracks.Num();

	case EMediaTrackType::Caption:
		return CaptionTracks.Num();

	case EMediaTrackType::Video:
		return VideoTracks.Num();

	default:
		return 0;
	}
}

int32 FTRTCMediaTracks::GetNumTrackFormats(EMediaTrackType TrackType, int32 TrackIndex) const
{
	return ((TrackIndex >= 0) && (TrackIndex < GetNumTracks(TrackType))) ? 1 : 0;
}

int32 FTRTCMediaTracks::GetSelectedTrack(EMediaTrackType TrackType) const
{
	return INDEX_NONE;
}

FText FTRTCMediaTracks::GetTrackDisplayName(EMediaTrackType TrackType, int32 TrackIndex) const
{
	switch (TrackType)
	{
	case EMediaTrackType::Audio:
		if (AudioTracks.IsValidIndex(TrackIndex))
		{
			return AudioTracks[TrackIndex].DisplayName;
		}
		break;

	case EMediaTrackType::Caption:
		if (CaptionTracks.IsValidIndex(TrackIndex))
		{
			return CaptionTracks[TrackIndex].DisplayName;
		}
		break;

	case EMediaTrackType::Video:
		if (VideoTracks.IsValidIndex(TrackIndex))
		{
			return VideoTracks[TrackIndex].DisplayName;
		}

	default:
		break;
	}

	return FText::GetEmpty();
}

int32 FTRTCMediaTracks::GetTrackFormat(EMediaTrackType TrackType, int32 TrackIndex) const
{
	return (GetSelectedTrack(TrackType) != INDEX_NONE) ? 0 : INDEX_NONE;
}

FString FTRTCMediaTracks::GetTrackLanguage(EMediaTrackType TrackType, int32 TrackIndex) const
{
	return TEXT("und"); // undefined
}

FString FTRTCMediaTracks::GetTrackName(EMediaTrackType TrackType, int32 TrackIndex) const
{
	switch (TrackType)
	{
	case EMediaTrackType::Audio:
		if (AudioTracks.IsValidIndex(TrackIndex))
		{
			return AudioTracks[TrackIndex].Name;
		}
		break;

	case EMediaTrackType::Caption:
		if (CaptionTracks.IsValidIndex(TrackIndex))
		{
			return CaptionTracks[TrackIndex].Name;
		}
		break;

	case EMediaTrackType::Video:
		if (VideoTracks.IsValidIndex(TrackIndex))
		{
			return VideoTracks[TrackIndex].Name;
		}

	default:
		break;
	}

	return FString();
}

bool FTRTCMediaTracks::GetVideoTrackFormat(int32 TrackIndex, int32 FormatIndex, FMediaVideoTrackFormat& OutFormat) const
{
	if (!VideoTracks.IsValidIndex(TrackIndex) || (FormatIndex != 0) /*|| (Player == nullptr)*/)
	{
		return false;
	}

	// @todo liuqi: fix video specs
	// OutFormat.Dim = FIntPoint(FVlc::VideoGetWidth(Player), FVlc::VideoGetHeight(Player));
	// OutFormat.FrameRate = FVlc::MediaPlayerGetFps(Player);
	// OutFormat.FrameRates = TRange<float>(OutFormat.FrameRate);
	// OutFormat.TypeName = TEXT("Default");

	return true;
}

bool FTRTCMediaTracks::SelectTrack(EMediaTrackType TrackType, int32 TrackIndex)
{
	return false;
}

bool FTRTCMediaTracks::SetTrackFormat(EMediaTrackType TrackType, int32 TrackIndex, int32 FormatIndex)
{
	UE_LOG(LogTRTCMedia, Verbose, TEXT("Tracks %p: Setting format on %s track %i to %i"), this, *MediaUtils::TrackTypeToString(TrackType), TrackIndex, FormatIndex);

	FScopeLock Lock(&CriticalSection);

	TArray<FTrack>* Tracks = nullptr;

	switch (TrackType)
	{
	case EMediaTrackType::Audio:
		Tracks = &AudioTracks;
		break;

	case EMediaTrackType::Caption:
		Tracks = &CaptionTracks;
		break;

	case EMediaTrackType::Metadata:
		Tracks = &MetadataTracks;
		break;

	case EMediaTrackType::Video:
		Tracks = &VideoTracks;
		break;

	default:
		return false; // unsupported track type
	};

	check(Tracks != nullptr);

	if (!Tracks->IsValidIndex(TrackIndex))
	{
		return false; // invalid track index
	}


	return true;
}


void FTRTCMediaTracks::onExitRoom(int reason)
{
	writeCallbackLog("onExitRoom");
	UE_LOG(LogTRTCMedia, Log, TEXT("<== %d"), reason);
	CurrentState = EMediaState::Closed;
}

void FTRTCMediaTracks::onEnterRoom(int result)
{
	writeCallbackLog("onEnteredRoom");
	UE_LOG(LogTRTCMedia, Log, TEXT("<== %d"), result);
	CurrentState = EMediaState::Preparing;
}

void FTRTCMediaTracks::onUserVideoAvailable(const char* userId, bool available)
{
	writeCallbackLog("onUserVideoAvailable");
	writeCallbackLog(userId);
	if (available)
	{
		Player->startRemoteView(userId, TRTCVideoStreamTypeBig, nullptr);

		Player->setRemoteVideoRenderCallback(userId, TRTCVideoPixelFormat_BGRA32, TRTCVideoBufferType_Buffer, this);
		CurrentState = EMediaState::Preparing;
	}
	else
	{
		Player->stopRemoteView(userId, TRTCVideoStreamTypeBig);
		CurrentState = EMediaState::Stopped;

		//todo liuqi 暂停  析构
		// ResetBuffer(false);
		// AsyncTask(ENamedThreads::GameThread, [=]()
		// {
		// 	remoteRenderTargetTexture->UpdateTextureRegions(0, 1, remoteUpdateTextureRegion, remoteWidth * 4, (uint32)4,remoteBuffer);
		// });
	}
}

void FTRTCMediaTracks::onUserSubStreamAvailable(const char* userId, bool available)
{
	writeCallbackLog("onUserSubStreamAvailable");
	writeCallbackLog(userId);
	if (available)
	{
		Player->startRemoteView(userId, TRTCVideoStreamTypeSub, nullptr);
		Player->setRemoteVideoRenderCallback(userId, TRTCVideoPixelFormat_BGRA32, TRTCVideoBufferType_Buffer, this);
		CurrentState = EMediaState::Preparing;
	}
	else
	{
		Player->stopRemoteView(userId, TRTCVideoStreamTypeSub);
		CurrentState = EMediaState::Stopped;
		//todo liuqi 暂停  析构
		//		remoteRenderTargetTexture->UpdateTextureRegions(0, 1, remoteUpdateTextureRegion, remoteWidth * 4, (uint32)4, remoteBuffer);
	}
}

void FTRTCMediaTracks::onError(TXLiteAVError errCode, const char* errMsg, void* extraInfo)
{
	writeCallbackLog(errMsg);
}

void FTRTCMediaTracks::onWarning(TXLiteAVWarning warningCode, const char* warningMsg, void* extraInfo)
{
	writeCallbackLog(warningMsg);
}

void FTRTCMediaTracks::onFirstVideoFrame(const char* userId, const TRTCVideoStreamType streamType, const int width, const int height)
{
	//log
	UE_LOG(LogTRTCMedia, Warning, TEXT("onFirstVideoFrame userId:%s streamType:%d width:%d height:%d"), ANSI_TO_TCHAR(userId), streamType, width, height)

	auto VideoSample = VideoSamplePool->Acquire();

	VideoBufferDim = FIntPoint(width, height);
	VideoOutputDim = FIntPoint(width, height);
	VideoSampleFormat = EMediaTextureSampleFormat::CharBGR10A2;
	VideoBufferStride = width * 4;

	// if (VideoSample->Initialize(FIntPoint(width, height), FIntPoint(width, height), EMediaTextureSampleFormat::CharAYUV, width * 4, FTimespan(1)))
	// {
	// 	return;
	// }
	// VideoSample->SetTime(FTimespan(0));
	// //todo liuqi
	// CurrentState = EMediaState::Playing;
	// CurrentState = EMediaState::Playing;
}

void FTRTCMediaTracks::onFirstAudioFrame(const char* userId)
{
	//log
	UE_LOG(LogTRTCMedia, Warning, TEXT("onFirstAudioFrame userId:%s"), ANSI_TO_TCHAR(userId))
}

void FTRTCMediaTracks::onNetworkQuality(TRTCQualityInfo localQuality, TRTCQualityInfo* remoteQuality, uint32_t remoteQualityCount)
{
	//log


	UE_LOG(LogTRTCMedia, Warning, TEXT("onNetworkQuality :localQuality:%d"), localQuality.quality)
}

void FTRTCMediaTracks::onStatistics(const TRTCStatistics& statistics)
{
	//log
	UE_LOG(LogTRTCMedia, Warning, TEXT("onStatistics"))
}

void FTRTCMediaTracks::onSpeedTestResult(const TRTCSpeedTestResult& result)
{
	//break struct result and log quality
	UE_LOG(LogTRTCMedia, Warning, TEXT("TRTC onSpeedTestResult"))
	//todo liuqi log result.quality 
	UE_LOG(LogTRTCMedia, Warning, TEXT("TRTC onSpeedTestResult quality"))
}

void FTRTCMediaTracks::onConnectionLost()
{
	//log
	UE_LOG(LogTRTCMedia, Warning, TEXT("TRTC onConnectionLost"))
}

void FTRTCMediaTracks::onTryToReconnect()
{
	//log
	UE_LOG(LogTRTCMedia, Warning, TEXT("TRTC onTryToReconnect"))
}

void FTRTCMediaTracks::WriteLblLog(const char* logStr) const
{
	std::string stdStrLog(logStr);
	FString log = stdStrLog.c_str();
	UE_LOG(LogTRTCMedia, Log, TEXT("TRTC Log ==> %s"), *log);
}

void FTRTCMediaTracks::writeCallbackLog(const char* logStr) const
{
	std::string stdStrLog(logStr);
	FString log = stdStrLog.c_str();
	UE_LOG(LogTRTCMedia, Log, TEXT("TRTC Callback<== %s"), *log);
}

void FTRTCMediaTracks::onRenderVideoFrame(const char* userId, TRTCVideoStreamType streamType, TRTCVideoFrame* videoFrame)
{
	//log
	//UE_LOG(LogTRTCMedia, Warning, TEXT("onRenderVideoFrame 1 userId:%s streamType:%d"), ANSI_TO_TCHAR(userId), streamType)

	FScopeLock lock(&remoteMutex);
	{
		//log
		UE_LOG(LogTRTCMedia, Warning, TEXT("onRenderVideoFrame userId:%s streamType:%d"), ANSI_TO_TCHAR(userId), streamType)

		// CurrentTime = FTimespan(static_cast<double>(videoFrame->timestamp) / 1000.f);

		FTRTCMediaTextureSample* VideoSample = reinterpret_cast<FTRTCMediaTextureSample*>(videoFrame);

		//log videoFrame->timestamp
		UE_LOG(LogTRTCMedia, Warning, TEXT("onRenderVideoFrame videoFrame->timestamp:%llu"), videoFrame->timestamp)


		// 将毫秒转换为微秒，因为FTimeSpan以微秒为单位构造
		int64 microseconds = videoFrame->timestamp * 1000;

		// 使用微秒创建FTimeSpan对象
		CurrentTime += FTimespan::FromMicroseconds(microseconds);

		VideoBufferDim = FIntPoint(videoFrame->width, videoFrame->height);
		VideoOutputDim = FIntPoint(videoFrame->width, videoFrame->height);
		VideoSampleFormat = EMediaTextureSampleFormat::CharBGR10A2;
		VideoBufferStride = videoFrame->width * 4;
		VideoSample->Initialize(VideoBufferDim, VideoOutputDim, EMediaTextureSampleFormat::CharBGR10A2, VideoBufferStride, FTimespan(0));

		// 设置 VideoSample 的时间
		VideoSample->SetTime(CurrentTime);

		// VideoSample->SetTime(FTimespan(videoFrame->timestamp));
		//log current time
		UE_LOG(LogTRTCMedia, Warning, TEXT("onRenderVideoFrame CurrentTime:%f"), CurrentTime.GetTotalMilliseconds())


		Samples->AddVideo(VideoSamplePool->ToShared(VideoSample));

		// CurrentState = EMediaState::Playing;
		CurrentState = EMediaState::Preparing;

		// int frameLength = videoFrame->length;
		// std::string strUserId(TCHAR_TO_UTF8(*userId));
		//
		// if (frameLength > 1)
		// {
		// 	bool isLocalUser = ((strcmp(strUserId.c_str(), userId) == 0 || nullptr == userId || strlen(userId) == 0) && streamType == TRTCVideoStreamTypeBig) ? true : false;
		// 	UpdateBuffer(videoFrame->data, videoFrame->width, videoFrame->height, frameLength, isLocalUser);
		// }
	}
}

IMediaSamples& FTRTCMediaTracks::GetSamples() const
{
	return *Samples;
}

void FTRTCMediaTracks::SetCurrentTime(FTimespan Time)
{
	CurrentTime = Time;
}

bool FTRTCMediaTracks::FetchAudio(TRange<FTimespan> TimeRange, TSharedPtr<IMediaAudioSample, ESPMode::ThreadSafe>& OutSample)
{
	TSharedPtr<IMediaAudioSample, ESPMode::ThreadSafe> Sample;

	FTimespan timeSpan = TimeRange.Size<FTimespan>();
	// UE_LOG(LogTemp, Error, TEXT("FetchAudio!!!!!!!!!!!!"));
	if (!AudioSampleQueue.Peek(Sample))
	{
		// UE_LOG(LogTemp, Error, TEXT("!AudioSampleQueue.Dequeue(Sample)"));

		return false;
	}

	const FTimespan SampleTime = Sample->GetTime().Time;

	if (SampleTime + Sample->GetDuration() > CurrentTime)
	{
		// UE_LOG(LogTemp, Error, TEXT("Audio time >  %lu  Video time:%lu"), SampleTime.GetTicks(),CurrentTime.GetTicks());
		//UE_LOG(LogTemp, Error, TEXT("Audio time >  %lu  Video time:%lu"), );
		//UE_LOG(LogFFMPEGMedia, Error, TEXT("Audio time >  %lu  Video time:%lu"),this, SampleTime.GetTicks(),CurrentTime);
		return false;
	}


	// if (!TimeRange.Overlaps(TRange<FTimespan>(SampleTime, SampleTime + Sample->GetDuration())))
	// {
	//		AudioSampleQueue.RequestFlush();
	// 		return false;
	// }

	if (!AudioSampleQueue.Dequeue(Sample))
	{
		//UE_LOG(LogTemp, Error, TEXT("!AudioSampleQueue.Dequeue(Sample)"));
		return false;
	}
	//UE_LOG(LogTemp, Error, TEXT("Audio time < %lu  Video time:%lu"), SampleTime.GetTicks(),CurrentTime.GetTicks());
	OutSample = Sample;

	return true;
}

bool FTRTCMediaTracks::FetchCaption(TRange<FTimespan> TimeRange, TSharedPtr<IMediaOverlaySample, ESPMode::ThreadSafe>& OutSample)
{
	TSharedPtr<IMediaOverlaySample, ESPMode::ThreadSafe> Sample;

	if (!CaptionSampleQueue.Peek(Sample))
	{
		return false;
	}

	const FTimespan SampleTime = Sample->GetTime().Time;

	if (!TimeRange.Overlaps(TRange<FTimespan>(SampleTime, SampleTime + Sample->GetDuration())))
	{
		CaptionSampleQueue.RequestFlush();
		return false;
	}

	if (!CaptionSampleQueue.Dequeue(Sample))
	{
		return false;
	}

	OutSample = Sample;

	return true;
}

bool FTRTCMediaTracks::FetchMetadata(TRange<FTimespan> TimeRange, TSharedPtr<IMediaBinarySample, ESPMode::ThreadSafe>& OutSample)
{
	TSharedPtr<IMediaBinarySample, ESPMode::ThreadSafe> Sample;

	if (!MetadataSampleQueue.Peek(Sample))
	{
		return false;
	}

	const FTimespan SampleTime = Sample->GetTime().Time;

	if (!TimeRange.Overlaps(TRange<FTimespan>(SampleTime, SampleTime + Sample->GetDuration())))
	{
		return false;
	}

	if (!MetadataSampleQueue.Dequeue(Sample))
	{
		return false;
	}

	OutSample = Sample;

	return true;
}

bool FTRTCMediaTracks::FetchVideo(TRange<FTimespan> TimeRange, TSharedPtr<IMediaTextureSample, ESPMode::ThreadSafe>& OutSample)
{
	TSharedPtr<IMediaTextureSample, ESPMode::ThreadSafe> Sample;

	if (!VideoSampleQueue.Peek(Sample))
	{
		return false;
	}

	const FTimespan SampleTime = Sample->GetTime().Time;

	if (!TimeRange.Overlaps(TRange<FTimespan>(SampleTime, SampleTime + Sample->GetDuration())))
	{
		VideoSampleQueue.RequestFlush();
		return false;
	}

	if (!VideoSampleQueue.Dequeue(Sample))
	{
		return false;
	}

	OutSample = Sample;

	return true;
}

void FTRTCMediaTracks::FlushSamples()
{
	AudioSampleQueue.RequestFlush();
	CaptionSampleQueue.RequestFlush();
	MetadataSampleQueue.RequestFlush();
	VideoSampleQueue.RequestFlush();
}

bool FTRTCMediaTracks::PeekVideoSampleTime(FMediaTimeStamp& TimeStamp)
{
	TSharedPtr<IMediaTextureSample, ESPMode::ThreadSafe> Sample;
	if (!VideoSampleQueue.Peek(Sample))
	{
		return false;
	}
	TimeStamp = FMediaTimeStamp(Sample->GetTime());
	return true;
}

// void FTRTCMediaTracks::ResetBuffer(bool isLocal)
// {
// 	if (isLocal && localBuffer)
// 	{
// 		for (uint32 i = 0; i < localWidth * localHeight; ++i)
// 		{
// 			localBuffer[i * 4 + 0] = 0x32;
// 			localBuffer[i * 4 + 1] = 0x32;
// 			localBuffer[i * 4 + 2] = 0x32;
// 			localBuffer[i * 4 + 3] = 0xFF;
// 		}
// 		localRefresh = false;
// 	}
// 	else if (remoteBuffer)
// 	{
// 		for (uint32 i = 0; i < remoteWidth * remoteHeight; ++i)
// 		{
// 			remoteBuffer[i * 4 + 0] = 0x32;
// 			remoteBuffer[i * 4 + 1] = 0x32;
// 			remoteBuffer[i * 4 + 2] = 0x32;
// 			remoteBuffer[i * 4 + 3] = 0xFF;
// 		}
// 		remoteRefresh = false;
// 	}
// }
//
// void FTRTCMediaTracks::UpdateBuffer(char* RGBBuffer, uint32_t NewWidth, uint32_t NewHeight, uint32_t NewSize, bool isLocal)
// {
// }

bool FTRTCMediaTracks::CanControl(EMediaControl Control) const
{
	if (Player == nullptr)
	{
		return false;
	}

	//todo liuqi 看看这个能不能暂停 恢复 seek

	if (Control == EMediaControl::Pause)
	{
		return false;
	}

	if (Control == EMediaControl::Resume)
	{
		return false;
	}

	if (Control == EMediaControl::Scrub)
	{
		return false;
	}

	if (Control == EMediaControl::Seek)
	{
		return false;;
	}

	return false;
}

FTimespan FTRTCMediaTracks::GetDuration() const
{
	return FTimespan::Zero();
	return FTimespan(10000 * ETimespan::TicksPerMillisecond);
}

float FTRTCMediaTracks::GetRate() const
{
	return CurrentRate;
}

EMediaState FTRTCMediaTracks::GetState() const
{
	// return EMediaState::Closed;
	if (Player == nullptr)
	{
		return EMediaState::Closed;
	}

	return CurrentState; // should never get here
}

EMediaStatus FTRTCMediaTracks::GetStatus() const
{
	return EMediaStatus::None;
}

TRangeSet<float> FTRTCMediaTracks::GetSupportedRates(EMediaRateThinning Thinning) const
{
	TRangeSet<float> Result;

	if (Thinning == EMediaRateThinning::Thinned)
	{
		Result.Add(TRange<float>::Inclusive(0.0f, 10.0f));
	}
	else
	{
		Result.Add(TRange<float>::Inclusive(0.0f, 1.0f));
	}

	return Result;
}

FTimespan FTRTCMediaTracks::GetTime() const
{
	return FTimespan(0);
	return CurrentTime;
}

bool FTRTCMediaTracks::IsLooping() const
{
	return ShouldLoop;
}

bool FTRTCMediaTracks::Seek(const FTimespan& Time)
{
	return false;
}

bool FTRTCMediaTracks::SetLooping(bool Looping)
{
	//todo liuqi
	ShouldLoop = Looping;
	return true;
}

bool FTRTCMediaTracks::SetRate(float Rate)
{
	if (Player == nullptr)
	{
		return false;
	}
	//todo 播放 设置播放频率

	return true;
}
