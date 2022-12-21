// Search for a Star 2023

#include "Screens/GameOverScreen.h"
#include "STB/STBGameModes.h"


void UGameOverScreen::Select_Implementation()
{
	if(IsValid(PlayerController))
	{
		PlayerController->ShowUI(ESTBGameMode::MainMenu);
	}
}
