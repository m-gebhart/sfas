// Search for a Star 2023

#include "Screens/SettingsScreen.h"

#include "Components/TextBlock.h"
#include "STB/STBGameModes.h"

void USettingsScreen::NativeConstruct()
{
	Super::NativeConstruct();

	for(int Count = 0; Count < Texts.Num(); ++Count)
	{
		if(Texts[Count]->Text.ToString().Contains("Volume"))
		{
			VolumeFormat = Texts[Count]->Text;
			VolumeTextIndex = Count;
			break;
		}
	}
	
	CurrentVolume = 1.0f;
	UpdatedChangedVolume();
}

void USettingsScreen::Back_Implementation()
{
	if(IsValid(PlayerController))
	{
		PlayerController->ShowUI(ESTBGameMode::MainMenu);
	}	
}

void USettingsScreen::Alt1_Implementation()
{
	CurrentVolume = FMath::Clamp(CurrentVolume + 0.1f, 0.0f, 1.0f);
	UpdatedChangedVolume();
}

void USettingsScreen::Alt2_Implementation()
{
	CurrentVolume = FMath::Clamp(CurrentVolume - 0.1f, 0.0f, 1.0f);
	UpdatedChangedVolume();
}

void USettingsScreen::UpdatedChangedVolume()
{
	if(VolumeTextIndex >= 0 && VolumeTextIndex < Texts.Num())
	{
		Texts[VolumeTextIndex]->SetText(FText::Format(VolumeFormat, CurrentVolume));
	}
	
	USoundClass * Class = GetDefault<UAudioSettings>()->GetDefaultSoundClass();
	Class->Properties.Volume = CurrentVolume; 
}
