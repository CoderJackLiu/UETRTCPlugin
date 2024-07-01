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

			RuntimeDependencies.Add("$(BinaryOutputDir)/liteav.dll", Path.Combine(ModuleDirectory, "Win64", "liteav.dll"));
			RuntimeDependencies.Add("$(BinaryOutputDir)/txffmpeg.dll", Path.Combine(ModuleDirectory, "Win64", "txffmpeg.dll"));
			RuntimeDependencies.Add("$(BinaryOutputDir)/txsoundtouch.dll", Path.Combine(ModuleDirectory, "Win64", "txsoundtouch.dll"));
			
		}
		else if (Target.Platform == UnrealTargetPlatform.Mac)
		{
			PublicFrameworks.Add(Path.Combine(ModuleDirectory, "Mac", "TXLiteAVSDK_TRTC_Mac.framework"));
			PublicFrameworks.Add(Path.Combine(ModuleDirectory, "Mac", "TXFFmpeg.framework"));
			PublicFrameworks.Add(Path.Combine(ModuleDirectory, "Mac", "TXSoundTouch.framework"));
		}
		else if (Target.Platform == UnrealTargetPlatform.IOS)
		{
			PublicAdditionalFrameworks.Add(new Framework("TXLiteAVSDK_TRTC", Path.Combine(ModuleDirectory, "iOS", "TXLiteAVSDK_TRTC.framework.zip"), "", true));
			PublicAdditionalFrameworks.Add(new Framework("TXFFmpeg", Path.Combine(ModuleDirectory, "iOS", "TXFFmpeg.framework.zip"), "", true));
			PublicAdditionalFrameworks.Add(new Framework("TXSoundTouch", Path.Combine(ModuleDirectory, "iOS", "TXSoundTouch.framework.zip"), "", true));

			PublicFrameworks.AddRange(
						new string[] {
							"CoreServices",
							"VideoToolbox",
							"AudioToolbox",
							"Accelerate",
							"CFNetwork",
							"OpenGLES",
							"MetalKit",
							"CoreTelephony"
				}
			);
		}
		else if (Target.Platform == UnrealTargetPlatform.Android)
		{
			AdditionalPropertiesForReceipt.Add("AndroidPlugin", Path.Combine(ModuleDirectory, "Android", "UPL_TRTCSDK.xml"));

			// string Architecture = "armeabi-v7a";
			string Architecture = "arm64-v8a";

			PublicAdditionalLibraries.Add(Path.Combine(ModuleDirectory, "Android", Architecture, "libliteavsdk.so"));
			PublicAdditionalLibraries.Add(Path.Combine(ModuleDirectory, "Android", Architecture, "libtxsoundtouch.so"));
			PublicAdditionalLibraries.Add(Path.Combine(ModuleDirectory, "Android", Architecture, "libtxffmpeg.so"));
		}
	}
}
