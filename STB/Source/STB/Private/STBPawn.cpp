// Search for a Star 2023


#include "STBPawn.h"

#include "Kismet/GameplayStatics.h"
#include "Particles/Collision/ParticleModuleCollisionGPU.h"

ASTBPawn::ASTBPawn()
{
	PrimaryActorTick.bCanEverTick = true;
}

void ASTBPawn::BeginPlay()
{
	UActorComponent* SaucerMeshComp = GetComponentsByTag(UStaticMeshComponent::StaticClass(), "Saucer")[0];
	if (IsValid(SaucerMeshComp))
	{
		RotatingComponent = Cast<UStaticMeshComponent>(SaucerMeshComp);
	}

	UActorComponent* BeamMeshComp = GetComponentsByTag(UStaticMeshComponent::StaticClass(), "Showing")[0];
	if (IsValid(BeamMeshComp))
	{
		ShowingBeamComp = Cast<UStaticMeshComponent>(BeamMeshComp);
		ShowingBeamComp->SetVisibility(false);
		DynamicBeamMaterial = ShowingBeamComp->CreateDynamicMaterialInstance(0); // UMaterialInstanceDynamic::Create(ShowingBeamComp->GetMaterial(0), 0);
		BeamCompOffset = GetActorLocation().Z - ShowingBeamComp->GetComponentLocation().Z;
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

void ASTBPawn::UpdateAnimation(float DeltaTime, FVector2D Input2D, FVector2D Acceleration2D)
{
	/*Constant spinning of saucer*/
	if (IsValid(RotatingComponent))
	{
		RotatingComponent->AddRelativeRotation(FRotator(0., SpinningRate*DeltaTime, 0.f));
	}

	if (!CanMove())
		return;
	
	/*TILT ANIMATION WHEN MOVING*/
	/*Default: as if no buttons are pressed*/
	float RollTiltSpeed = DecelerationTiltSpeed;
	float RollInterpTarget = 0;

	float PitchTiltSpeed = DecelerationTiltSpeed;
	float PitchInterpTarget = 0;

	if(GetBouncing())
	{
		if (GetBouncingDirection() == EBounceBehaviour::Horizontal)
		{
			RollTiltSpeed = BounceTiltSpeed;
			RollInterpTarget = FMath::Sign(Acceleration2D.X)*(-1)*TiltAngle;
		}
		else
		{
			PitchTiltSpeed = BounceTiltSpeed;
			PitchInterpTarget = FMath::Sign(Acceleration2D.Y)*TiltAngle;
		}
	}
	else if(!bMovementLocked)
	{
		/*If buttons for horizontal movement are pressed*/
		if (Input2D.X > 0.1f || Input2D.X < -0.1f)
		{
			RollInterpTarget = FMath::Sign(Input2D.X)*TiltAngle*(-1);

			/*if changing direction while still having momentum = brake*/
			if (FMath::Sign(Acceleration2D.X) != FMath::Sign(Input2D.X))
			{
				RollTiltSpeed = BrakeDeceleration;
			}
			else
			{
				/*normal horizontal acceleration*/
				RollTiltSpeed = AccelerationTiltSpeed;
			}
		}

		/*If buttons for vertical movement are pressed*/
		if(Input2D.Y > 0.1f || Input2D.Y < -0.1f)
		{
			PitchInterpTarget = FMath::Sign(Input2D.Y)*TiltAngle;

			/*if changing direction while still having momentum = brake*/
			if (FMath::Sign(Acceleration2D.Y) != FMath::Sign(Input2D.Y))
			{
				PitchTiltSpeed = BrakeDeceleration;
			}
			else
			{
				/*normal horizontal acceleration*/
				PitchTiltSpeed = AccelerationTiltSpeed;
			}
		}
	}

	/*Translating rotation*/
	SetActorRotation(FRotator(
		FMath::FInterpTo(GetActorRotation().Pitch, PitchInterpTarget, DeltaTime, PitchTiltSpeed),
		GetActorRotation().Yaw,
		FMath::FInterpTo(GetActorRotation().Roll, RollInterpTarget, DeltaTime, RollTiltSpeed)));
}

//Called by PlayerController to calculate new position (PC's Tick() will call MoveTo() and will refer to that new pos)
float ASTBPawn::GetAcceleratedLocationOnAxis(double& InputDirection, float InputValue, double& CurrentAcceleration, float DeltaTime)
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
		InputDirection = 0;
		//if no button is pressed, decelerate based on momentum
		CurrentAcceleration -= Deceleration*InputDirection*DeltaTime;
	}

	return CurrentAcceleration*PlayerSpeed;
}

void ASTBPawn::ShowBeamUp()
{
	if (IsValid(ShowingBeamComp))
	{
		bMovementLocked = true;
		ShowingBeamComp->SetWorldRotation(FRotator::ZeroRotator);
		ShowingBeamComp->SetWorldLocation(GetActorLocation() - FVector(0, 0, BeamCompOffset));
		ShowingBeamComp->SetVisibility(true);
		if (IsValid(DynamicBeamMaterial))
		{
			DynamicBeamMaterial->SetVectorParameterValue(("Color"), FColor::Red);
		}
	}
}

void ASTBPawn::EndBeamUp()
{
	if (IsValid(ShowingBeamComp))
	{
		ShowingBeamComp->SetVisibility(false);
	}
}

void ASTBPawn::SetBouncingInDirection(EBounceBehaviour BounceDirection)
{
	BounceBehaviour = BounceDirection;
}

bool ASTBPawn::GetBouncing() const
{
	return BounceBehaviour != EBounceBehaviour::None;
}

EBounceBehaviour ASTBPawn::GetBouncingDirection() const
{
	return BounceBehaviour;
}

void ASTBPawn::LockMovement(bool bMove)
{
	bMovementLocked = bMove;
}

bool ASTBPawn::CanMove() const
{
	return !bMovementLocked;
}


