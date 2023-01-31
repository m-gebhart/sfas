// Search for a Star 2023


#include "STBPawn.h"

#include "Kismet/GameplayStatics.h"

ASTBPawn::ASTBPawn()
{
	PrimaryActorTick.bCanEverTick = true;
}

void ASTBPawn::BeginPlay()
{
	UActorComponent* SaucerMeshComp = GetComponentsByTag(UStaticMeshComponent::StaticClass(), "Saucer")[0];
	if (SaucerMeshComp)
	{
		RotatingComponent = Cast<UStaticMeshComponent>(SaucerMeshComp);
	}

	UActorComponent* BeamMeshComp = GetComponentsByTag(UStaticMeshComponent::StaticClass(), "Showing")[0];
	if (BeamMeshComp)
	{
		ShowingBeamComp = Cast<UStaticMeshComponent>(BeamMeshComp);
		ShowingBeamComp->SetVisibility(false);
	}
}

void ASTBPawn::MoveTo(FVector2D Coord, const FBoxSphereBounds &LevelBounds)
{
	if (!bMovementLocked)
	{
		const float xLoc = -LevelBounds.BoxExtent.X + Coord.X*LevelBounds.BoxExtent.X*2;
		const float yLoc = -LevelBounds.BoxExtent.Y + Coord.Y*LevelBounds.BoxExtent.Y*2;

		const FVector Location(yLoc, xLoc, MovementHeight);
	
		SetActorLocation(Location);
	}
}

void ASTBPawn::UpdateAnimation(float DeltaTime)
{
	if (RotatingComponent)
	{
		RotatingComponent->AddRelativeRotation(FRotator(0., SpinningRate*DeltaTime, 0.f));
	}
}


//Called by PlayerController to calculate new position (PC's Tick() will call MoveTo() and will refer to that new pos)
float ASTBPawn::GetAcceleratedLocation(double& InputDirection, float InputValue, double& CurrentAcceleration, float DeltaTime)
{
	//if movement button is pressed
	if((InputValue > 0.1f || InputValue < -0.1f))
	{
		InputDirection = FMath::Sign(InputValue);
		if (InputDirection == -1*FMath::Sign(CurrentAcceleration))
		{
			//if direction change while player still has momentum
			CurrentAcceleration += BrakeDeceleration*InputDirection*DeltaTime;
		}
		else
		{
			//keep adding acceleration if button is still pressed
			CurrentAcceleration += Acceleration*InputValue*DeltaTime;
		}
	}
	else if (FMath::Sign(CurrentAcceleration) == InputDirection)
	{
		//if no button is pressed, decelerate based on momentum
		CurrentAcceleration -= Deceleration*InputDirection*DeltaTime;
	}

	return CurrentAcceleration*PlayerSpeed;
}


void ASTBPawn::ShowBeamUp()
{
	if (ShowingBeamComp)
	{
		bMovementLocked = true;
		UMaterialInstanceDynamic* DynamicBeamMaterial = UMaterialInstanceDynamic::Create(ShowingBeamComp->GetMaterial(0), this);
		ShowingBeamComp->SetVisibility(true);
		if (IsValid(DynamicBeamMaterial))
			DynamicBeamMaterial->SetScalarParameterValue(FName(TEXT("EmissionLevel")),0.f);
	}
}

void ASTBPawn::EndBeamUp()
{
	if (ShowingBeamComp)
	{
		ShowingBeamComp->SetVisibility(false);
	}
}

