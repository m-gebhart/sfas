// Search for a Star 2023

#pragma once

#include "CoreMinimal.h"
#include "ProgressionData.h"
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

	UPROPERTY(EditAnywhere, Category="Movement")
	float MovementHeight = 100.f;

	UPROPERTY(EditAnywhere, Category="Movement")
	float BeamUpSpeed = 3.f;

	UPROPERTY(EditAnywhere, Category="Movement")
	bool bTargetAcquired = false;

	UPROPERTY(VisibleAnywhere, Category="Movement")
	bool bMovementLocked = false;

protected:
	virtual void BeginPlay() override;

private:
	
	UPROPERTY(VisibleAnywhere)
	UStaticMeshComponent* ShowingBeamComp;
};
