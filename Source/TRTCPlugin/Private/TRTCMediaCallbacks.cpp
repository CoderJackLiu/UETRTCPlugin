#include "TRTCMediaCallbacks.h"

#include "MediaSamples.h"


FTRTCMediaCallbacks::FTRTCMediaCallbacks()
	: Samples(new FMediaSamples), Player(nullptr)
{
	//todo liuqi
}

FTRTCMediaCallbacks::~FTRTCMediaCallbacks()
{
	//todo liuqi
	delete Samples;
	Samples = nullptr;
}

IMediaSamples& FTRTCMediaCallbacks::GetSamples()
{
	return *Samples;
}

void FTRTCMediaCallbacks::Initialize(liteav::ue::TRTCCloud& InPlayer)
{
	Shutdown();

	Player = &InPlayer;

	// register callbacks
	// FVlc::AudioSetFormatCallbacks(
	// 	Player,
	// 	&FVlcMediaCallbacks::StaticAudioSetupCallback,
	// 	&FVlcMediaCallbacks::StaticAudioCleanupCallback
	// );
	//
	// FVlc::AudioSetCallbacks(
	// 	Player,
	// 	&FVlcMediaCallbacks::StaticAudioPlayCallback,
	// 	&FVlcMediaCallbacks::StaticAudioPauseCallback,
	// 	&FVlcMediaCallbacks::StaticAudioResumeCallback,
	// 	&FVlcMediaCallbacks::StaticAudioFlushCallback,
	// 	&FVlcMediaCallbacks::StaticAudioDrainCallback,
	// 	this
	// );
	//
	// FVlc::VideoSetFormatCallbacks(
	// 	Player,
	// 	&FVlcMediaCallbacks::StaticVideoSetupCallback,
	// 	&FVlcMediaCallbacks::StaticVideoCleanupCallback
	// );
	//
	// FVlc::VideoSetCallbacks(
	// 	Player,
	// 	&FVlcMediaCallbacks::StaticVideoLockCallback,
	// 	&FVlcMediaCallbacks::StaticVideoUnlockCallback,
	// 	&FVlcMediaCallbacks::StaticVideoDisplayCallback,
	// 	this
	// );
}

void FTRTCMediaCallbacks::Shutdown()
{
	if (Player == nullptr)
	{
		return;
	}

	// unregister callbacks
	// FVlc::AudioSetCallbacks(Player, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr);
	// FVlc::AudioSetFormatCallbacks(Player, nullptr, nullptr);
	//
	// FVlc::VideoSetCallbacks(Player, nullptr, nullptr, nullptr, nullptr);
	// FVlc::VideoSetFormatCallbacks(Player, nullptr, nullptr);
	//
	// AudioSamplePool->Reset();
	// VideoSamplePool->Reset();
	//
	CurrentTime = FTimespan::Zero();
	Player = nullptr;
}
