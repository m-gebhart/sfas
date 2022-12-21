// Search for a Star 2023

#pragma once

#include "CoreMinimal.h"
#include "Screen.h"
#include "SettingsScreen.generated.h"

/**
 * Class to control the settings
 */
UCLASS()
class STB_API USettingsScreen : public UScreen
{
	GENERATED_BODY()

public:

	virtual void NativeConstruct() override;

	virtual void Back_Implementation() override;
	virtual void Alt1_Implementation() override;
	virtual void Alt2_Implementation() override;

private:

	void UpdatedChangedVolume();

	UPROPERTY(VisibleAnywhere)
	FText VolumeFormat;
	
	UPROPERTY(VisibleAnywhere)
	float CurrentVolume;

	UPROPERTY(VisibleAnywhere)
	int VolumeTextIndex;

};
