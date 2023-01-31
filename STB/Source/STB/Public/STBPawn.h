// Search for a Star 2023

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/DefaultPawn.h"
#include "STBPawn.generated.h"

UCLASS()
class STB_API ASTBPawn : public APawn
{
	GENERATED_BODY()

public:

	ASTBPawn();

	void MoveTo(FVector2D Coord, const FBoxSphereBounds &LevelBounds);
	void ShowBeamUp();
	void EndBeamUp();
	float GetAcceleratedLocation(double& InputDirection, float InputValue, double& CurrentAcceleration, float DeltaTime);
	void UpdateAnimation(float DeltaTime, FVector2D Input2D);

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Movement")
	float PlayerSpeed = 0.01f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement")
	float Acceleration = 1.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement")
	float Deceleration = 0.5f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement")
	float BrakeDeceleration = 1.f;

	UPROPERTY(EditAnywhere, Category="Movement")
	float MovementHeight = 100.f;

	UPROPERTY(VisibleAnywhere, Category="Movement")
	bool bMovementLocked = false;

	UPROPERTY(EditAnywhere, Category="Animation")
	float SpinningRate = 15.f;

	UPROPERTY(EditAnywhere, Category="Animation")
	float AccelerationTiltSpeed = 1.f;

	UPROPERTY(EditAnywhere, Category="Animation")
	float DecelerationTiltSpeed = 2.f;

	UPROPERTY(EditAnywhere, Category="Animation")
	float BrakeTiltSpeed = 3.f;
	
	UPROPERTY(EditAnywhere, Category="Animation")
	float TiltAngle = 40.f;
	
	UPROPERTY(EditAnywhere, Category="Gameplay")
	float BeamUpSpeed = 3.f;

	UPROPERTY(EditAnywhere, Category="Gameplay")
	bool bTargetAcquired = false;

protected:
	virtual void BeginPlay() override;
	
private:
	UPROPERTY(VisibleAnywhere)
	UStaticMeshComponent* RotatingComponent;
	
	UPROPERTY(VisibleAnywhere)
	UStaticMeshComponent* ShowingBeamComp;
};
