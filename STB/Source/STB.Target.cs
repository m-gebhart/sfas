// Search for a Star 2023

using UnrealBuildTool;

public class STBTarget : TargetRules
{
	public STBTarget( TargetInfo Target) : base(Target)
	{
		Type = TargetType.Game;
		DefaultBuildSettings = BuildSettingsVersion.V2;
		ExtraModuleNames.AddRange( new string[] { "STB", "STBEditor" } );
	}
}
