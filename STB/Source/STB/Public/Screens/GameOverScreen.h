// Search for a Star 2023

#pragma once

#include "CoreMinimal.h"
#include "Screen.h"
#include "GameOverScreen.generated.h"

/**
 * Class to control the showing a logo
 */
UCLASS()
class STB_API UGameOverScreen : public UScreen
{
	GENERATED_BODY()

public:

	virtual void Select_Implementation() override;

};
