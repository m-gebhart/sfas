// Search for a Star 2023

#pragma once

#include "CoreMinimal.h"
#include "ProgressionData.h"
#include "Gameplay.generated.h"

UCLASS()
class STB_API UGameplay : public UObject
{
	GENERATED_BODY()
	
public:

	UGameplay();

	UFUNCTION(BlueprintCallable)
	void SetLevels(AProgressionData * Data);

	UFUNCTION(BlueprintCallable)
	void StartNewGame();

	UFUNCTION(BlueprintCallable)
	void NextLevel();	

	UFUNCTION(BlueprintCallable)
	void ResetLevel();
	
	UFUNCTION(BlueprintCallable)
	const FBoxSphereBounds& GetCurrentBallBounds() const;	

	UFUNCTION(BlueprintCallable)
	const FVector& GetBallLocation() const;
	
	UFUNCTION(BlueprintCallable)
	int GetLevel() const;

	UFUNCTION(BlueprintCallable)
	int GetLives() const;

	UFUNCTION(BlueprintCallable)
	bool GetWin() const;
	
	UFUNCTION(BlueprintCallable)
	bool TryMove(const FVector2D& PlayerGuess, const FVector2D& BallLocation2D);

	UFUNCTION(BlueprintCallable)
	void ChooseRandomBallLocation();
	
protected:
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	int StartingLives = 3;
	
private:


	UPROPERTY(VisibleAnywhere)
	AProgressionData * Levels;

	UPROPERTY(VisibleAnywhere)
	FBoxSphereBounds CurrentBallBounds; 

	UPROPERTY(VisibleAnywhere)
	FVector BallLocation;

	UPROPERTY(VisibleAnywhere)
	float CurrentRequiredDistance;
	
	UPROPERTY(VisibleAnywhere)
	int CurrentLevel;

	UPROPERTY(VisibleAnywhere)
	int CurrentLives;
	
	UPROPERTY(VisibleAnywhere)
	bool bWin;
	
};
