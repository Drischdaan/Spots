// Copyright Drischdaan All Rights Reserved Copyright

using UnrealBuildTool;

public class SpotsExampleEditorTarget : TargetRules
{
	public SpotsExampleEditorTarget(TargetInfo target)
		: base(target)
	{
		Type = TargetType.Editor;
		DefaultBuildSettings = BuildSettingsVersion.V6;
		IncludeOrderVersion = EngineIncludeOrderVersion.Unreal5_7;
		ExtraModuleNames.Add("SpotsExample");
	}
}