// Search for a Star 2023

#include "Screens/OutroScreen.h"

#include "Kismet/KismetSystemLibrary.h"

void UOutroScreen::OnShown_Implementation()
{
	Super::OnShown_Implementation();

	if(IsValid(PlayerController))
	{
		PlayerController->ConsoleCommand("quit");
	}
}
