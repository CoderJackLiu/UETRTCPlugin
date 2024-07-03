// Copyright (c) 2022 Tencent. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "Modules/ModuleManager.h"



class IMediaEventSink;
class IMediaPlayer;


/**
 * Interface for the VlcMedia module.
 */
class ITRTCMediaModule
	: public IModuleInterface
{
public:

	/**
	 * Create a VideoLAN based media player.
	 *
	 * @param EventSink The object that receives media events from the player.
	 * @return A new media player, or nullptr if a player couldn't be created.
	 */
	virtual TSharedPtr<IMediaPlayer, ESPMode::ThreadSafe> CreatePlayer(IMediaEventSink& EventSink) = 0;

public:

	/** Virtual destructor. */
	virtual ~ITRTCMediaModule() { }



};

class FTRTCPluginModule : public ITRTCMediaModule
{
public:
	/** IModuleInterface implementation */
	virtual void StartupModule() override;
	virtual void ShutdownModule() override;

	// ITRTCMediaModule implementation
	virtual TSharedPtr<IMediaPlayer, ESPMode::ThreadSafe> CreatePlayer(IMediaEventSink& EventSink) override;

private:
	/** Whether the module has been initialized. */
	bool Initialized = false;
};
