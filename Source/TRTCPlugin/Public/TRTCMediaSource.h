﻿#pragma once
#include "TRTCCloud.h"


//todo liuqi
class FTRTCMediaSource
{
public:
	/**
	 * Create and initialize a new instance.
	 *
	 * @param InInstance The LibVLC instance to use.
	 */
	FTRTCMediaSource(liteav::ue::TRTCCloud* InTRTCInstance);



public:

	// /** Get the media object. */
	// FLibvlcMedia* GetMedia() const
	// {
	// 	return Media;
	// }

	/** Get the URL of the currently open media source. */
	const FString& GetCurrentUrl() const
	{
		return CurrentUrl;
	}

	/**
	 * Get the duration of the media source.
	 *
	 * @return Media duration.
	 */
	FTimespan GetDuration() const;

	/**
	 * Open a media source using the given archive.
	 *
	 * You must call Close() if this media source is open prior to calling this method.
	 *
	 * @param Archive The archive to read media data from.
	 * @return The media object.
	 * @see OpenUrl, Close
	 */
	//FLibvlcMedia* OpenArchive(const TSharedRef<FArchive, ESPMode::ThreadSafe>& Archive, const FString& OriginalUrl);

	/**
	 * Open a media source from the specified URL.
	 *
	 * You must call Close() if this media source is open prior to calling this method.
	 *
	 * @param Url The media resource locator.
	 * @return The media object.
	 * @see OpenArchive, Close
	 */
	//FLibvlcMedia* OpenUrl(const FString& Url);

	/**
	 * Close the media source.
	 *
	 * @see OpenArchive, OpenUrl
	 */
	void Close();
	
private:

	/** Handles open callbacks from VLC. */
	static int HandleMediaOpen(void* Opaque, void** OutData, uint64* OutSize);

	/** Handles read callbacks from VLC. */
	static SSIZE_T HandleMediaRead(void* Opaque, void* Buffer, SIZE_T Length);

	/** Handles seek callbacks from VLC. */
	static int HandleMediaSeek(void* Opaque, uint64 Offset);

	/** Handles close callbacks from VLC. */
	static void HandleMediaClose(void* Opaque);
	
private:
	/** Currently opened media. */
	FString CurrentUrl;

	/** The file or memory archive to stream from (for local media only). */
	TSharedPtr<FArchive, ESPMode::ThreadSafe> Data;

	liteav::ue::TRTCCloud* TRTCInstance;
};
