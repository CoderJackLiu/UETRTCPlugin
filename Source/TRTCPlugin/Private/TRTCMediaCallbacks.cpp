#include "TRTCMediaCallbacks.h"

#include "MediaSamples.h"

FTRTCMediaCallbacks::FTRTCMediaCallbacks()
{
	//todo liuqi

}

FTRTCMediaCallbacks::~FTRTCMediaCallbacks()
{
	//todo liuqi
}

IMediaSamples& FTRTCMediaCallbacks::GetSamples()
{
	return *Samples;
}
