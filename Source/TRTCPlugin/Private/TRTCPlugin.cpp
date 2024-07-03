// Copyright (c) 2022 Tencent. All rights reserved.

#include "TRTCPlugin.h"

#include "TRTCMediaPlayer.h"

#define LOCTEXT_NAMESPACE "FTRTCPluginModule"

void FTRTCPluginModule::StartupModule()
{
	// This code will execute after your module is loaded into memory; the exact timing is specified in the .uplugin file
	// per-module FString BaseDir = IPluginManager::Get().FindPlugin("TRTCPlugin")->GetBaseDir();

	//todo liuqi 看是否可以进一步的初始化TRTC
	
}

void FTRTCPluginModule::ShutdownModule()
{
	// This function may be called during shutdown to clean up your module.  For modules that support dynamic reloading,
	// we call this function before unloading the module.
}

TSharedPtr<IMediaPlayer, ESPMode::ThreadSafe> FTRTCPluginModule::CreatePlayer(IMediaEventSink& EventSink)
{
	return MakeShareable(new FTRTCMediaPlayer(EventSink));
}

#undef LOCTEXT_NAMESPACE

IMPLEMENT_MODULE(FTRTCPluginModule, TRTCPlugin)
