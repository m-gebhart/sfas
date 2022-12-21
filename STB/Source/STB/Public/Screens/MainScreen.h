// Search for a Star 2023

#pragma once

#include "CoreMinimal.h"
#include "Screen.h"
#include "MainScreen.generated.h"

/**
 * Class to control the showing a logo
 */
UCLASS()
class STB_API UMainScreen : public UScreen
{
	GENERATED_BODY()

public:

	virtual void Select_Implementation() override;
	virtual void Alt1_Implementation() override;
	virtual void Alt2_Implementation() override;
};
