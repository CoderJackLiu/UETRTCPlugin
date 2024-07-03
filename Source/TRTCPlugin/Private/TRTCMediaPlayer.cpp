#include "TRTCMediaPlayer.h"

#include "IMediaEventSink.h"
#include "IMediaOptions.h"
#include "MediaSamples.h"
#include "TRTCCloud.h"
#include "TRTCLogMacro.h"
#include "TRTCMediaCallbacks.h"
#include "TRTCTestTool.h"
#include "Serialization/ArrayReader.h"


FTRTCMediaPlayer::FTRTCMediaPlayer(IMediaEventSink& InEventSink)
	: CurrentRate(0)
	  , CurrentTime(FTimespan::Zero())
	  , EventSink(InEventSink)
	  //todo liuqi 也许需要赋值
	  , MediaSource(nullptr)
	  , Player(nullptr)
	  , ShouldLoop(false)
	  , Samples(new FMediaSamples)
{
}

FTRTCMediaPlayer::~FTRTCMediaPlayer()
{
	FTRTCMediaPlayer::Close();
}

void FTRTCMediaPlayer::Close()
{
	//todo liuqi 关闭播放器

	if (Player == nullptr)
	{
		return;
	}
	ExitRoom();
	// detach callback handlers
	Callbacks.Shutdown();
	Tracks.Shutdown();
	// View.Shutdown();

	// release player
	if (Player != nullptr)
	{
		Player->exitRoom();
		//todo liuqi 接口继承
		// Player->removeCallback(this);
		Player->destroySharedInstance();
		Player = nullptr;
	}
	//todo liuqi 放置内存泄露
	// delete[] localBuffer;
	// delete localUpdateTextureRegion;
	// delete[] remoteBuffer;
	// delete remoteUpdateTextureRegion;
	Player = nullptr;

	// reset fields
	CurrentRate = 0.0f;
	CurrentTime = FTimespan::Zero();
	MediaSource.Close();
	Info.Empty();

	// notify listeners
	EventSink.ReceiveMediaEvent(EMediaEvent::TracksChanged);
	EventSink.ReceiveMediaEvent(EMediaEvent::MediaClosed);
}

IMediaCache& FTRTCMediaPlayer::GetCache()
{
	return *this;
}

IMediaControls& FTRTCMediaPlayer::GetControls()
{
	return *this;
}

FString FTRTCMediaPlayer::GetInfo() const
{
	return Info;
}

FName FTRTCMediaPlayer::GetPlayerName() const
{
	static FName PlayerName(TEXT("TRTCMedia"));
	return PlayerName;
}

FGuid FTRTCMediaPlayer::GetPlayerPluginGUID() const
{
	static FGuid GetPlayerPluginGUID(TEXT("TRTCMedia"));
	return GetPlayerPluginGUID;
}

IMediaSamples& FTRTCMediaPlayer::GetSamples()
{
	return Callbacks.GetSamples();
}

FString FTRTCMediaPlayer::GetStats() const
{
	//todo liuqi   看看如何获取最新的TRTC播放器的状态
	return FString();
}

IMediaTracks& FTRTCMediaPlayer::GetTracks()
{
	return Tracks;
}

FString FTRTCMediaPlayer::GetUrl() const
{
	return MediaSource.GetCurrentUrl();
}

IMediaView& FTRTCMediaPlayer::GetView()
{
	return *this;
}

bool FTRTCMediaPlayer::Open(const FString& Url, const IMediaOptions* Options)
{
	Close();

	if (Url.IsEmpty())
	{
		return false;
	}

	//todo 播放本地文件和云端url
	if (Url.StartsWith(TEXT("file://")))
	{
		// open local files via platform file system
		TSharedPtr<FArchive, ESPMode::ThreadSafe> Archive;
		const TCHAR* FilePath = &Url[7];

		if ((Options != nullptr) && Options->GetMediaOption("PrecacheFile", false))
		{
			FArrayReader* Reader = new FArrayReader;

			if (FFileHelper::LoadFileToArray(*Reader, FilePath))
			{
				Archive = MakeShareable(Reader);
			}
			else
			{
				delete Reader;
			}
		}
		else
		{
			Archive = MakeShareable(IFileManager::Get().CreateFileReader(FilePath));
		}

		if (!Archive.IsValid())
		{
			UE_LOG(LogTRTCMedia, Warning, TEXT("Failed to open media file: %s"), FilePath);
			return false;
		}

		//播放本地缓存文件加载进来的数据
		// if (!MediaSource.OpenArchive(Archive.ToSharedRef(), Url))
		// {
		// 	return false;
		// }
	}
	//todo liuqi 播放云端TRTC
	// else if (!MediaSource.OpenUrl(Url))
	// {
	// 	return false;
	// }

	return InitializePlayer();
}

bool FTRTCMediaPlayer::Open(const TSharedRef<FArchive, ESPMode::ThreadSafe>& Archive, const FString& OriginalUrl, const IMediaOptions* Options)
{
	Close();

	if (Archive->TotalSize() == 0)
	{
		UE_LOG(LogTRTCMedia, Warning, TEXT("TRTCMediaPlayer %p: Cannot open media from archive (archive is empty)"), this);
		return false;
	}

	if (OriginalUrl.IsEmpty())
	{
		UE_LOG(LogTRTCMedia, Warning, TEXT("TRTCMediaPlayer %p: Cannot open media from archive (no original URL provided)"), this);
		return false;
	}

	return InitializePlayer();
}

void FTRTCMediaPlayer::TickInput(FTimespan DeltaTime, FTimespan Timecode)
{
	if (Player == nullptr)
	{
		return;
	}
	Callbacks.SetCurrentTime(CurrentTime);
}

void FTRTCMediaPlayer::EnterRoom(trtc::TRTCParams params, trtc::TRTCAppScene scene) const
{
	Player->enterRoom(params, scene);
	//Player->startLocalAudio(trtc::TRTCAudioQualityDefault);
}

void FTRTCMediaPlayer::ExitRoom()
{
	Player->exitRoom();
}

bool FTRTCMediaPlayer::InitializePlayer()
{
	// create player for media source
	Player = liteav::ue::TRTCCloud::getSharedInstance();
	if (Player == nullptr)
	{
		//todo 
		// UE_LOG(LogTRTCMedia, Warning, TEXT("Failed to initialize media player: %s"), ANSI_TO_TCHAR(FVlc::Errmsg()));
		return false;
	}
	Player->addCallback(&Tracks);

	//todo liuqi  后期从项目设置中获取到对应的APPKEY SECRET 等信息 动态获取uid, roomid等信息
	TRTCParams params;
	params.role = TRTCRoleAudience;
	params.sdkAppId = 1600042662;
	params.userId = "user1";
	const char* userSig = "7ac4ceb6a62da74fa2743c8297da50b2a27faf8500360088acc1e3f3ad7fc193";
	std::string sSig = liteav::ue::TestUserSigGenerator().gen(params.userId, params.sdkAppId, userSig);
	params.userSig = sSig.c_str();
	params.roomId = 123;
	EnterRoom(params, TRTCAppSceneLIVE);
	//todo 
	// attach to event managers
	// FLibvlcEventManager* MediaEventManager = FVlc::MediaEventManager(MediaSource.GetMedia());
	// FLibvlcEventManager* PlayerEventManager = FVlc::MediaPlayerEventManager(Player);
	//
	// if ((MediaEventManager == nullptr) || (PlayerEventManager == nullptr))
	// {
	// 	FVlc::MediaPlayerRelease(Player);
	// 	Player = nullptr;
	//
	// 	return false;
	// }
	//
	// FVlc::EventAttach(MediaEventManager, ELibvlcEventType::MediaParsedChanged, &FVlcMediaPlayer::StaticEventCallback, this);
	// FVlc::EventAttach(PlayerEventManager, ELibvlcEventType::MediaPlayerEndReached, &FVlcMediaPlayer::StaticEventCallback, this);
	// FVlc::EventAttach(PlayerEventManager, ELibvlcEventType::MediaPlayerPlaying, &FVlcMediaPlayer::StaticEventCallback, this);
	// FVlc::EventAttach(PlayerEventManager, ELibvlcEventType::MediaPlayerPositionChanged, &FVlcMediaPlayer::StaticEventCallback, this);
	// FVlc::EventAttach(PlayerEventManager, ELibvlcEventType::MediaPlayerStopped, &FVlcMediaPlayer::StaticEventCallback, this);
	//
	// // initialize player
	// CurrentRate = 0.0f;
	// CurrentTime = FTimespan::Zero();
	//
	// EventSink.ReceiveMediaEvent(EMediaEvent::MediaOpened);

	return true;
}

bool FTRTCMediaPlayer::CanControl(EMediaControl Control) const
{
	if (Player == nullptr)
	{
		return false;
	}

	//todo liuqi 看看这个能不能暂停 恢复 seek

	// if (Control == EMediaControl::Pause)
	// {
	// 	return (FVlc::MediaPlayerCanPause(Player) != 0);
	// }
	//
	// if (Control == EMediaControl::Resume)
	// {
	// 	return (FVlc::MediaPlayerGetState(Player) != ELibvlcState::Playing);
	// }
	//
	// if ((Control == EMediaControl::Scrub) || (Control == EMediaControl::Seek))
	// {
	// 	return (FVlc::MediaPlayerIsSeekable(Player) != 0);
	// }

	return false;
}

FTimespan FTRTCMediaPlayer::GetDuration() const
{
	return MediaSource.GetDuration();
}

float FTRTCMediaPlayer::GetRate() const
{
	return CurrentRate;
}

EMediaState FTRTCMediaPlayer::GetState() const
{
	if (Player == nullptr)
	{
		return EMediaState::Closed;
	}

	//todo liuqi  获取播放器状态！

	// ELibvlcState State = FVlc::MediaPlayerGetState(Player);
	//
	// switch (State)
	// {
	// case ELibvlcState::Error:
	// 	return EMediaState::Error;
	//
	// case ELibvlcState::Buffering:
	// case ELibvlcState::Opening:
	// 	return EMediaState::Preparing;
	//
	// case ELibvlcState::Paused:
	// 	return EMediaState::Paused;
	//
	// case ELibvlcState::Playing:
	// 	return EMediaState::Playing;
	//
	// case ELibvlcState::Ended:
	// case ELibvlcState::NothingSpecial:
	// case ELibvlcState::Stopped:
	// 	return EMediaState::Stopped;
	// }

	return EMediaState::Error; // should never get here
}

EMediaStatus FTRTCMediaPlayer::GetStatus() const
{
	return (GetState() == EMediaState::Preparing) ? EMediaStatus::Buffering : EMediaStatus::None;
}

TRangeSet<float> FTRTCMediaPlayer::GetSupportedRates(EMediaRateThinning Thinning) const
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

FTimespan FTRTCMediaPlayer::GetTime() const
{
	return CurrentTime;
}

bool FTRTCMediaPlayer::IsLooping() const
{
	return ShouldLoop;
}

bool FTRTCMediaPlayer::Seek(const FTimespan& Time)
{
	//todo liuqi seek trtc 可能不支持seek 需要查Doc

	// ELibvlcState State = FVlc::MediaPlayerGetState(Player);
	//
	// if ((State == ELibvlcState::Opening) ||
	// 	(State == ELibvlcState::Buffering) ||
	// 	(State == ELibvlcState::Error))
	// {
	// 	return false;
	// }
	//
	// if (Time != CurrentTime)
	// {
	// 	FVlc::MediaPlayerSetTime(Player, Time.GetTotalMilliseconds());
	// 	CurrentTime = Time;
	// }

	return true;
}

bool FTRTCMediaPlayer::SetLooping(bool Looping)
{
	//todo liuqi
	ShouldLoop = Looping;
	return true;
}

bool FTRTCMediaPlayer::SetRate(float Rate)
{
	if (Player == nullptr)
	{
		return false;
	}

	//todo 播放 设置播放频率
	//
	// if ((FVlc::MediaPlayerSetRate(Player, Rate) == -1))
	// {
	// 	return false;
	// }
	//
	// if (FMath::IsNearlyZero(Rate))
	// {
	// 	if (FVlc::MediaPlayerGetState(Player) == ELibvlcState::Playing)
	// 	{
	// 		if (FVlc::MediaPlayerCanPause(Player) == 0)
	// 		{
	// 			return false;
	// 		}
	//
	// 		FVlc::MediaPlayerPause(Player);
	// 	}
	// }
	// else if (FVlc::MediaPlayerGetState(Player) != ELibvlcState::Playing)
	// {
	// 	if (FVlc::MediaPlayerPlay(Player) == -1)
	// 	{
	// 		return false;
	// 	}
	// }

	return true;
}
