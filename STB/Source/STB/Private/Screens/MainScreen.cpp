// Search for a Star 2023

#include "Screens/MainScreen.h"
#include "STB/STBGameModes.h"


void UMainScreen::Select_Implementation()
{
	if(IsValid(PlayerController))
	{
		PlayerController->BeginNewGame();
		PlayerController->ShowUI(ESTBGameMode::Playing);
	}
}

void UMainScreen::Alt1_Implementation()
{
	if(IsValid(PlayerController))
	{
		PlayerController->ShowUI(ESTBGameMode::Settings);
	}
}

void UMainScreen::Alt2_Implementation()
{
	if(IsValid(PlayerController))
	{
		PlayerController->ShowUI(ESTBGameMode::Outro);
	}
}
