#include "TRTCMediaTracks.h"

//todo liuqi 完善所有实现

FTRTCMediaTracks::FTRTCMediaTracks()
{
}

void FTRTCMediaTracks::Shutdown()
{
	
}

bool FTRTCMediaTracks::GetAudioTrackFormat(int32 TrackIndex, int32 FormatIndex, FMediaAudioTrackFormat& OutFormat) const
{
	return false;
}

int32 FTRTCMediaTracks::GetNumTracks(EMediaTrackType TrackType) const
{
	return 0;
}

int32 FTRTCMediaTracks::GetNumTrackFormats(EMediaTrackType TrackType, int32 TrackIndex) const
{
	return 0;
}

int32 FTRTCMediaTracks::GetSelectedTrack(EMediaTrackType TrackType) const
{
	return 0;

}

FText FTRTCMediaTracks::GetTrackDisplayName(EMediaTrackType TrackType, int32 TrackIndex) const
{
	return FText();
}

int32 FTRTCMediaTracks::GetTrackFormat(EMediaTrackType TrackType, int32 TrackIndex) const
{
	return 0;

}

FString FTRTCMediaTracks::GetTrackLanguage(EMediaTrackType TrackType, int32 TrackIndex) const
{
	return FString();
}

FString FTRTCMediaTracks::GetTrackName(EMediaTrackType TrackType, int32 TrackIndex) const
{
	return FString();
}

bool FTRTCMediaTracks::GetVideoTrackFormat(int32 TrackIndex, int32 FormatIndex, FMediaVideoTrackFormat& OutFormat) const
{
	return false;
}

bool FTRTCMediaTracks::SelectTrack(EMediaTrackType TrackType, int32 TrackIndex)
{
	return false;
}

bool FTRTCMediaTracks::SetTrackFormat(EMediaTrackType TrackType, int32 TrackIndex, int32 FormatIndex)
{
	return false;
}
