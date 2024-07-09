#include "TRTCMediaFactory.h"

#include "IMediaModule.h"
#include "TRTCPlugin.h"

#define LOCTEXT_NAMESPACE "FTRTCMediaFactoryModule"

void FTRTCMediaFactoryModule::StartupModule()
{
	// supported platforms
	SupportedPlatforms.Add(TEXT("Linux"));
	SupportedPlatforms.Add(TEXT("Mac"));
	SupportedPlatforms.Add(TEXT("Windows"));
	SupportedPlatforms.Add(TEXT("WinRT"));



	// register player factory
	// auto MediaModule = FModuleManager::LoadModulePtr<IMediaModule>("Media");
	//
	// if (MediaModule != nullptr)
	// {
	// 	MediaModule->RegisterPlayerFactory(*this);
	// }
	//
}

void FTRTCMediaFactoryModule::ShutdownModule()
{
	// unregister player factory
	auto MediaModule = FModuleManager::GetModulePtr<IMediaModule>("Media");

	if (MediaModule != nullptr)
	{
		MediaModule->UnregisterPlayerFactory(*this);
	}
}

bool FTRTCMediaFactoryModule::CanPlayUrl(const FString& Url, const IMediaOptions* Options, TArray<FText>* OutWarnings, TArray<FText>* OutErrors) const
{
	return true;
}

TSharedPtr<IMediaPlayer, ESPMode::ThreadSafe> FTRTCMediaFactoryModule::CreatePlayer(IMediaEventSink& EventSink)
{
	auto TRTCMediaModule = FModuleManager::LoadModulePtr<ITRTCMediaModule>("TRTCPlugin");
	return (TRTCMediaModule != nullptr) ? TRTCMediaModule->CreatePlayer(EventSink) : nullptr;
}

FText FTRTCMediaFactoryModule::GetDisplayName() const
{
	return LOCTEXT("MediaPlayerDisplayName", "TRTC Player");
}

FName FTRTCMediaFactoryModule::GetPlayerName() const
{
	static FName PlayerName(TEXT("TRTCMedia"));
	return PlayerName;
}

FGuid FTRTCMediaFactoryModule::GetPlayerPluginGUID() const
{
	static FGuid PlayerPluginGUID(TEXT("TRTCMedia"));
	return PlayerPluginGUID;
}

const TArray<FString>& FTRTCMediaFactoryModule::GetSupportedPlatforms() const
{
	return SupportedPlatforms;
}

bool FTRTCMediaFactoryModule::SupportsFeature(EMediaFeature Feature) const
{
	return ((Feature == EMediaFeature::AudioSamples) ||
		(Feature == EMediaFeature::AudioTracks) ||
		(Feature == EMediaFeature::CaptionTracks) ||
		(Feature == EMediaFeature::VideoSamples) ||
		(Feature == EMediaFeature::VideoTracks));
}

#undef LOCTEXT_NAMESPACE

IMPLEMENT_MODULE(FTRTCMediaFactoryModule, TRTCMediaFactory)
