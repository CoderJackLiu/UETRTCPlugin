#pragma once
#include "IMediaTracks.h"
#include "TRTCSDK/Android/include/TXLiteAVCode.h"
// #include "TRTCCloudCallback.h"
#include "IMediaControls.h"
#include "TRTCCloud.h"


enum class EMediaAudioSampleFormat;
class FTRTCMediaAudioSamplePool;
enum class EMediaTextureSampleFormat;
class FTRTCMediaTextureSamplePool;
class FMediaSamples;
class IMediaSamples;


class FTRTCMediaTracks : public IMediaTracks, public IMediaControls, public ITRTCVideoRenderCallback, public ITRTCCloudCallback
{
public:
	struct FTrack
	{
		FText DisplayName;
		int32 Id;
		FString Name;
	};

public:
	/** Default constructor. */
	FTRTCMediaTracks();

	IMediaSamples& GetSamples() const;

	/**
	 * Set the player's current time.
	 *
	 * @param Time The player's play time.
	 */
	void SetCurrentTime(FTimespan Time);


public:
	/**
	 * Initialize this object for the specified VLC media player.
	 *
	 * @param InPlayer The VLC media player.
	 * @param OutInfo Will contain information about the available media tracks.
	 */
	void Initialize(ue::TRTCCloud& InPlayer, FString& OutInfo);

	// void TickInput(FTimespan DeltaTime, FTimespan Timecode);

	/** Shut down this object. */
	void Shutdown();
	//~ IMediaTracks interface

	virtual bool GetAudioTrackFormat(int32 TrackIndex, int32 FormatIndex, FMediaAudioTrackFormat& OutFormat) const override;
	virtual int32 GetNumTracks(EMediaTrackType TrackType) const override;
	virtual int32 GetNumTrackFormats(EMediaTrackType TrackType, int32 TrackIndex) const override;
	virtual int32 GetSelectedTrack(EMediaTrackType TrackType) const override;
	virtual FText GetTrackDisplayName(EMediaTrackType TrackType, int32 TrackIndex) const override;
	virtual int32 GetTrackFormat(EMediaTrackType TrackType, int32 TrackIndex) const override;
	virtual FString GetTrackLanguage(EMediaTrackType TrackType, int32 TrackIndex) const override;
	virtual FString GetTrackName(EMediaTrackType TrackType, int32 TrackIndex) const override;
	virtual bool GetVideoTrackFormat(int32 TrackIndex, int32 FormatIndex, FMediaVideoTrackFormat& OutFormat) const override;
	virtual bool SelectTrack(EMediaTrackType TrackType, int32 TrackIndex) override;
	virtual bool SetTrackFormat(EMediaTrackType TrackType, int32 TrackIndex, int32 FormatIndex) override;

private:
	virtual void onExitRoom(int reason) override;
	virtual void onEnterRoom(int result) override;
	virtual void onUserVideoAvailable(const char* userId, bool available) override;
	virtual void onUserSubStreamAvailable(const char* userId, bool available) override;
	virtual void onError(TXLiteAVError errCode, const char* errMsg, void* extraInfo) override;
	virtual void onWarning(TXLiteAVWarning warningCode, const char* warningMsg, void* extraInfo) override;

	virtual void onFirstVideoFrame(const char* userId, const TRTCVideoStreamType streamType, const int width, const int height) override;
	virtual void onFirstAudioFrame(const char* userId) override;
	virtual void onNetworkQuality(TRTCQualityInfo localQuality, TRTCQualityInfo* remoteQuality, uint32_t remoteQualityCount) override;
	virtual void onStatistics(const TRTCStatistics& statistics) override;
	virtual void onSpeedTestResult(const TRTCSpeedTestResult& result) override;
	virtual void onConnectionLost() override;
	virtual void onTryToReconnect() override;

	void WriteLblLog(const char* log) const;
	void writeCallbackLog(const char* log) const;

	// ITRTCVideoRenderCallback Customize Video Rendering Callback
	virtual void onRenderVideoFrame(const char* userId, TRTCVideoStreamType streamType, TRTCVideoFrame* frame) override;

	void ResetBuffer(bool isLocal);
	void UpdateBuffer(char* RGBBuffer, uint32_t Width, uint32_t Height, uint32_t Size, bool isLocal);

protected:
	//~ IMediaControls interface

	virtual bool CanControl(EMediaControl Control) const override;
	virtual FTimespan GetDuration() const override;
	virtual float GetRate() const override;
	virtual EMediaStatus GetStatus() const override;
	virtual TRangeSet<float> GetSupportedRates(EMediaRateThinning Thinning) const override;
	virtual FTimespan GetTime() const override;
	virtual bool IsLooping() const override;
	virtual bool Seek(const FTimespan& Time) override;
	virtual bool SetLooping(bool Looping) override;
	virtual bool SetRate(float Rate) override;

public:
	virtual EMediaState GetState() const override;

private:
	//local
	FCriticalSection localMutex;
	uint8* localBuffer = nullptr;
	uint32_t localWidth = 0;
	uint32_t localHeight = 0;
	bool localRefresh = false;
	uint32 localBufferSize = 0;

	//remote 
	FCriticalSection remoteMutex;
	uint8* remoteBuffer = nullptr;
	uint32_t remoteWidth = 0;
	uint32_t remoteHeight = 0;
	bool remoteRefresh = false;
	uint32 remoteBufferSize = 0;

private:
	/** Audio track descriptors. */
	TArray<FTrack> AudioTracks;

	/** Caption track descriptors. */
	TArray<FTrack> CaptionTracks;

	/** The TRTC media player object. */
	ue::TRTCCloud* Player;

	/** Video track descriptors. */
	TArray<FTrack> VideoTracks;

	FTimespan TargetTime;

	/** The output media samples. */
	FMediaSamples* Samples;

	/** Current number of channels in audio samples( accessed by VLC thread only). */
	uint32 AudioChannels;

	/** Current audio sample format (accessed by VLC thread only). */
	EMediaAudioSampleFormat AudioSampleFormat;

	/** Audio sample object pool. */
	FTRTCMediaAudioSamplePool* AudioSamplePool;

	/** Current audio sample rate (accessed by VLC thread only). */
	uint32 AudioSampleRate;

	/** Size of a single audio sample (in bytes). */
	SIZE_T AudioSampleSize;

	/** The player's current time. */
	FTimespan CurrentTime;

	/** Current video buffer dimensions (accessed by VLC thread only; may be larger than VideoOutputDim). */
	FIntPoint VideoBufferDim;

	/** Number of bytes per row of video pixels. */
	uint32 VideoBufferStride;

	/** Current duration of video frames. */
	FTimespan VideoFrameDuration;

	/** Current video output dimensions (accessed by VLC thread only). */
	FIntPoint VideoOutputDim;

	/** Play time of the previous frame. */
	FTimespan VideoPreviousTime;

	/** Current video sample format (accessed by VLC thread only). */
	EMediaTextureSampleFormat VideoSampleFormat;

	/** Video sample object pool. */
	FTRTCMediaTextureSamplePool* VideoSamplePool;

private:
	/** Media playback state. */
	EMediaState CurrentState = EMediaState::Closed;

	/** Current playback rate. */
	float CurrentRate = 0.0f;

	/** Media information string. */
	FString Info;

	/** Whether playback should be looping. */
	bool ShouldLoop;
};
