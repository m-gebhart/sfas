// Search for a Star 2023


#include "STBPlayerController.h"

#include "EngineUtils.h"
#include "ProgressionData.h"
#include "Gameplay.h"
#include "Screen.h"
#include "STBPawn.h"
#include "STBPlayerCameraManager.h"
#include "GameFramework/Character.h"
#include "GameFramework/PawnMovementComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"
#include "STB/STBGameModes.h"

const FString ASTBPlayerController::TopButtonActionName = TEXT("TopButton");
const FString ASTBPlayerController::LeftButtonActionName = TEXT("LeftButton");
const FString ASTBPlayerController::RightButtonActionName = TEXT("RightButton");
const FString ASTBPlayerController::BottomButtonActionName = TEXT("BottomButton");
const FString ASTBPlayerController::SpecialButtonActionName = TEXT("SpecialButton");
const FString ASTBPlayerController::LeftStickXAxisName = TEXT("LeftX");
const FString ASTBPlayerController::LeftStickYAxisName = TEXT("LeftY");
const FString ASTBPlayerController::RightStickXAxisName = TEXT("RightX");
const FString ASTBPlayerController::RightStickYAxisName = TEXT("RightY");
const FString ASTBPlayerController::LeftTriggerAxisName = TEXT("LeftTrigger");
const FString ASTBPlayerController::RightTriggerAxisName = TEXT("RightTrigger");

ASTBPlayerController::ASTBPlayerController()
{
	PlayerCameraManagerClass = ASTBPlayerCameraManager::StaticClass();
}

void ASTBPlayerController::PreProcessInput(const float DeltaTime, const bool bGamePaused)
{
	if (APawn* const CurrentPawn = GetPawnOrSpectator())
	{
		if (UPawnMovementComponent* MovementComponent = CurrentPawn->GetMovementComponent())
		{
			// Reset velocity before processing input when orbiting to limit acceleration which 
			// can cause overshooting and jittering as orbit attempts to maintain a fixed radius.
			MovementComponent->Velocity = FVector::ZeroVector;
			MovementComponent->UpdateComponentVelocity();
		}
	}	
}

void ASTBPlayerController::UpdateCameraManager(float DeltaSeconds)
{
	if (ASTBPlayerCameraManager* STBCameraManager = Cast<ASTBPlayerCameraManager>(PlayerCameraManager))
	{
		STBCameraManager->SetLocationAndRotation(LastOrbitPawnLocation, LastOrbitPawnViewRotation);
		STBCameraManager->UpdateCamera(DeltaSeconds);
	}
}

void ASTBPlayerController::UpdateRotation(float DeltaTime)
{
	if(APawn* const CurrentPawn = GetPawnOrSpectator())
	{
		FRotator Rotation = GetControlRotation();

		Rotation.Yaw += RotationInput.Yaw * AxisScale * DeltaTime;
		Rotation.Pitch += RotationInput.Pitch * AxisScale * DeltaTime;

		LastOrbitPawnLocation = OrbitPivot + Rotation.Vector() * OrbitRadius;
		LastOrbitPawnViewRotation = UKismetMathLibrary::FindLookAtRotation(PlayerCameraManager->GetCameraLocation(), Gameplay->GetCurrentBallBounds().Origin);
		//LastOrbitPawnViewRotation = (OrbitPivot - LastOrbitPawnLocation).GetSafeNormal().Rotation();
		//CurrentPawn->SetActorLocationAndRotation(LastOrbitPawnLocation, LastOrbitPawnViewRotation);
		//SetControlRotation(Rotation);
	}
}

void ASTBPlayerController::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	if(IsValid(Gameplay) && CurrentState == ESTBGameMode::Playing)
	{
		PlayerPawn->MoveTo(CurrentPlayerLocation, Gameplay->GetCurrentBallBounds());
		PlayerPawn->UpdateAnimation(DeltaSeconds, MoveDirection, CurrentAcceleration);
	}
}

void ASTBPlayerController::CreateUI()
{
	Widgets.Init(nullptr, static_cast<int>(ESTBGameMode::NumModes));
	SetupScreen(ESTBGameMode::Intro, IntroClass, TEXT("Intro"));
	SetupScreen(ESTBGameMode::MainMenu, MenuClass, TEXT("MainMenu"));
	SetupScreen(ESTBGameMode::Settings, SettingsClass, TEXT("Settings"));
	SetupScreen(ESTBGameMode::Credits, CreditsClass, TEXT("Credits"));
	SetupScreen(ESTBGameMode::Playing, PlayingClass, TEXT("Playing"));
	SetupScreen(ESTBGameMode::GameOver, GameOverClass, TEXT("GameOver"));
	SetupScreen(ESTBGameMode::Outro, OutroClass, TEXT("Outro"));
	ShowUI(ESTBGameMode::Intro);
}

void ASTBPlayerController::BeginNewGame()
{
	if(Gameplay != nullptr)
	{
		ResetPlayer();
		PlayerPawn = dynamic_cast<ASTBPawn*>(GetPawn());
		Gameplay->StartNewGame();
		Gameplay->NextLevel();
	}
}

void ASTBPlayerController::ContinueGame()
{
	if(Gameplay != nullptr)
	{
		ResetPlayer();

		if(Gameplay->GetWin())
		{
			Gameplay->NextLevel();
		}
		else if(Gameplay->GetLives() <= 0)
		{
			ShowUI(ESTBGameMode::GameOver);		
		}
		else
		{
			Gameplay->ChooseRandomBallLocation();
			Gameplay->SetTime(Gameplay->GetTotalTimeLimit());
		}
	}	
}

void ASTBPlayerController::ShowUI(ESTBGameMode State)
{
 	const int TargetModeIndex = static_cast<int>(State);

	for(int Count = 0; Count < static_cast<int>(ESTBGameMode::NumModes); ++Count)
	{
		const bool bShow = Count == TargetModeIndex;
		Widgets[Count]->Show(bShow);
		CurrentState = State;
	} 
}

const UGameplay* ASTBPlayerController::GetGameplay() const
{
	return Gameplay;
}

const FVector2D& ASTBPlayerController::GetCurrentPlayerLocation() const
{
	return CurrentPlayerLocation;
}

bool ASTBPlayerController::HorizontalBorderHitByPlayer() const
{
	return CurrentPlayerLocation.Y < 0.01f || CurrentPlayerLocation.Y > 0.99f;
}

bool ASTBPlayerController::VerticalBorderHitByPlayer() const
{
	return CurrentPlayerLocation.X < 0.01f || CurrentPlayerLocation.X > 0.99f;
}


FVector2D ASTBPlayerController::GetCurrentBallLocation() const
{
	//static const FVector2D ViewportSize = FVector2D(GEngine->GameViewport->Viewport->GetSizeXY());
	FVector2D ScreenLocation = FVector2D::ZeroVector;
	
	if(IsValid(Gameplay))
	{
		const FVector BallLocation = Gameplay->GetBallLocation();
		ProjectWorldLocationToScreen(BallLocation, ScreenLocation, true);
	}
	
	return ScreenLocation/* - (ViewportSize * 0.5f)*/;
}

bool ASTBPlayerController::TryMove()
{
	return Gameplay->TryMove3D(PlayerPawn->GetActorLocation(), Gameplay->GetBallLocation());
}

bool ASTBPlayerController::bIsTargetCaptured()
{
	if (GetPlayerPawn() && GetGameplay() && GetGameplay()->GetLevelData()->GetTargetSightActor())
	{
		return GetPlayerPawn()->MovementHeight*0.8f < GetGameplay()->GetLevelData()->GetTargetSightActor()->GetActorLocation().Z;
	}
	return false;
}

ASTBPawn* ASTBPlayerController::GetPlayerPawn() const
{
	return PlayerPawn;
}

FVector2D ASTBPlayerController::GetCurrentAcceleration() const
{
	return CurrentAcceleration;
}

void ASTBPlayerController::BeginPlay()
{
	Super::BeginPlay();

	Gameplay = NewObject<UGameplay>();
	
	if(IsValid(Gameplay))
	{
		const TActorIterator<AProgressionData> ProgressionIterator(GetWorld());
		if(ProgressionIterator)
		{
			Gameplay->SetLevels(*ProgressionIterator);
		}
	}	
}

void ASTBPlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();
	
	if(InputComponent)
	{
		InputComponent->BindAction(*TopButtonActionName, IE_Pressed, this, &ASTBPlayerController::TopButtonPress);
		InputComponent->BindAction(*LeftButtonActionName, IE_Pressed, this, &ASTBPlayerController::LeftButtonPress);
		InputComponent->BindAction(*RightButtonActionName, IE_Pressed, this, &ASTBPlayerController::RightButtonPress);
		InputComponent->BindAction(*BottomButtonActionName, IE_Pressed, this, &ASTBPlayerController::BottomButtonPress);
		InputComponent->BindAction(*SpecialButtonActionName, IE_Pressed, this, &ASTBPlayerController::SpecialButtonPress);

		InputComponent->BindAxis(*LeftStickXAxisName, this, &ASTBPlayerController::LeftRight);
		InputComponent->BindAxis(*LeftStickYAxisName, this, &ASTBPlayerController::UpDown);
	}
}

void ASTBPlayerController::SetupScreen(ESTBGameMode State, TSubclassOf<UScreen> Class, FName Name)
{
	UScreen* Screen = NewObject<UScreen>(this, Class, Name);
	Screen->bIsFocusable = true;
	Screen->SetOwningPlayer(this);
	Screen->AddToViewport(0);
	Widgets[static_cast<int>(State)] = Screen;
}

void ASTBPlayerController::LeftRight(float Value)
{
	if(CurrentState == ESTBGameMode::Playing && !Gameplay->bIsTimeOver() && PlayerPawn->CanMove())
	{
		/* if borders on the left or right are hit, start bounce back*/
		if(VerticalBorderHitByPlayer() && !PlayerPawn->GetBouncing()) 
		{
			LatestPlayerBounceTimeStamp = UGameplayStatics::GetTimeSeconds(GetWorld());
			CurrentAcceleration.X *= PlayerPawn->BounceIntensity*(-1.f);
			PlayerPawn->SetBouncingInDirection(EBounceBehaviour::Horizontal);
			Value = 0;
		}
		/*as long as player is bouncing back, input value is ignored*/
		else if ((PlayerPawn->GetBouncing()))
		{
			Value = 0;

			/*stop bouncing back after time defined in BP PlayerPawn*/
			if (UGameplayStatics::GetTimeSeconds(GetWorld()) - LatestPlayerBounceTimeStamp > PlayerPawn->BounceStunDuration)
			{
				PlayerPawn->SetBouncingInDirection(EBounceBehaviour::None);
			}
		}
		
		CurrentPlayerLocation.X = FMath::Clamp(
			CurrentPlayerLocation.X + PlayerPawn->GetAcceleratedLocationOnAxis(MoveDirection.X, Value, CurrentAcceleration.X, UGameplayStatics::GetWorldDeltaSeconds(GetWorld())),
			0, 1.f);
	}
}

void ASTBPlayerController::UpDown(float Value)
{
	if(CurrentState == ESTBGameMode::Playing && !Gameplay->bIsTimeOver() && PlayerPawn->CanMove())
	{
		/* if borders on the top or bottom are hit, start bounce back*/
		if(HorizontalBorderHitByPlayer() && !PlayerPawn->GetBouncing()) 
		{
			LatestPlayerBounceTimeStamp = UGameplayStatics::GetTimeSeconds(GetWorld());
			CurrentAcceleration.Y *= PlayerPawn->BounceIntensity*(-1.f);
			PlayerPawn->SetBouncingInDirection(EBounceBehaviour::Vertical);
			Value = 0;
		}
		/*as long as player is bouncing back, input value is ignored*/
		else if ((PlayerPawn->GetBouncing()))
		{
			Value = 0;

			/*stop bouncing back after time defined in BP PlayerPawn*/
			if (UGameplayStatics::GetTimeSeconds(GetWorld()) - LatestPlayerBounceTimeStamp > PlayerPawn->BounceStunDuration)
			{
				PlayerPawn->SetBouncingInDirection(EBounceBehaviour::None);
			}
		}
		CurrentPlayerLocation.Y = FMath::Clamp(
			CurrentPlayerLocation.Y + PlayerPawn->GetAcceleratedLocationOnAxis(MoveDirection.Y, Value, CurrentAcceleration.Y, UGameplayStatics::GetWorldDeltaSeconds(GetWorld())),
			0, 1.f);
	}
}

void ASTBPlayerController::TopButtonPress()
{
	if(const int Index = static_cast<int>(CurrentState); Index >= 0 && Index < static_cast<int>(ESTBGameMode::NumModes))
	{
		Widgets[Index]->Alt2();
	}
}

void ASTBPlayerController::LeftButtonPress()
{
	if(const int Index = static_cast<int>(CurrentState); Index >= 0 && Index < static_cast<int>(ESTBGameMode::NumModes))
	{
		Widgets[Index]->Alt1();
	}
}

void ASTBPlayerController::RightButtonPress()
{
	if(const int Index = static_cast<int>(CurrentState); Index >= 0 && Index < static_cast<int>(ESTBGameMode::NumModes))
	{
		Widgets[Index]->Back();
	}
}

void ASTBPlayerController::BottomButtonPress()
{
	if(const int Index = static_cast<int>(CurrentState); Index >= 0 && Index < static_cast<int>(ESTBGameMode::NumModes))
	{
		Widgets[Index]->Select();
	}	
}

void ASTBPlayerController::SpecialButtonPress()
{
	if(const int Index = static_cast<int>(CurrentState); Index >= 0 && Index < static_cast<int>(ESTBGameMode::NumModes))
	{
		Widgets[Index]->Special();
	}
}

void ASTBPlayerController::ResetPlayer()
{
	CurrentPlayerLocation = FVector2D(0.1f);
	CurrentAcceleration = FVector2D(0.1f);
	if(IsValid(PlayerPawn))
	{
		PlayerPawn->LockMovement(false);
		if(IsValid(GetGameplay()->GetLevelData()))
		{
			const FProgressionMovementData* CurrentMovementData = &GetGameplay()->GetLevelData()->GetDataFromLevelIndex(GetGameplay()->GetLevel())->Movement;
			if (CurrentMovementData->bOverwriteMovement)
			{
				PlayerPawn->PlayerSpeed = CurrentMovementData->Speed;
				PlayerPawn->Acceleration = CurrentMovementData->Acceleration;
				PlayerPawn->Deceleration = CurrentMovementData->NormalDeceleration;
				PlayerPawn->BrakeDeceleration = CurrentMovementData->BrakeDeceleration;
			}
		}
	}
}
