// � 2021 Mustafa Moiz. All rights reserved.

using UnrealBuildTool;

public class Cursory : ModuleRules
{
	public Cursory(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

		PublicDependencyModuleNames.AddRange(new string[] 
		{ 
			"Core", 
			"CoreUObject", 
			"Engine", 
			"InputCore"
		});

		PrivateDependencyModuleNames.AddRange(new string[] 
		{ 
			"DeveloperSettings",
			"Slate", 
			"SlateCore", 
			"GameplayTags",
			"ImageWrapper",
			"ApplicationCore",
			"UMG"
		});

		if (Target.Type == TargetType.Editor)
		{
			PrivateDependencyModuleNames.AddRange(new string[]
			{
				"UnrealEd"
			});
		}
	}
}
