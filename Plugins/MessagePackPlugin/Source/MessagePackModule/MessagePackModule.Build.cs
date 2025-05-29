// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;
using System.IO; // Required for Path.Combine

public class MessagePackModule : ModuleRules
{
	public MessagePackModule(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = ModuleRules.PCHUsageMode.UseExplicitOrSharedPCHs;
		bEnableExceptions = true; // Enable C++ exceptions for msgpack-c

		// Set C++ standard to C++20
		CppStandard = CppStandardVersion.Cpp20;

		PublicDependencyModuleNames.AddRange(new string[] { 
			"Core", 
			"CoreUObject", 
			"Engine", 
			"InputCore", 
			"EnhancedInput", 
			"Json", 
			"JsonUtilities"
		});

		PrivateDependencyModuleNames.AddRange(new string[] { 
		});

		// Add the include path for the msgpack-c header-only library
		// PublicIncludePaths.Add("/Users/d/Projects/MessagePack/Source/MessagePack/ThirdParty/msgpack-c/include/msgpack"); // Absolute path for testing
		// PublicSystemIncludePaths.Add(Path.Combine(ModuleDirectory, "ThirdParty/msgpack-c/include/msgpack")); // Using PublicSystemIncludePaths for testing
		PublicIncludePaths.Add(Path.Combine(ModuleDirectory, "ThirdParty/msgpack-c/include"));

		// Tell msgpack-c to not use Boost
		PublicDefinitions.Add("MSGPACK_NO_BOOST=1");

		// Add /EHsc flag for MSVC compiler
		if (Target.Platform == UnrealTargetPlatform.Win64)
		{
			// this.PublicAdditionalCompilerArguments += " /EHsc"; // Still causing issues
			// PrivateDefinitions.Add("/EHsc"); // Incorrect attempt
            // No fix for PublicAdditionalCompilerArguments yet, leaving blank for now
		}
	}
}
