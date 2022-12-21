// Search for a Star 2023

#pragma once

#include "CoreMinimal.h"
#include "Screen.h"
#include "OutroScreen.generated.h"

/**
 * Class to control the showing a logo
 */
UCLASS()
class STB_API UOutroScreen : public UScreen
{
	GENERATED_BODY()

public:

	virtual void OnShown_Implementation() override;
	
};
