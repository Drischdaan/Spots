// Copyright Drischdaan All Rights Reserved Copyright

using UnrealBuildTool;

public class SpotsExampleTarget : TargetRules
{
	public SpotsExampleTarget(TargetInfo target)
		: base(target)
	{
		Type = TargetType.Game;
		DefaultBuildSettings = BuildSettingsVersion.V6;
		IncludeOrderVersion = EngineIncludeOrderVersion.Unreal5_7;
		ExtraModuleNames.Add("SpotsExample");
	}
}