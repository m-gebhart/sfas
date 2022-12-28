// Search for a Star 2023


#include "STBPlayerController.h"

#include "EngineUtils.h"
#include "ProgressionData.h"
#include "Gameplay.h"
#include "Screen.h"
#include "Screens/PlayingScreen.h"
#include "STBPlayerCameraManager.h"
#include "GameFramework/Character.h"
#include "GameFramework/PawnMovementComponent.h"
#include "Kismet/GameplayStatics.h"
#include "STB/STBGameModes.h"

const FString ASTBPlayerController::TopButtonActionName = TEXT("TopButton");
const FString ASTBPlayerController::LeftButtonActionName = TEXT("LeftButton");
const FString ASTBPlayerController::RightButtonActionName = TEXT("RightButton");
const FString ASTBPlayerController::BottomButtonActionName = TEXT("BottomButton");
const FString ASTBPlayerController::SpecialLeftButtonActionName = TEXT("SpecialLeftButton");
const FString ASTBPlayerController::SpecialRightButtonActionName = TEXT("SpecialRightButton");
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
		LastOrbitPawnViewRotation = (OrbitPivot - LastOrbitPawnLocation).GetSafeNormal().Rotation();
		CurrentPawn->SetActorLocationAndRotation(LastOrbitPawnLocation, LastOrbitPawnViewRotation);
		SetControlRotation(Rotation);
	}
}

void ASTBPlayerController::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	if(IsValid(Gameplay) && CurrentState == ESTBGameMode::Playing)
	{
		const auto Bounds = Gameplay->GetCurrentBallBounds();
		DrawDebugBox(GetWorld(), Bounds.Origin, Bounds.BoxExtent, FColor::Green, false, 0.2f, SDPG_Foreground, 1.0f);		
		DrawDebugSphere(GetWorld(), Gameplay->GetBallLocation(), Bounds.SphereRadius, 10.0f, FColor::Red, false, 0.2f, SDPG_Foreground, 1.0f);
	}
}

void ASTBPlayerController::CreateUI()
{
	Widgets.Init(nullptr, static_cast<int>(ESTBGameMode::NumModes));
	SetupScreen(ESTBGameMode::Intro, IntroClass, TEXT("Intro"));
	SetupScreen(ESTBGameMode::MainMenu, MenuClass, TEXT("MainMenu"));
	SetupScreen(ESTBGameMode::Settings, SettingsClass, TEXT("Settings"));
	SetupScreen(ESTBGameMode::Playing, PlayingClass, TEXT("Playing"));
	SetupScreen(ESTBGameMode::GameOver, GameOverClass, TEXT("GameOver"));
	SetupScreen(ESTBGameMode::Outro, OutroClass, TEXT("Outro"));
	ShowUI(ESTBGameMode::Intro);
}

void ASTBPlayerController::BeginNewGame()
{
	if(Gameplay != nullptr)
	{
		CurrentPlayerLocation = FVector2D::ZeroVector;
		Gameplay->StartNewGame();
		Gameplay->NextLevel();
	}
}

void ASTBPlayerController::ContinueGame()
{
	if(Gameplay != nullptr)
	{
		CurrentPlayerLocation = FVector2D::ZeroVector;

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
	return Gameplay->TryMove(CurrentPlayerLocation, GetCurrentBallLocation());
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
		InputComponent->BindAction(*SpecialLeftButtonActionName, IE_Pressed, this, &ASTBPlayerController::SpecialLeftButtonPress);
		InputComponent->BindAction(*SpecialRightButtonActionName, IE_Pressed, this, &ASTBPlayerController::SpecialRightButtonPress);

		InputComponent->BindAxis(*RightStickXAxisName, this, &ASTBPlayerController::LeftRight);
		InputComponent->BindAxis(*RightStickYAxisName, this, &ASTBPlayerController::UpDown);
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
	if(CurrentState == ESTBGameMode::Playing)
	{
		CurrentPlayerLocation.X = FMath::Clamp(CurrentPlayerLocation.X + Value*PlayerSpeed, 0, 1.f);
	}
}

void ASTBPlayerController::UpDown(float Value)
{
	if(CurrentState == ESTBGameMode::Playing)
	{
		CurrentPlayerLocation.Y = FMath::Clamp(CurrentPlayerLocation.Y + Value*PlayerSpeed, 0, 1.f);
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

void ASTBPlayerController::SpecialLeftButtonPress()
{
	if(const int Index = static_cast<int>(CurrentState); Index >= 0 && Index < static_cast<int>(ESTBGameMode::NumModes))
	{
		Widgets[Index]->Special1();
	}
}

void ASTBPlayerController::SpecialRightButtonPress()
{
	if(const int Index = static_cast<int>(CurrentState); Index >= 0 && Index < static_cast<int>(ESTBGameMode::NumModes))
	{
		Widgets[Index]->Special2();
	}
}
