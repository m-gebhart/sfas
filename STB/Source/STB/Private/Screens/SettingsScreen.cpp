// Search for a Star 2023

#include "Screens/SettingsScreen.h"

#include "Gameplay.h"
#include "STBPlayerCameraManager.h"
#include "Components/TextBlock.h"
#include "Kismet/GameplayStatics.h"
#include "Screens/PlayingScreen.h"
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
		}
		else if (Texts[Count]->Text.ToString().Contains("Minimap"))
		{
			MinimapSizeFormat = Texts[Count]->Text.ToString();
			MinimapSizeTextIndex = Count;
		}
	}

	CurrentVolume = 1.0f;
	CurrentMinimapSize = EMinimapSize::Medium;
}

void USettingsScreen::Show(bool bShow)
{
	Super::Show(bShow);

	if (bShow)
	{
		UpdateUI();
	}
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
	UpdateUI();
}

void USettingsScreen::Alt2_Implementation()
{
	CurrentVolume = FMath::Clamp(CurrentVolume - 0.1f, 0.0f, 1.0f);
	UpdateUI();
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

void USettingsScreen::Special1_Implementation()
{
	//Decrease Value of Minimap Size
	const uint8 CurrentSizeIndex = FMath::Clamp(static_cast<uint8>(CurrentMinimapSize) -1 , 0, 2);
	switch (CurrentSizeIndex)
	{
		default:
		case 0:
			CurrentMinimapSize = EMinimapSize::Small; break;
		case 1:
			CurrentMinimapSize = EMinimapSize::Medium; break;
		case 2:
			CurrentMinimapSize = EMinimapSize::Big; break;
	}
	
	UpdateUI();
}

void USettingsScreen::Special2_Implementation()
{
	//Increase Value of Minimap Size
	const uint8 CurrentSizeIndex = FMath::Clamp(static_cast<uint8>(CurrentMinimapSize) +1 , 0, 2);
	switch (CurrentSizeIndex)
	{
	default:
	case 0:
		CurrentMinimapSize = EMinimapSize::Small; break;
	case 1:
		CurrentMinimapSize = EMinimapSize::Medium; break;
	case 2:
		CurrentMinimapSize = EMinimapSize::Big; break;
	}
	
	UpdateUI();
}


void USettingsScreen::UpdatedChangedMinimapSize()
{
	if(MinimapSizeTextIndex >= 0 && MinimapSizeTextIndex < Texts.Num())
	{
		FString SizeStr = StaticEnum<EMinimapSize>()->GetValueAsString(CurrentMinimapSize);
		SizeStr.Split("::", nullptr, &SizeStr);
		MinimapSizeFormat.Split("{", &MinimapSizeFormat, nullptr);

		Texts[MinimapSizeTextIndex]->SetText(FText::FromString(MinimapSizeFormat + SizeStr));
	}

	ASTBPlayerCameraManager* Class = Cast<ASTBPlayerCameraManager>(PlayerController->PlayerCameraManager);
	const float updatedScale = 0.2f*(static_cast<uint8>(CurrentMinimapSize)+1.f);
	if (IsValid(Class))
	{
		Class->MinimapScale = updatedScale;
	}
}

void USettingsScreen::UpdateUI()
{
	UpdatedChangedVolume();
	UpdatedChangedMinimapSize();
}
