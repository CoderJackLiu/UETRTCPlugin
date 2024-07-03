#pragma once

#include "CoreMinimal.h"
#include "IMediaPlayerFactory.h"
#include "Modules/ModuleManager.h"








class FTRTCMediaFactoryModule : public IModuleInterface, public IMediaPlayerFactory
{
public:
	virtual void StartupModule() override;
	virtual void ShutdownModule() override;

	//Implementation of IMediaPlayerFactory

	/**
	 * Whether the player can play the specified source URL.
	 *
	 * @param Url The media source URL to check.
	 * @param Options Optional media player parameters.
	 * @param OutWarnings Will contain warning messages (optional).
	 * @param OutErrors will contain error messages (optional).
	 * @return true if the source can be played, false otherwise.
	 */
	virtual bool CanPlayUrl(const FString& Url, const IMediaOptions* Options, TArray<FText>* OutWarnings, TArray<FText>* OutErrors) const override;

	/**
	 * Creates a media player.
	 *
	 * @param EventSink The object that will receive the player's events.
	 * @return A new media player, or nullptr if a player couldn't be created.
	 */
	virtual TSharedPtr<IMediaPlayer, ESPMode::ThreadSafe> CreatePlayer(IMediaEventSink& EventSink) override;

	/**
	 * Get the human readable name of the player.
	 *
	 * @return Display name text.
	 * @see GetName
	 */
	virtual FText GetDisplayName() const override;

	/**
	 * Get the unique name of the media player.
	 *
	 * @return Media player name, i.e. 'AndroidMedia' or 'WmfMedia'.
	 * @see GetDisplayName
	 */
	virtual FName GetPlayerName() const override;

	/**
	 * Get the unique name of the media player.
	 *
	 * @return Media player name, i.e. 'AndroidMedia' or 'WmfMedia'.
	 * @see GetDisplayName
	 */
	virtual FGuid GetPlayerPluginGUID() const override;

	/**
	 * Get the names of platforms that the media player supports.
	 *
	 * The returned platforms names must match the ones returned by
	 * FPlatformProperties::IniPlatformName, i.e. "Windows", "Android", etc.
	 *
	 * @return Platform name collection.
	 * @see GetOptionalParameters, GetRequiredParameters, GetSupportedFileExtensions, GetSupportedUriSchemes
	 */
	virtual const TArray<FString>& GetSupportedPlatforms() const override;

	/**
	 * Check whether the media player supports the specified feature.
	 *
	 * @param Feature The feature to check.
	 * @return true if the feature is supported, false otherwise.
	 */
	virtual bool SupportsFeature(EMediaFeature Feature) const override;

private:

	/** List of supported media file types. */
	TArray<FString> SupportedFileExtensions;

	/** List of platforms that the media player support. */
	TArray<FString> SupportedPlatforms;

	/** List of supported URI schemes. */
	TArray<FString> SupportedUriSchemes;
	
};
