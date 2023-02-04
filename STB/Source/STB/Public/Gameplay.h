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
	const FVector GetNormalizedBallLocation() const;
	
	UFUNCTION(BlueprintCallable)
	int GetLevel() const;

	UFUNCTION(BlueprintCallable)
	AProgressionData* GetLevelData() const;

	UFUNCTION(BlueprintCallable)
	int GetLives() const;

	UFUNCTION(BlueprintCallable)
	bool GetWin() const;

	UFUNCTION(BlueprintCallable)
	bool GetGameComplete() const;

	UFUNCTION(BlueprintCallable)
	bool TryMove3D(const FVector& PlayerGuess3D, const FVector& BallLocation3D);
	
	UFUNCTION(BlueprintCallable)
	bool TryMove(const FVector2D& PlayerGuess, const FVector2D& BallLocation2D);

	UFUNCTION(BlueprintCallable)
	void ChooseRandomBallLocation();

	UFUNCTION(BlueprintCallable)
	float GetTotalTimeLimit() const;
	
	UFUNCTION(BlueprintCallable)
	float GetRemainingTime() const;

	UFUNCTION(BlueprintCallable)
	float bIsTimeOver() const;

	UFUNCTION(BlueprintCallable)
	void SetTime(float TimeLimit);
	
	UFUNCTION(BlueprintCallable)
	float SubtractFromRemainingTime(float deltaTime);

protected:
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	int StartingLives = 1;
	
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
	float CurrentTimeLeft;

	UPROPERTY(VisibleAnywhere)
	float CurrentTimeLimit;
	
	UPROPERTY(VisibleAnywhere)
	bool bWin;

	UPROPERTY(VisibleAnywhere)
	bool bGameComplete;
};
