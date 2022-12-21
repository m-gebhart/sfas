// Search for a Star 2023

#include "Screens/IntroScreen.h"
#include "STB/STBGameModes.h"


void UIntroScreen::OnShown_Implementation()
{
	Super::OnShown_Implementation();

	if(IsValid(PlayerController))
	{
		PlayerController->ShowUI(ESTBGameMode::MainMenu);
	}
}
