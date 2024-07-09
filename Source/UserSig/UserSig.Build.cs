//  Copyright (c) 2022 Tencent All rights reserved.

using System.IO;
using UnrealBuildTool;


public class UserSig : ModuleRules
{
	public UserSig(ReadOnlyTargetRules Target) : base(Target)
	{
		Type = ModuleType.External;

		PublicIncludePaths.Add(Path.Combine(ModuleDirectory, "include"));

		if (Target.Platform == UnrealTargetPlatform.Win64)
		{
			PublicAdditionalLibraries.Add(Path.Combine(ModuleDirectory, "Win64", "TXLiteAVTestUserSig.lib"));
			PublicDelayLoadDLLs.Add(Path.Combine(ModuleDirectory, "Win64", "TXLiteAVTestUserSig.dll"));
			RuntimeDependencies.Add("$(BinaryOutputDir)/TXLiteAVTestUserSig.dll", Path.Combine(ModuleDirectory, "Win64", "TXLiteAVTestUserSig.dll"));
		}
	}
}
