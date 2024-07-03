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

// #include "VlcMediaCallbacks.h"
// #include "VlcMediaSource.h"
// #include "VlcMediaTracks.h"
// #include "VlcMediaView.h"


namespace trtc = liteav;


class IMediaEventSink;
class IMediaOutput;

class FTRTCMediaPlayer : public IMediaPlayer, protected IMediaCache, protected IMediaControls, protected IMediaView
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
	void EnterRoom(trtc::TRTCParams params, trtc::TRTCAppScene scene) const;
	void ExitRoom();



protected:
	/**
	 * Initialize the media player.
	 *
	 * @return true on success, false otherwise.
	 */
	bool InitializePlayer();

protected:
	//~ IMediaControls interface

	virtual bool CanControl(EMediaControl Control) const override;
	virtual FTimespan GetDuration() const override;
	virtual float GetRate() const override;
	virtual EMediaState GetState() const override;
	virtual EMediaStatus GetStatus() const override;
	virtual TRangeSet<float> GetSupportedRates(EMediaRateThinning Thinning) const override;
	virtual FTimespan GetTime() const override;
	virtual bool IsLooping() const override;
	virtual bool Seek(const FTimespan& Time) override;
	virtual bool SetLooping(bool Looping) override;
	virtual bool SetRate(float Rate) override;

private:
	/** VLC callback manager. */
	FTRTCMediaCallbacks Callbacks;

	/** Current playback rate. */
	float CurrentRate;

	/** Current playback time (to work around VLC's broken time tracking). */
	FTimespan CurrentTime;

	/** The media event handler. */
	IMediaEventSink& EventSink;

	/** Media information string. */
	FString Info;

	/** The media source (from URL or archive). */
	FTRTCMediaSource MediaSource;

	/** The VLC media player object. */
	liteav::ue::TRTCCloud* Player;

	/** Whether playback should be looping. */
	bool ShouldLoop;

	/** Track collection. */
	FTRTCMediaTracks Tracks;


	/** View settings. */
	//FTRTCMediaView View;

	FMediaSamples* Samples;




};
