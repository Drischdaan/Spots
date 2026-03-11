// Copyright Drischdaan All Rights Reserved Copyright

using UnrealBuildTool;

public class SpotsExample : ModuleRules
{
	public SpotsExample(ReadOnlyTargetRules target)
		: base(target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

		PublicDependencyModuleNames.AddRange(new string[]
		{
			"Core",
			"CoreUObject",
			"Engine",
			"InputCore",
			"EnhancedInput",
		});
	}
}