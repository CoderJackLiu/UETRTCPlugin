#pragma once
#include "TRTCCloud.h"

class FMediaSamples;
class FVlcMediaAudioSamplePool;
class FVlcMediaTextureSamplePool;
class IMediaOptions;
class IMediaAudioSink;
class IMediaOverlaySink;
class IMediaSamples;
class IMediaTextureSink;

class FTRTCMediaCallbacks
{
public:
	/** Default constructor. */
	FTRTCMediaCallbacks();

	/** Virtual destructor. */
	~FTRTCMediaCallbacks();


	/**
	 * Get the output media samples.
	 *
	 * @return Media samples interface.
	 */
	IMediaSamples& GetSamples();

	/**
 * Initialize the handler for the specified media player.
 *
 * @param InPlayer The media player that owns this handler.
 */
	void Initialize(liteav::ue::TRTCCloud& InPlayer);
	/** Shut down the callback handler. */
	void Shutdown();
	
	void SetCurrentTime(FTimespan Timespan)
	{
		CurrentTime = Timespan;
	}

private:
	/** The output media samples. */
	FMediaSamples* Samples;

	/** The player's current time. */
	FTimespan CurrentTime = FTimespan::Zero();

	/** The VLC media player object. */
	liteav::ue::TRTCCloud* Player;
};
