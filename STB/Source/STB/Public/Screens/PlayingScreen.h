// Search for a Star 2023

#pragma once

#include "CoreMinimal.h"
#include "Screen.h"
#include "PlayingScreen.generated.h"


UENUM(BlueprintType)
enum class EPlayingState : uint8
{
	Guessing,
	Showing,
	Shown,

	NumStates
};

/**
 * Class to control the showing a logo
 */
UCLASS()
class STB_API UPlayingScreen : public UScreen
{
	GENERATED_BODY()

public:

	virtual void NativeConstruct() override;
	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;
	virtual void Select_Implementation() override;
	virtual void Special_Implementation() override;
	virtual void Show(bool bShow) override;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float MinimapPadding = 50.f;

private:

	void SetLevel(int Level);
	void SetLives(int Lives);
	void SetMinimap();
	void ShowLevel(bool bShow);
	void ShowLives(bool bShow);	
	void ShowPrompt(bool bShow);	
	void SetBallLocation();
	void DoReveal(const bool bLastGuessCorrect);
	void Reset();

	UFUNCTION()
	void StartCountdown();
	
	UFUNCTION()
	void UpdateCountdown();

	UFUNCTION()
	void EndCountdown();

	UPROPERTY(VisibleAnywhere)
	FText LevelTextFormat;

	UPROPERTY(VisibleAnywhere)
	FText LivesTextFormat;

	UPROPERTY(VisibleAnywhere)
	FText TimeTextFormat;

	UPROPERTY(VisibleAnywhere)
	int TargetImageIndex;

	UPROPERTY(VisibleAnywhere)
	int GuessImageIndex;	
	
	UPROPERTY(VisibleAnywhere)
	int LevelTextIndex;

	UPROPERTY(VisibleAnywhere)
	int LivesTextIndex;

	UPROPERTY(VisibleAnywhere)
	int TimeTextIndex;

	UPROPERTY(VisibleAnywhere)
	int PromptTextIndex;
	
	UPROPERTY(VisibleAnywhere)
	int MinimapImageIndex;

	UPROPERTY(VisibleAnywhere)
	EPlayingState PlayingState;

	UPROPERTY(VisibleAnywhere)
	FColor WinColor = FColor::Green;

	UPROPERTY(VisibleAnywhere)
	FColor LoseColor = FColor::Red;

	UPROPERTY(VisibleAnywhere)
	FVector2D MinimapSize;

	UPROPERTY(VisibleAnywhere)
	FTimerHandle CountdownTimerHandle;
};
