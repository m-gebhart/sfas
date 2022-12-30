// Search for a Star 2023


#include "STBPawn.h"

ASTBPawn::ASTBPawn()
{
	PrimaryActorTick.bCanEverTick = true;
}


void ASTBPawn::MoveTo(FVector2D Coord, const FBoxSphereBounds &LevelBounds)
{
	const float xLoc = -LevelBounds.BoxExtent.X + Coord.X*LevelBounds.BoxExtent.X*2;
	const float yLoc = -LevelBounds.BoxExtent.Y + Coord.Y*LevelBounds.BoxExtent.Y*2;

	const FVector Location(yLoc, xLoc, MovementHeight);
	
	SetActorLocation(Location);
}