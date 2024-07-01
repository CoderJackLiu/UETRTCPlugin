#pragma once

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



	IMediaSamples& GetSamples();


private:
	/** The output media samples. */
	FMediaSamples* Samples;

};
