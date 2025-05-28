// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;
using System.IO; // Required for Path.Combine

public class MessagePack : ModuleRules
{
	public MessagePack(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;
	
		PublicDependencyModuleNames.AddRange(new string[] { 
			"Core", 
			"CoreUObject", 
			"Engine", 
			"InputCore",
			"WebSockets",
			"MessagePackModule"
		});

		PrivateDependencyModuleNames.AddRange(new string[] { 
			"WebSockets"
		});

		// Attempt to explicitly add WebSockets include path - REVERTED
		//string EngineDir = Path.GetFullPath(Target.RelativeEnginePath);
        //PublicIncludePaths.Add(Path.Combine(EngineDir, "Source/Runtime/WebSockets/Public"));

		// Uncomment if you are using Slate UI
		// PrivateDependencyModuleNames.AddRange(new string[] { "Slate", "SlateCore" });
		
		// Uncomment if you are using online features
		// PrivateDependencyModuleNames.Add("OnlineSubsystem");

		// To include OnlineSubsystemSteam, add it to the plugins section in your uproject file with the Enabled attribute set to true
	}
} 