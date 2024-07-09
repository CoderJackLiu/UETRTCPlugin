#include "Player/TRTCLivePlayer.h"
#include "TRTCLogMacro.h"

UTRTCLivePlayer::UTRTCLivePlayer()
{
}

UTRTCLivePlayer::~UTRTCLivePlayer()
{
	if (live_player_ != nullptr)
	{
		releaseV2TXLivePlayer(live_player_);
		live_player_ = nullptr;
		delete[] VideoBuffer;
		delete UpdateTextureRegion;
	}
}

void UTRTCLivePlayer::Tick(float DeltaTime)
{
	RenderVideoFrameToRT();
}


ETickableTickType UTRTCLivePlayer::GetTickableTickType() const
{
	return ETickableTickType::Always;
}

bool UTRTCLivePlayer::IsTickable() const
{
	return true;
}

TStatId UTRTCLivePlayer::GetStatId() const
{
	return TStatId();
}

void UTRTCLivePlayer::InitializePlayer()
{
	live_player_ = createV2TXLivePlayer();
	live_player_->setObserver(this);
	live_player_->enableObserveVideoFrame(true, V2TXLivePixelFormatBGRA32, V2TXLiveBufferTypeByteBuffer);
}

void UTRTCLivePlayer::StartPlay(const FString& url) const
{
	//log event name
	UE_LOG(LogTRTCMedia, Log, TEXT( "UTRTCLivePlayer::StartPlay" ));
	live_player_->startPlay(TCHAR_TO_ANSI(*url));
}

void UTRTCLivePlayer::StopPlay()
{
	//log event name
	UE_LOG(LogTRTCMedia, Log, TEXT( "UTRTCLivePlayer::StopPlay" ));
	live_player_->stopPlay();
	IsBeginPlay = false;
}

void UTRTCLivePlayer::PauseVideo()
{
	//log event name
	UE_LOG(LogTRTCMedia, Log, TEXT( "UTRTCLivePlayer::PauseVideo" ));
	live_player_->pauseVideo();
	IsBeginPlay = false;
}

void UTRTCLivePlayer::ResumeVideo() const
{
	//log event name
	UE_LOG(LogTRTCMedia, Log, TEXT( "UTRTCLivePlayer::ResumeVideo" ));
	live_player_->resumeVideo();
}

bool UTRTCLivePlayer::IsPlaying() const
{
	//log event name
	UE_LOG(LogTRTCMedia, Log, TEXT( "UTRTCLivePlayer::IsPlaying" ));
	return live_player_->isPlaying() == 1;
}

void UTRTCLivePlayer::onError(V2TXLivePlayer* player, int32_t code, const char* msg, void* extraInfo)
{
	//log error
	UE_LOG(LogTRTCMedia, Error, TEXT( "UTRTCLivePlayer::onError: %s" ), ANSI_TO_TCHAR( msg ));
}

void UTRTCLivePlayer::onWarning(V2TXLivePlayer* player, int32_t code, const char* msg, void* extraInfo)
{
	//log warning
	UE_LOG(LogTRTCMedia, Warning, TEXT( "UTRTCLivePlayer::onWarning: %s" ), ANSI_TO_TCHAR( msg ));
}

void UTRTCLivePlayer::onConnected(V2TXLivePlayer* player, void* extraInfo)
{
	//log connected
	UE_LOG(LogTRTCMedia, Log, TEXT( "UTRTCLivePlayer::onConnected" ));
}

void UTRTCLivePlayer::onVideoPlaying(V2TXLivePlayer* player, bool firstPlay, void* extraInfo)
{
	//log video playing
	UE_LOG(LogTRTCMedia, Log, TEXT( "UTRTCLivePlayer::onVideoPlaying" ));
}

void UTRTCLivePlayer::onAudioPlaying(V2TXLivePlayer* player, bool firstPlay, void* extraInfo)
{
	//log audio playing
	UE_LOG(LogTRTCMedia, Log, TEXT( "UTRTCLivePlayer::onAudioPlaying" ));
}

void UTRTCLivePlayer::onVideoLoading(V2TXLivePlayer* player, void* extraInfo)
{
	//log video loading
	UE_LOG(LogTRTCMedia, Log, TEXT( "UTRTCLivePlayer::onVideoLoading" ));
}

void UTRTCLivePlayer::onAudioLoading(V2TXLivePlayer* player, void* extraInfo)
{
	//log audio loading
	UE_LOG(LogTRTCMedia, Log, TEXT( "UTRTCLivePlayer::onAudioLoading" ));
}

void UTRTCLivePlayer::onPlayoutVolumeUpdate(V2TXLivePlayer* player, int32_t volume)
{
	//log playout volume update and volume
	UE_LOG(LogTRTCMedia, Log, TEXT( "UTRTCLivePlayer::onPlayoutVolumeUpdate: %d" ), volume);
}

void UTRTCLivePlayer::onStatisticsUpdate(V2TXLivePlayer* player, V2TXLivePlayerStatistics statistics)
{
	//log one time per 100 times
	static int count = 0;
	//count grater than 1000 reset to 0
	if (count > 1000)
	{
		count = 0;
	}
	if (count++ % 30 == 0)
	{
		//log fps and bitrate
		UE_LOG(LogTRTCMedia, Log, TEXT( "UTRTCLivePlayer::onStatisticsUpdate: %d %d" ), statistics.fps, statistics.videoBitrate);
	}
}

void UTRTCLivePlayer::onSnapshotComplete(V2TXLivePlayer* player, const char* image, int length, int width, int height, V2TXLivePixelFormat format)
{
	//log event name
	UE_LOG(LogTRTCMedia, Log, TEXT( "UTRTCLivePlayer::onSnapshotComplete" ));
}

void UTRTCLivePlayer::onRenderVideoFrame(V2TXLivePlayer* player, const V2TXLiveVideoFrame* videoFrame)
{
	//log  one time per 10 times
	static int count = 0;
	//count grater than 1000 reset to 0
	if (count > 1000)
	{
		count = 0;
	}
	if (count++ % 100 == 0)
	{
		UE_LOG(LogTRTCMedia, Log, TEXT( "UTRTCLivePlayer::onRenderVideoFrame: %d %d %d" ), videoFrame->length, videoFrame->width, videoFrame->height);
	}

	int frameLength = videoFrame->length;
	if (frameLength > 1)
	{
		UpdateBuffer(videoFrame->data, videoFrame->width, videoFrame->height, frameLength);
	}
}

void UTRTCLivePlayer::onReceiveSeiMessage(V2TXLivePlayer* player, int payloadType, const uint8_t* data, uint32_t dataSize)
{
	//log event name
	UE_LOG(LogTRTCMedia, Log, TEXT( "UTRTCLivePlayer::onReceiveSeiMessage" ));
}

void UTRTCLivePlayer::onVideoResolutionChanged(V2TXLivePlayer* player, int width, int height)
{
	//log event name and resolution
	UE_LOG(LogTRTCMedia, Log, TEXT( "UTRTCLivePlayer::onVideoResolutionChanged: %d %d" ), width, height);
}

void UTRTCLivePlayer::UpdateBuffer(char* RGBBuffer, uint32_t NewWidth, uint32_t NewHeight, uint32_t NewSize)
{
	FScopeLock lock(&Mutex);
	if (!RGBBuffer)
	{
		return;
	}
	if (NewHeight != TextureHeight)
	{
		UE_LOG(LogTemp, Warning, TEXT("NewHeight != remoteHeight ,remoteHeight=%d, NewHeight=%d"), TextureHeight,
		       NewHeight);
	}
	if (!VideoRefresh)
	{
		// Fist Video Frame
		UE_LOG(LogTemp, Warning, TEXT("VideoRefresh==false; NewSize=%d ,NewWidth=%d, NewHeight=%d"), NewSize, NewWidth, NewHeight);
		AsyncTask(ENamedThreads::GameThread, [=]()
		{
			TextureWidth = NewWidth;
			TextureHeight = NewHeight;
			RenderTargetTexture = UTexture2D::CreateTransient(TextureWidth, TextureHeight);
			RenderTargetTexture->UpdateResource();
			TextureBufferSize = TextureWidth * TextureHeight * 4;
			VideoBuffer = new uint8[TextureBufferSize];
			for (uint32 i = 0; i < TextureWidth * TextureHeight; ++i)
			{
				VideoBuffer[i * 4 + 0] = 0x32;
				VideoBuffer[i * 4 + 1] = 0x32;
				VideoBuffer[i * 4 + 2] = 0x32;
				VideoBuffer[i * 4 + 3] = 0xFF;
			}
			UpdateTextureRegion = new FUpdateTextureRegion2D(0, 0, 0, 0, TextureWidth, TextureHeight);
			RenderTargetTexture->UpdateTextureRegions(0, 1, UpdateTextureRegion, TextureWidth * 4, (uint32)4, VideoBuffer);
			OnRenderTargetAvailable.Broadcast(RenderTargetTexture);
			VideoRefresh = true;
		});
		return;
	}
	//if second time enter this func
	if (!IsBeginPlay)
	{
		IsBeginPlay = true;
		AsyncTask(ENamedThreads::GameThread, [=]()
		          {
			          OnPlayerBeginPlay.Broadcast(RenderTargetTexture);
		          }
		);
	}

	if(TextureBufferSize==NewSize)
	{
		TextureWidth = NewWidth;
		TextureHeight = NewHeight;
		std::copy_n(RGBBuffer, NewSize, VideoBuffer);
	}
	else
	{
		if (VideoBuffer)
		{
			delete[] VideoBuffer;
		}
		TextureBufferSize = NewSize;
		TextureWidth = NewWidth;
		TextureHeight = NewHeight;
		VideoBuffer = new uint8[TextureBufferSize];
		std::copy_n(RGBBuffer, NewSize, VideoBuffer);
	}
	VideoRefresh =true;
}

void UTRTCLivePlayer::ResetBuffer()
{
	for (uint32 i = 0; i < TextureWidth * TextureHeight; ++i)
	{
		VideoBuffer[i * 4 + 0] = 0x32;
		VideoBuffer[i * 4 + 1] = 0x32;
		VideoBuffer[i * 4 + 2] = 0x32;
		VideoBuffer[i * 4 + 3] = 0xFF;
	}
	VideoRefresh = false;
}

void UTRTCLivePlayer::RenderVideoFrameToRT()
{
	FScopeLock lock(&Mutex);
	if (!RenderTargetTexture || !VideoBuffer)
		return;
	if (RenderTargetTexture->GetSizeX() != TextureWidth || RenderTargetTexture->GetSizeY() != TextureHeight)
	{
		UE_LOG(LogTemp, Warning, TEXT("remoteBufferSize=%d, remoteWidth=%d, remoteHeight=%d  remoteRenderTargetTexture->GetSizeX =%d , ""remoteRenderTargetTexture->GetSizeY =%d"), TextureBufferSize, TextureWidth, TextureHeight,
		       RenderTargetTexture->GetSizeX(), RenderTargetTexture->GetSizeY());
		auto NewUpdateTextureRegion = new FUpdateTextureRegion2D(0, 0, 0, 0, TextureWidth, TextureHeight);
		auto NewRenderTargetTexture = UTexture2D::CreateTransient(TextureWidth, TextureHeight);

		NewRenderTargetTexture->UpdateResource();
		NewRenderTargetTexture->UpdateTextureRegions(0, 1, NewUpdateTextureRegion, TextureWidth * 4, (uint32)4, VideoBuffer);
		FUpdateTextureRegion2D* TmpRMUpdateTextureRegion = UpdateTextureRegion;
		RenderTargetTexture = NewRenderTargetTexture;
		UpdateTextureRegion = NewUpdateTextureRegion;
		delete TmpRMUpdateTextureRegion;
		return;
	}
	RenderTargetTexture->UpdateTextureRegions(0, 1, UpdateTextureRegion, TextureWidth * 4, (uint32)4, VideoBuffer);
}
