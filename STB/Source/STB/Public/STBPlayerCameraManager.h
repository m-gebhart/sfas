// Search for a Star 2023

#pragma once

#include "CoreMinimal.h"
#include "Camera/PlayerCameraManager.h"
#include "STBPlayerCameraManager.generated.h"

/**
 * Camera manager that points the camera in a direction from a location
 */
UCLASS()
class STB_API ASTBPlayerCameraManager : public APlayerCameraManager
{
	GENERATED_BODY()

public:
	void SetLocationAndRotation(const FVector& InCameraLocation, const FRotator& InCameraRotation);
	virtual void UpdateViewTarget(FTViewTarget& OutVT, float DeltaTime) override;

	UPROPERTY(VisibleAnywhere)
	float MinimapScale = 0.25f;

private:

	UPROPERTY(VisibleAnywhere)
	FVector CameraLocation;

	UPROPERTY(VisibleAnywhere)
	FRotator CameraRotation;
};
