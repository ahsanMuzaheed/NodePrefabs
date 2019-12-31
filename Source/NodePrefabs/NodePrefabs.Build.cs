// Copyright 2019 Guenther Schrenk (Rumbleball)

using UnrealBuildTool;
using System.IO;

public class NodePrefabs : ModuleRules
{
	public NodePrefabs(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = ModuleRules.PCHUsageMode.UseExplicitOrSharedPCHs;

  
        PublicIncludePaths.AddRange(
			new string[] {
				// ... add public include paths required here ...
                 Path.Combine(EngineDirectory, "Source/Editor"),
                 Path.Combine(EngineDirectory, "Source/Runtime/Slate/Public"),
            }
			);
				
		
		PrivateIncludePaths.AddRange(
			new string[] {
				// ... add other private include paths required here ...
			}
			);
			
		
		PublicDependencyModuleNames.AddRange(
			new string[]
			{
				"Core",
				// ... add other public dependencies that you statically link with here ...
			}
			);
			
		
		PrivateDependencyModuleNames.AddRange(
			new string[]
			{
				"CoreUObject",
				"Engine",
				"Slate",
				"SlateCore",
                "InputCore",
                "GraphEditor",
                "EditorStyle",
                "MainFrame",
                "ApplicationCore",
                "UnrealEd",
                "AssetTools",
                "AssetRegistry",
                "PropertyEditor",
				// ... add private dependencies that you statically link with here ...	
			}
			);
		
		
		DynamicallyLoadedModuleNames.AddRange(
			new string[]
			{
				// ... add any modules that your module loads dynamically here ...
			}
			);
	}
}
