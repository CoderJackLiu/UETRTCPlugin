// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class TRTCPlugin : ModuleRules
{
	public TRTCPlugin(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = ModuleRules.PCHUsageMode.UseExplicitOrSharedPCHs;

		PublicIncludePaths.AddRange(
			new string[]
			{
				// "Public",
				// ... add public include paths required here ...
			}
		);
		

		PrivateIncludePaths.AddRange(
			new string[]
			{
				// ... add other private include paths required here ...
			}
		);

		PublicDependencyModuleNames.AddRange(
			new string[]
			{
				"Core",
				"TRTCSDK",

				// Test Only
				"UserSig", 
				"Media", 
				"MediaUtils"
			}
		);

		PrivateDependencyModuleNames.AddRange(
			new string[]
			{
				"CoreUObject",
				"Engine",
				"Slate",
				"SlateCore",
				"MediaUtils",

			}
		);

		DynamicallyLoadedModuleNames.AddRange(
			new string[]
			{
			}
		);
		bEnableUndefinedIdentifierWarnings = false;
	}
}