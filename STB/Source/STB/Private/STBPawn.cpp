// Search for a Star 2023


#include "STBPawn.h"

ASTBPawn::ASTBPawn()
{
	PrimaryActorTick.bCanEverTick = true;
}

void ASTBPawn::BeginPlay()
{
	ShowingBeamComp = Cast<UStaticMeshComponent>(GetComponentsByTag(UStaticMeshComponent::StaticClass(), "Showing")[0]);
	if (ShowingBeamComp)
		ShowingBeamComp->SetVisibility(false);
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

void ASTBPawn::ShowBeamUp()
{
	if (ShowingBeamComp)
	{
		bMovementLocked = true;
		ShowingBeamComp->SetVisibility(true);
	}
}


void ASTBPawn::EndBeamUp()
{
	if (ShowingBeamComp)
	{
		bMovementLocked = false;
		ShowingBeamComp->SetVisibility(false);
	}
}

