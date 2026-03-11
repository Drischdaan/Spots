// Copyright Drischdaan All Rights Reserved Copyright

using UnrealBuildTool;

public class SpotsEditor : ModuleRules
{
	public SpotsEditor(ReadOnlyTargetRules target)
		: base(target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

		PublicDependencyModuleNames.AddRange(new string[]
		{
			"Core",
			"Spots",
			"GameplayTags",
		});

		PrivateDependencyModuleNames.AddRange(new string[]
		{
			"AssetRegistry",
			"CoreUObject",
			"Engine",
			"Slate",
			"SlateCore",
			"InputCore",
			"EditorFramework",
			"EditorStyle",
			"UnrealEd",
			"LevelEditor",
			"InteractiveToolsFramework",
			"EditorInteractiveToolsFramework",
			"Navmesh",
			"NavigationSystem",
		});
	}
}