// Search for a Star 2023


#include "STBPlayerCameraManager.h"

#include "Camera/CameraActor.h"
#include "Camera/CameraComponent.h"

void ASTBPlayerCameraManager::SetLocationAndRotation(const FVector& InCameraLocation, const FRotator& InCameraRotation)
{
	CameraLocation = InCameraLocation;
	CameraRotation = InCameraRotation;
}

void ASTBPlayerCameraManager::UpdateViewTarget(FTViewTarget& OutVT, float DeltaTime)
{
	// Don't update outgoing viewtarget during an interpolation 
	if ((PendingViewTarget.Target != NULL) && BlendParams.bLockOutgoing && OutVT.Equal(ViewTarget))
	{
		return;
	}

	// store previous POV, in case we need it later
	FMinimalViewInfo OrigPOV = OutVT.POV;

	OutVT.POV.FOV = DefaultFOV;
	OutVT.POV.OrthoWidth = DefaultOrthoWidth;
	OutVT.POV.AspectRatio = DefaultAspectRatio;
	OutVT.POV.bConstrainAspectRatio = bDefaultConstrainAspectRatio;
	OutVT.POV.bUseFieldOfViewForLOD = true;
	OutVT.POV.ProjectionMode = bIsOrthographic ? ECameraProjectionMode::Orthographic : ECameraProjectionMode::Perspective;
	OutVT.POV.PostProcessSettings.SetBaseValues();
	OutVT.POV.PostProcessBlendWeight = 1.0f;
	OutVT.POV.Location = CameraLocation;
	OutVT.POV.Rotation = CameraRotation;

	if (bAlwaysApplyModifiers)
	{
		// Apply camera modifiers at the end (view shakes for example)
		ApplyCameraModifiers(DeltaTime, OutVT.POV);
	}

	// Synchronize the actor with the view target results
	SetActorLocationAndRotation(OutVT.POV.Location, OutVT.POV.Rotation, false);

	UpdateCameraLensEffects(OutVT);
}
