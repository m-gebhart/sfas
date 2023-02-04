// Search for a Star 2023

#pragma once

#include "CoreMinimal.h"
#include "STBPawn.h"
#include "GameFramework/PlayerController.h"
#include "STBPlayerController.generated.h"

class UGameplay;
enum class ESTBGameMode : uint8;
class UScreen;

/**
 * 
 */
UCLASS()
class STB_API ASTBPlayerController : public APlayerController
{
	GENERATED_BODY()

public:
	
	ASTBPlayerController();

	virtual void PreProcessInput(const float DeltaTime, const bool bGamePaused) override;
	virtual void UpdateCameraManager(float DeltaSeconds) override;
	virtual void UpdateRotation(float DeltaTime) override;
	virtual void Tick(float DeltaSeconds) override;

	UFUNCTION(BlueprintCallable)
	void CreateUI();

	UFUNCTION(BlueprintCallable)
	void BeginNewGame();

	UFUNCTION(BlueprintCallable)
	void ContinueGame();	

	UFUNCTION(BlueprintCallable)
	void ShowUI(ESTBGameMode State);

	UFUNCTION(BlueprintCallable)
	const UGameplay * GetGameplay() const;

	UFUNCTION(BlueprintCallable)
	const FVector2D& GetCurrentPlayerLocation() const;

	UFUNCTION(BlueprintCallable)
	bool HorizontalBorderHitByPlayer() const;

	UFUNCTION(BlueprintCallable)
	bool VerticalBorderHitByPlayer() const;
	
	UFUNCTION(BlueprintCallable)
	FVector2D GetCurrentBallLocation() const;

	UFUNCTION(BlueprintCallable)
	bool TryMove();

	UFUNCTION(BlueprintCallable)
	bool bIsTargetCaptured();

	UFUNCTION(BlueprintCallable)
	ASTBPawn* GetPlayerPawn() const;

	UFUNCTION(BlueprintCallable)
	FVector2D GetCurrentAcceleration() const;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Camera")
	FVector OrbitPivot = FVector(0.0f, -50.0f, 125.0f);

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Camera")
	float OrbitRadius = 250.0f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Camera")
	FRotator OrbitRotationOffset;
	
protected:

	virtual void BeginPlay() override;	
	virtual void SetupInputComponent() override;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "UI")
	TSubclassOf<UScreen> IntroClass;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "UI")
	TSubclassOf<UScreen> MenuClass;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "UI")
	TSubclassOf<UScreen> SettingsClass;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "UI")
	TSubclassOf<UScreen> PlayingClass;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "UI")
	TSubclassOf<UScreen> GameOverClass;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "UI")
	TSubclassOf<UScreen> CreditsClass;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "UI")
	TSubclassOf<UScreen> GameCompleteClass;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "UI")
	TSubclassOf<UScreen> OutroClass;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Camera")
	float MinOrbitRadius = 60.0f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Camera")
	float MaxOrbitRadius = 1000.0f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Camera")
	float MinPitch = -30.0f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Camera")
	float MaxPitch = 30.0f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Camera")
	float AxisScale = 100.0f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Camera")
	float RadiusScale = 10.0f;
	
private:

	void SetupScreen(ESTBGameMode State, TSubclassOf<UScreen> Class, FName Name);

	UFUNCTION()
	void LeftRight(float Value);

	UFUNCTION()
	void UpDown(float Value);
	
	UFUNCTION()
	void TopButtonPress();

	UFUNCTION()
	void LeftButtonPress();

	UFUNCTION()
	void RightButtonPress();

	UFUNCTION()
	void BottomButtonPress();

	UFUNCTION()
	void SpecialButtonPress();

	UFUNCTION()
	void ResetPlayer();
	
	UPROPERTY(VisibleAnywhere)
	UGameplay * Gameplay;

	UPROPERTY(VisibleAnywhere)
	ASTBPawn* PlayerPawn;
	
	UPROPERTY(VisibleAnywhere, Category = "UI")
	TArray<UScreen*> Widgets;

	UPROPERTY(VisibleAnywhere)
	FVector2D CurrentPlayerLocation;	
	
    UPROPERTY(VisibleAnywhere)
    FVector2D CurrentAcceleration;

	UPROPERTY(VisibleAnywhere)
	FVector2D MoveDirection;

	UPROPERTY(VisibleAnywhere)
	float AccelerationValue;

	UPROPERTY(VisibleAnywhere)
	float LatestPlayerBounceTimeStamp;
	
	FVector LastOrbitPawnLocation;
	FRotator LastOrbitPawnViewRotation;
	ESTBGameMode CurrentState;

	static const FString TopButtonActionName;
	static const FString LeftButtonActionName;
	static const FString RightButtonActionName;
	static const FString BottomButtonActionName;
	static const FString SpecialButtonActionName;
	static const FString LeftStickXAxisName;
	static const FString LeftStickYAxisName;
	static const FString RightStickXAxisName;
	static const FString RightStickYAxisName;
	static const FString LeftTriggerAxisName;
	static const FString RightTriggerAxisName;
};
