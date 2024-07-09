//  Copyright (c) 2022 Tencent All rights reserved.

using System.IO;
using UnrealBuildTool;


public class TRTCSDK : ModuleRules
{
	public TRTCSDK(ReadOnlyTargetRules Target) : base(Target)
	{
		Type = ModuleType.External;

		PublicIncludePaths.Add(Path.Combine(ModuleDirectory, "include"));

		if (Target.Platform == UnrealTargetPlatform.Win64)
		{
			PublicIncludePaths.Add(Path.Combine(ModuleDirectory, "Win64", "include"));

			
			PublicAdditionalLibraries.Add(Path.Combine(ModuleDirectory, "Win64", "liteav.lib"));

			PublicDelayLoadDLLs.Add(Path.Combine(ModuleDirectory, "Win64", "liteav.dll"));
			PublicDelayLoadDLLs.Add(Path.Combine(ModuleDirectory, "Win64", "txffmpeg.dll"));
			PublicDelayLoadDLLs.Add(Path.Combine(ModuleDirectory, "Win64", "txsoundtouch.dll"));
			// PublicDelayLoadDLLs.Add(Path.Combine(ModuleDirectory, "Win64", "liteav_screen.dll"));

			RuntimeDependencies.Add("$(BinaryOutputDir)/liteav.dll", Path.Combine(ModuleDirectory, "Win64", "liteav.dll"));
			RuntimeDependencies.Add("$(BinaryOutputDir)/txffmpeg.dll", Path.Combine(ModuleDirectory, "Win64", "txffmpeg.dll"));
			RuntimeDependencies.Add("$(BinaryOutputDir)/txsoundtouch.dll", Path.Combine(ModuleDirectory, "Win64", "txsoundtouch.dll"));
			// RuntimeDependencies.Add("$(BinaryOutputDir)/liteav_screen.dll", Path.Combine(ModuleDirectory, "Win64", "liteav_screen.dll"));
			
		}
		
	
	}
}
