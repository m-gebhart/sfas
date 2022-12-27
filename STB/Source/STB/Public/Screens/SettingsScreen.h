// Search for a Star 2023

#pragma once

#include "CoreMinimal.h"
#include "IntVectorTypes.h"
#include "Screen.h"
#include "SettingsScreen.generated.h"

UENUM(BlueprintType)
enum class EMinimapSize : uint8
{
	Small = 0	UMETA(DisplayName="Small"),
	Medium = 1	UMETA(DisplayName="Medium"),
	Big	= 2		UMETA(DisplayName="Big")
};

/**
 * Class to control the settings
 */
UCLASS()
class STB_API USettingsScreen : public UScreen
{
	GENERATED_BODY()

public:

	virtual void NativeConstruct() override;
	virtual void Show(bool bShow) override;

	virtual void Back_Implementation() override;
	virtual void Alt1_Implementation() override;
	virtual void Alt2_Implementation() override;
	virtual void Special1_Implementation() override;
	virtual void Special2_Implementation() override;

private:

	void UpdateUI();
	
	void UpdatedChangedVolume();
	void UpdatedChangedMinimapSize();

	UPROPERTY(VisibleAnywhere)
	FText VolumeFormat;
	
	UPROPERTY(VisibleAnywhere)
	float CurrentVolume;

	UPROPERTY(VisibleAnywhere)
	int VolumeTextIndex;

	UPROPERTY(VisibleAnywhere)
	FString MinimapSizeFormat;
	
	UPROPERTY(VisibleAnywhere)
	EMinimapSize CurrentMinimapSize;

	UPROPERTY(VisibleAnywhere)
	int MinimapSizeTextIndex;
};
