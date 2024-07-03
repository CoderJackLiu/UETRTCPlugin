using UnrealBuildTool;

public class TRTCMediaFactory : ModuleRules
{
	public TRTCMediaFactory(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = ModuleRules.PCHUsageMode.UseExplicitOrSharedPCHs;

		PrivateDependencyModuleNames.AddRange(
			new string[]
			{
				"MediaAssets",
			});

		PrivateIncludePathModuleNames.AddRange(
			new string[]
			{
				"Media",
				"TRTCPlugin",
			});

		PrivateIncludePaths.AddRange(
			new string[]
			{
				"TRTCMediaFactory/Private",
			});

		PublicDependencyModuleNames.AddRange(
			new string[]
			{
				"Core",
				"CoreUObject", "Media",
			});

		if (Target.Type == TargetRules.TargetType.Editor)
		{
			DynamicallyLoadedModuleNames.Add("Settings");
			PrivateIncludePathModuleNames.Add("Settings");
		}

		PublicDependencyModuleNames.AddRange(
			new string[]
			{
				"Core",
			}
		);

		PrivateDependencyModuleNames.AddRange(
			new string[]
			{
				"CoreUObject",
				"Engine",
				"Slate",
				"SlateCore"
			}
		);
	}
}