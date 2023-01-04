#include "Screens/CreditsScreen.h"
#include "STB/STBGameModes.h"

void UCreditsScreen::Back_Implementation()
{
	if(IsValid(PlayerController))
	{
		PlayerController->ShowUI(ESTBGameMode::MainMenu);
	}	
}