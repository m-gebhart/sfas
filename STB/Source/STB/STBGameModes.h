// Search for a Star 2023

#pragma once

UENUM(BlueprintType)
enum class ESTBGameMode : uint8
{
	Intro,
	MainMenu,
	Settings,
	Credits,
	Playing,
	GameOver,
	Outro,

	NumModes
};