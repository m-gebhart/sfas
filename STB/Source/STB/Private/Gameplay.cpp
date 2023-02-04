#include "Gameplay.h"

#include "STBPawn.h"
#include "STBPlayerController.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"

UGameplay::UGameplay()
{
	CurrentLevel = 0;
	CurrentLives = 0;	
}

void UGameplay::SetLevels(AProgressionData* Data)
{
	Levels = Data;
}

void UGameplay::StartNewGame()
{
	CurrentLevel = 0;
	CurrentLives = StartingLives;
	bGameComplete = false;
}

void UGameplay::NextLevel()
{
	Levels->CleanupLevel();
	
	if(!bGameComplete && CurrentLevel < Levels->GetNumLevels())
	{
		const FProgressionLevelData* LevelData = Levels->SpawnLevel(CurrentLevel);

		if(LevelData != nullptr)
		{
			CurrentBallBounds = LevelData->BallBounds;
			CurrentRequiredDistance = LevelData->RequiredDistance;
			SetTime(LevelData->TimeLimit);
			ChooseRandomBallLocation();
		}			
	}
}

void UGameplay::ResetLevel()
{
	CurrentLives--;
}

const FBoxSphereBounds& UGameplay::GetCurrentBallBounds() const
{
	return CurrentBallBounds;
}

const FVector& UGameplay::GetBallLocation() const
{
	return BallLocation;
}

const FVector UGameplay::GetNormalizedBallLocation() const
{
	FVector NormalizedLocation;
	NormalizedLocation.X = UKismetMathLibrary::NormalizeToRange(GetBallLocation().X+ CurrentBallBounds.BoxExtent.X, 0, CurrentBallBounds.BoxExtent.X*2);
	NormalizedLocation.Y = UKismetMathLibrary::NormalizeToRange(GetBallLocation().Y+ CurrentBallBounds.BoxExtent.Y, 0, CurrentBallBounds.BoxExtent.Y*2);
	NormalizedLocation.Z = UKismetMathLibrary::NormalizeToRange(GetBallLocation().Z+ CurrentBallBounds.BoxExtent.Z, 0, CurrentBallBounds.BoxExtent.Z*2);
	return NormalizedLocation;
}


int UGameplay::GetLevel() const
{
	return CurrentLevel;
}

AProgressionData* UGameplay::GetLevelData() const
{
	return Levels;
}

int UGameplay::GetLives() const
{
	return CurrentLives;
}

bool UGameplay::GetWin() const
{
	return bWin;
}

bool UGameplay::GetGameComplete() const
{
	return bGameComplete;
}

float UGameplay::bIsTimeOver() const
{
	return CurrentTimeLeft < 0;
}


float UGameplay::GetTotalTimeLimit() const
{
	return CurrentTimeLimit;
}


float UGameplay::GetRemainingTime() const
{
	return CurrentTimeLeft;
}

void UGameplay::SetTime(float TimeLimit)
{
	CurrentTimeLimit = TimeLimit;
	CurrentTimeLeft = CurrentTimeLimit;
}

float UGameplay::SubtractFromRemainingTime(float deltaTime)
{
	CurrentTimeLeft -= deltaTime;
	return CurrentTimeLeft;
}

bool UGameplay::TryMove3D(const FVector& PlayerGuess3D, const FVector& BallLocation3D)
{
	const FVector2D PlayerLocation2D(PlayerGuess3D.X, PlayerGuess3D.Y);
	const FVector2D BallLocation2D(BallLocation3D.X, BallLocation3D.Y);
	return TryMove(PlayerLocation2D, BallLocation2D);
}

bool UGameplay::TryMove(const FVector2D& PlayerGuess, const FVector2D& BallLocation2D)
{
	bWin = false;
	const float Distance = FVector2D::Distance(BallLocation2D, PlayerGuess);
	
	if(Distance <= CurrentRequiredDistance)
	{
		if(CurrentLevel == Levels->GetNumLevels()-1)
			bGameComplete = true;
		
		CurrentLevel = FMath::Clamp(CurrentLevel + 1, 0, Levels->GetNumLevels()-1);
		bWin = true;
	}
	else
	{
		CurrentLives = FMath::Clamp(CurrentLives - 1, 0, 3);
	}

	return bWin;
}

void UGameplay::ChooseRandomBallLocation()
{
	BallLocation = CurrentBallBounds.Origin;
	BallLocation.X += FMath::RandRange( -CurrentBallBounds.BoxExtent.X, CurrentBallBounds.BoxExtent.X);
	BallLocation.Y += FMath::RandRange( -CurrentBallBounds.BoxExtent.Y, CurrentBallBounds.BoxExtent.Y);
	BallLocation.Z += CurrentBallBounds.BoxExtent.Z;
	
	Levels->GetTargetSightActor()->SetActorLocation(BallLocation);
}
