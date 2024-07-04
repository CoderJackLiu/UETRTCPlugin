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
	: EventSink(InEventSink)
	  , Player(nullptr)
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
	// MediaSource.Close();
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
	return Tracks;
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
	return Tracks.GetSamples();
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
	return "test";
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
	EMediaState State = Tracks.GetState();
	if (State == EMediaState::Playing)
	{
		CurrentTime += DeltaTime * CurrentRate;
	}
	else
	{
		CurrentRate = 0.f;
	}
	Tracks.SetCurrentTime(CurrentTime);
}

void FTRTCMediaPlayer::EnterRoom(const TRTCParams& params, TRTCAppScene scene) const
{
	Player->enterRoom(params, scene);
	Player->stopLocalAudio();
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
	Tracks.Initialize(*Player, Info);
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
	return true;
}
