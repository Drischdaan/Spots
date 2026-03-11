// Copyright Drischdaan All Rights Reserved Copyright

using UnrealBuildTool;

public class Spots : ModuleRules
{
	public Spots(ReadOnlyTargetRules target)
		: base(target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

		PublicDependencyModuleNames.AddRange(new string[]
		{
			"Core",
		});

		PrivateDependencyModuleNames.AddRange(new string[]
		{
			"CoreUObject",
			"Engine",
			"Slate",
			"SlateCore",
		});
	}
}