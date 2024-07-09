#pragma once

#include "CoreMinimal.h"
#include "Containers/Queue.h"
#include "IMediaCache.h"
#include "IMediaControls.h"
#include "IMediaPlayer.h"
#include "IMediaSamples.h"
#include "TRTCMediaCallbacks.h"
#include "TRTCMediaSource.h"
#include "TRTCMediaTracks.h"
#include "TRTCMediaView.h"
#include "TRTCCloud.h"


// #include "VlcMediaCallbacks.h"
// #include "VlcMediaSource.h"
// #include "VlcMediaTracks.h"
// #include "VlcMediaView.h"


namespace trtc = liteav;


class IMediaEventSink;
class IMediaOutput;

class FTRTCMediaPlayer : public IMediaPlayer, protected IMediaCache, protected IMediaView
{
public:
	/**
	 * Create and initialize a new instance.
	 *
	 * @param InEventSink The object that receives media events from this player.
	 */
	FTRTCMediaPlayer(IMediaEventSink& InEventSink);

	/** Virtual destructor. */
	virtual ~FTRTCMediaPlayer() override;

public:
	//~ IMediaPlayer interface

	virtual void Close() override;
	virtual IMediaCache& GetCache() override;
	virtual IMediaControls& GetControls() override;
	virtual FString GetInfo() const override;
	virtual FName GetPlayerName() const;
	virtual FGuid GetPlayerPluginGUID() const override;
	virtual IMediaSamples& GetSamples() override;
	virtual FString GetStats() const override;
	virtual IMediaTracks& GetTracks() override;
	virtual FString GetUrl() const override;
	virtual IMediaView& GetView() override;
	virtual bool Open(const FString& Url, const IMediaOptions* Options) override;
	virtual bool Open(const TSharedRef<FArchive, ESPMode::ThreadSafe>& Archive, const FString& OriginalUrl, const IMediaOptions* Options) override;
	virtual void TickInput(FTimespan DeltaTime, FTimespan Timecode) override;


	//enter room for trtc
	void EnterRoom(const TRTCParams& params, TRTCAppScene scene) const;
	void ExitRoom();

protected:
	/**
	 * Initialize the media player.
	 *
	 * @return true on success, false otherwise.
	 */
	bool InitializePlayer();

private:
	/** VLC callback manager. */
	FTRTCMediaCallbacks Callbacks;


	/** The media event handler. */
	IMediaEventSink& EventSink;

	/** Media information string. */
	FString Info;

	// /** The media source (from URL or archive). */
	// FTRTCMediaSource MediaSource;

	/** The VLC media player object. */
	liteav::ue::TRTCCloud* Player;

	/** Current playback rate. */
	float CurrentRate = 30.f;

	/** Current playback time (to work around VLC's broken time tracking). */
	FTimespan CurrentTime;


	/** Track collection. */
	FTRTCMediaTracks Tracks;


	/** View settings. */
	//FTRTCMediaView View;

	FMediaSamples* Samples;
};
