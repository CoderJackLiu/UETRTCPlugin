#include "TRTCMediaTracks.h"

#include "TRTCLogMacro.h"
#include "TRTCTypeDef.h"

//todo liuqi 完善所有实现

FTRTCMediaTracks::FTRTCMediaTracks()
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
	return false;
}


void FTRTCMediaTracks::onExitRoom(int reason)
{
	writeCallbackLog("onExitRoom");
	UE_LOG(LogTRTCMedia, Log, TEXT("<== %d"), reason);
}

void FTRTCMediaTracks::onEnterRoom(int result)
{
	writeCallbackLog("onEnterRoom");
	UE_LOG(LogTRTCMedia, Log, TEXT("<== %d"), result);
}

void FTRTCMediaTracks::onUserVideoAvailable(const char* userId, bool available)
{
	writeCallbackLog("onUserVideoAvailable");
	writeCallbackLog(userId);
	if (available)
	{
		Player->startRemoteView(userId, TRTCVideoStreamTypeBig, nullptr);

		Player->setRemoteVideoRenderCallback(userId, TRTCVideoPixelFormat_BGRA32, TRTCVideoBufferType_Buffer, this);
	}
	else
	{
		Player->stopRemoteView(userId, TRTCVideoStreamTypeBig);

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
	}
	else
	{
		Player->stopRemoteView(userId, trtc::TRTCVideoStreamTypeSub);
		ResetBuffer(false);
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

void FTRTCMediaTracks::WriteLblLog(const char* logStr)
{
	std::string stdStrLog(logStr);
	FString log = stdStrLog.c_str();
	UE_LOG(LogTRTCMedia, Log, TEXT("TRTC Log ==> %s"), *log);
}

void FTRTCMediaTracks::writeCallbackLog(const char* logStr)
{
	std::string stdStrLog(logStr);
	FString log = stdStrLog.c_str();
	UE_LOG(LogTRTCMedia, Log, TEXT("TRTC Callback<== %s"), *log);
}

void FTRTCMediaTracks::onRenderVideoFrame(const char* userId, TRTCVideoStreamType streamType, TRTCVideoFrame* frame)
{
	//log
	UE_LOG(LogTRTCMedia, Warning, TEXT("onRenderVideoFrame userId:%s streamType:%d"), ANSI_TO_TCHAR(userId), streamType)
}

void FTRTCMediaTracks::ResetBuffer(bool isLocal)
{
	if (isLocal && localBuffer)
	{
		for (uint32 i = 0; i < localWidth * localHeight; ++i)
		{
			localBuffer[i * 4 + 0] = 0x32;
			localBuffer[i * 4 + 1] = 0x32;
			localBuffer[i * 4 + 2] = 0x32;
			localBuffer[i * 4 + 3] = 0xFF;
		}
		localRefresh = false;
	}
	else if (remoteBuffer)
	{
		for (uint32 i = 0; i < remoteWidth * remoteHeight; ++i)
		{
			remoteBuffer[i * 4 + 0] = 0x32;
			remoteBuffer[i * 4 + 1] = 0x32;
			remoteBuffer[i * 4 + 2] = 0x32;
			remoteBuffer[i * 4 + 3] = 0xFF;
		}
		remoteRefresh = false;
	}
}
