// Search for a Star 2023

using UnrealBuildTool;
using System.Collections.Generic;

public class STBEditorTarget : TargetRules
{
	public STBEditorTarget( TargetInfo Target) : base(Target)
	{
		Type = TargetType.Editor;
		DefaultBuildSettings = BuildSettingsVersion.V2;
		ExtraModuleNames.AddRange( new string[] { "STB", "STBEditor" } );
	}
}
