#pragma once
#include "IMediaTracks.h"
#include "TRTCSDK/Android/include/TXLiteAVCode.h"
// #include "TRTCCloudCallback.h"
#include "TRTCCloud.h"


class FTRTCMediaTracks: public IMediaTracks , public ITRTCVideoRenderCallback, public ITRTCCloudCallback
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

public:

	/**
	 * Initialize this object for the specified VLC media player.
	 *
	 * @param InPlayer The VLC media player.
	 * @param OutInfo Will contain information about the available media tracks.
	 */
	void Initialize(liteav::ue::TRTCCloud& InPlayer, FString& OutInfo);

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
	
	void onExitRoom(int reason) override;
	void onEnterRoom(int result) override;
	void onUserVideoAvailable(const char* userId, bool available) override;
	void onUserSubStreamAvailable(const char* userId, bool available) override;
	void onError(TXLiteAVError errCode, const char* errMsg, void* extraInfo) override;
	void onWarning(TXLiteAVWarning warningCode, const char* warningMsg, void* extraInfo) override;

	void WriteLblLog(const char* log);
	void writeCallbackLog(const char* log);

	// ITRTCVideoRenderCallback Customize Video Rendering Callback
	void onRenderVideoFrame(const char* userId, TRTCVideoStreamType streamType, TRTCVideoFrame* frame) override;
	
	void ResetBuffer(bool isLocal);

private:

	/** Audio track descriptors. */
	TArray<FTrack> AudioTracks;

	/** Caption track descriptors. */
	TArray<FTrack> CaptionTracks;

	/** The VLC media player object. */
	// FLibvlcMediaPlayer* Player;

	/** Video track descriptors. */
	TArray<FTrack> VideoTracks;

	FTimespan TargetTime;

	/** The VLC media player object. */
	liteav::ue::TRTCCloud* Player;
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
};
