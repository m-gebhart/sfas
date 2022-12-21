// Search for a Star 2023

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "STBGameModeBase.generated.h"

enum class ESTBGameMode : uint8;

/**
 * STB Game Mode class used to control the current state of the game
 */
UCLASS()
class STB_API ASTBGameModeBase : public AGameModeBase
{
	GENERATED_BODY()
	
public:

	ASTBGameModeBase();

	virtual void BeginPlay() override;

private:

	void SetupUI() const;

};
