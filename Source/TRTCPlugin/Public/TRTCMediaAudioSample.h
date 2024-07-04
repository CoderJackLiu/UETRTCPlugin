﻿#pragma once
#include "CoreTypes.h"
#include "IMediaAudioSample.h"
#include "MediaObjectPool.h"
#include "Misc/Timespan.h"
#include "Templates/SharedPointer.h"

class FTRTCMediaAudioSample
	: public IMediaAudioSample
	  , public IMediaPoolable
{
public:
	/** Default constructor. */
	FTRTCMediaAudioSample()
		: Buffer(nullptr)
		  , BufferSize(0)
		  , Channels(0)
		  , Duration(FTimespan::Zero())
		  , Frames(0)
		  , SampleFormat(EMediaAudioSampleFormat::Undefined)
		  , SampleRate(0)
		  , Time(FTimespan::Zero())
	{
	}

	/** Virtual destructor. */
	virtual ~FTRTCMediaAudioSample() override
	{
		FreeBuffer();
	}

public:
	/**
	 * Initialize the sample.
	 *
	 * @param InBuffer The raw audio sample buffer.
	 * @param InBufferSize The size of the buffer (in bytes).
	 * @param InFrames Number of frames in the buffer.
	 * @param InChannels Number of audio channels.
	 * @param InSampleFormat The sample format.
	 * @param InSampleRate The sample rate.
	 * @param InTime The sample time (in the player's local clock).
	 * @param InDuration The duration for which the sample is valid.
	 * @return true on success, false otherwise.
	 */
	bool Initialize(
		void* InBuffer,
		uint32 InBufferSize,
		uint32 InFrames,
		uint32 InChannels,
		EMediaAudioSampleFormat InSampleFormat,
		uint32 InSampleRate,
		FTimespan InTime,
		FTimespan InDuration)
	{
		if ((InBuffer == nullptr) || (InBufferSize == 0) || (InSampleFormat == EMediaAudioSampleFormat::Undefined))
		{
			return false;
		}

		if (InBufferSize > BufferSize)
		{
			Buffer = FMemory::Realloc(Buffer, InBufferSize);
			BufferSize = InBufferSize;
		}

		FMemory::Memcpy(Buffer, InBuffer, InBufferSize);

		Channels = InChannels;
		Duration = InDuration;
		Frames = InFrames;
		SampleFormat = InSampleFormat;
		SampleRate = InSampleRate;
		Time = InTime;

		return true;
	}

public:
	//~ IMediaAudioSample interface

	virtual const void* GetBuffer() override
	{
		return Buffer;
	}

	virtual uint32 GetChannels() const override
	{
		return Channels;
	}

	virtual uint32 GetFrames() const override
	{
		return Frames;
	}

	virtual FTimespan GetDuration() const override
	{
		return Duration;
	}

	virtual EMediaAudioSampleFormat GetFormat() const override
	{
		return SampleFormat;
	}

	virtual uint32 GetSampleRate() const override
	{
		return SampleRate;
	}

	virtual FMediaTimeStamp GetTime() const override
	{
		return Time;
	}

protected:
	/** Free the sample buffer. */
	void FreeBuffer()
	{
		if (Buffer != nullptr)
		{
			FMemory::Free(Buffer);

			Buffer = nullptr;
			BufferSize = 0;
		}
	}

private:
	/** The sample's frame buffer. */
	void* Buffer;

	/** Allocated size of the buffer (in bytes). */
	SIZE_T BufferSize;

	/** Number of audio channels. */
	uint32 Channels;

	/** Duration for which the sample is valid. */
	FTimespan Duration;

	/** Number of frames in the buffer. */
	uint32 Frames;

	/** The sample format. */
	EMediaAudioSampleFormat SampleFormat;

	/** Audio sample rate (in samples per second). */
	uint32 SampleRate;

	/** Play time for which the sample was generated. */
	FTimespan Time;
};

/** Implements a pool for VLC audio sample objects. */
class FTRTCMediaAudioSamplePool : public TMediaObjectPool<FTRTCMediaAudioSample>
{
};
