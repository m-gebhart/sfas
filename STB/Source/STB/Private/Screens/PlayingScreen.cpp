// Search for a Star 2023

#include "Screens/PlayingScreen.h"

#include "Gameplay.h"
#include "STBPlayerCameraManager.h"
#include "Components/CanvasPanel.h"
#include "Components/CanvasPanelSlot.h"
#include "Components/Image.h"
#include "Components/TextBlock.h"
#include "Kismet/GameplayStatics.h"
#include "Screens/SettingsScreen.h"
#include "STB/STBGameModes.h"

void UPlayingScreen::NativeConstruct()
{
	Super::NativeConstruct();

	for(int Count = 0; Count < Texts.Num(); ++Count)
	{
		if(Texts[Count]->GetName().Contains("Level"))
		{
			LevelTextFormat = Texts[Count]->Text;
			LevelTextIndex = Count;
		}
		
		if(Texts[Count]->GetName().Contains("Lives"))
		{
			LivesTextFormat = Texts[Count]->Text;
			LivesTextIndex = Count;
		}
		
		if(Texts[Count]->GetName().Contains("Prompt"))
		{
			PromptTextIndex = Count;
		}
		
		if(Texts[Count]->GetName().Contains("Time"))
		{
			TimeTextFormat = Texts[Count]->Text;
			TimeTextIndex = Count;
		}
	}

	for(int Count = 0; Count < Images.Num(); ++Count)
	{
		if(Images[Count]->GetName().Contains("Target"))
		{
			TargetImageIndex = Count;
		}
		else if(Images[Count]->GetName().Contains("Guess"))
		{
			GuessImageIndex = Count;
		}
		else if(Images[Count]->GetName().Contains("Minimap"))
		{
			MinimapImageIndex = Count;
		}
	}	
	
	SetLevel(1);
	SetLives(0);
	
	PlayingState = EPlayingState::Guessing;
}

void UPlayingScreen::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);
	
	if(IsValid(PlayerController))
	{
		//Show Animation after confirming guess (do reveal)
		if(PlayingState == EPlayingState::Showing)
		{
			//Beam Up of Sight (Target)
			if(PlayerController->GetGameplay()->GetWin() && !PlayerController->bIsTargetCaptured())
			{
				static const float TotalDistance = PlayerController->GetPlayerPawn()->GetActorLocation().Z - PlayerController->GetGameplay()->GetBallLocation().Z;
				FVector CurrentBeamingSightLocation = FVector(PlayerController->GetGameplay()->GetLevelData()->GetTargetSightActor()->GetActorLocation() + FVector(0, 0, TotalDistance / PlayerController->GetPlayerPawn()->BeamUpSpeed*InDeltaTime));
				PlayerController->GetGameplay()->GetLevelData()->GetTargetSightActor()->SetActorLocation(CurrentBeamingSightLocation);
			}
			
			//Fade In Animation of Target Icon and Fade Out Animation of Guess Icon on Minimap
			if(TargetImageIndex >= 0 && TargetImageIndex < Images.Num())
			{
				Images[TargetImageIndex]->SetOpacity(FMath::Clamp(Images[TargetImageIndex]->ColorAndOpacity.A + InDeltaTime, 0.0f, 1.0f));

				if(GuessImageIndex >= 0 && GuessImageIndex < Images.Num())
				{
					Images[GuessImageIndex]->SetOpacity(1.0f - Images[TargetImageIndex]->ColorAndOpacity.A/2.f);
				}

				//Fully Faded In
				if(Images[TargetImageIndex]->ColorAndOpacity.A >= 1.0f && (PlayerController->bIsTargetCaptured() || !PlayerController->GetGameplay()->GetWin()))
				{
					PlayingState = EPlayingState::Shown;
					PlayerController->GetPlayerPawn()->EndBeamUp();
				}
			}
		}

		//Set Cursor position
		if(PlayingState == EPlayingState::Guessing && GuessImageIndex >= 0 && GuessImageIndex < Images.Num())
		{
			const FVector2D GuessLocation = PlayerController->GetCurrentPlayerLocation();
			if(UCanvasPanelSlot* GuessSlot = Cast<UCanvasPanelSlot>(Images[GuessImageIndex]->Slot))
			{
				GuessSlot->SetPosition(FVector2D(MinimapPadding + GuessLocation.X*MinimapSize.X, (MinimapSize.Y+MinimapPadding)-(GuessLocation.Y*MinimapSize.Y + MinimapPadding)));
			}	
		}

		//Check Time
		if(PlayingState == EPlayingState::Guessing)
		{
			if (const_cast<UGameplay*>(PlayerController->GetGameplay())->SubtractFromRemainingTime(InDeltaTime) < 0)
			{
				Select();
			}
		}
	}	
}

void UPlayingScreen::Select_Implementation()
{
	if(PlayingState == EPlayingState::Guessing)
	{
		if(IsValid(PlayerController))
		{
			const bool bCorrect = PlayerController->TryMove();
			if(!bCorrect)
			{
				if(const UGameplay* Gameplay = PlayerController->GetGameplay())
				{
					const int Lives = Gameplay->GetLives();
					SetLives(Lives);
				}					
			}
			EndCountdown();
			DoReveal(bCorrect);
		}
	}
	else if(PlayingState == EPlayingState::Shown)
	{
		Reset();
	}
}

void UPlayingScreen::Special_Implementation()
{
	if(IsValid(PlayerController))
	{
		PlayerController->ShowUI(ESTBGameMode::Outro);
	}
}


void UPlayingScreen::Show(bool bShow)
{
	Super::Show(bShow);

	if(bShow)
	{
		Reset();
	}
}

void UPlayingScreen::SetLevel(int Level)
{
	if(LevelTextIndex >= 0 && LevelTextIndex < Texts.Num())
	{
		Texts[LevelTextIndex]->SetText(FText::Format(LevelTextFormat, Level));
	}
}

void UPlayingScreen::SetLives(int Lives)
{
	if(LivesTextIndex >= 0 && LivesTextIndex < Texts.Num())
	{
		Texts[LivesTextIndex]->SetText(FText::Format(LivesTextFormat, Lives));
	}
}

void UPlayingScreen::SetMinimap()
{
	if (UCanvasPanelSlot* MinimapSlot = Cast<UCanvasPanelSlot>(Images[MinimapImageIndex]->Slot))
	{
		//Set Minimap Size
		const ASTBPlayerCameraManager* PlayerCameraManager = Cast<ASTBPlayerCameraManager>(PlayerController->PlayerCameraManager);
		MinimapSize = FVector2D(GEngine->GameViewport->Viewport->GetSizeXY())*PlayerCameraManager->MinimapScale;
		MinimapSlot->SetSize(MinimapSize + FVector2D(MinimapPadding*2, MinimapPadding));

		//reset color of target image
		Images[TargetImageIndex]->SetBrushTintColor(FColor::White);
	}
}

void UPlayingScreen::ShowLevel(bool bShow)
{
	if(LevelTextIndex >= 0 && LevelTextIndex < Texts.Num())
	{
		Texts[LevelTextIndex]->SetOpacity(bShow ? 1.0f : 0.0f);
	}
}

void UPlayingScreen::ShowLives(bool bShow)
{
	if(LivesTextIndex >= 0 && LivesTextIndex < Texts.Num())
	{
		Texts[LivesTextIndex]->SetOpacity(bShow ? 1.0f : 0.0f);
	}
}

void UPlayingScreen::ShowPrompt(bool bShow)
{
	if(PromptTextIndex >= 0 && PromptTextIndex < Texts.Num())
	{
		Texts[PromptTextIndex]->SetOpacity(bShow ? 1.0f : 0.0f);
	}
}

void UPlayingScreen::SetBallLocation()
{
	if(TargetImageIndex >= 0 && TargetImageIndex < Images.Num())
	{
		const FVector BallLocation = PlayerController->GetGameplay()->GetNormalizedBallLocation(); // GetCurrentBallRot
		if(UCanvasPanelSlot* TargetSlot = Cast<UCanvasPanelSlot>(Images[TargetImageIndex]->Slot))
		{
			//Inverse Components for Minimap (Minimap is X/-Y-Axis and Scene is Y/X-Axis)
			const FVector2D TargetPos(MinimapPadding + BallLocation.Y*MinimapSize.X, (MinimapSize.Y + MinimapPadding)-(BallLocation.X*MinimapSize.Y + MinimapPadding));
			TargetSlot->SetPosition(TargetPos);
		}
	}
}

void UPlayingScreen::DoReveal(const bool bLastGuessCorrect)
{
	if(TargetImageIndex >= 0 && TargetImageIndex < Images.Num())
	{
		Images[TargetImageIndex]->SetBrushTintColor(bLastGuessCorrect ? WinColor : LoseColor);
	}
	
	ShowLevel(false);
	ShowLives(false);
	ShowPrompt(true);
	PlayerController->GetPlayerPawn()->ShowBeamUp(bLastGuessCorrect ? WinColor : LoseColor);
	PlayingState = EPlayingState::Showing;
}

void UPlayingScreen::StartCountdown()
{
	GetWorld()->GetTimerManager().SetTimer(CountdownTimerHandle, this, &UPlayingScreen::UpdateCountdown, 0.1f, true, 0);
	Texts[TimeTextIndex]->SetColorAndOpacity(FColor::White);
}

void UPlayingScreen::UpdateCountdown()
{
	const float TimeLeft = PlayerController->GetGameplay()->GetRemainingTime();

	FString SanitizedStr = FString::SanitizeFloat(round(TimeLeft*10)/10);
	FString DecimalStr;
	SanitizedStr.Split(".", &SanitizedStr, &DecimalStr);
	const FString Result = SanitizedStr + "." + DecimalStr[0];
				
	Texts[TimeTextIndex]->SetText(FText::FromString(Result));
}

void UPlayingScreen::EndCountdown()
{
	GetWorld()->GetTimerManager().ClearTimer(CountdownTimerHandle);
	Texts[TimeTextIndex]->SetText(PlayerController->GetGameplay()->GetWin() ? FText::FromString("SUCCESS!") : FText::FromString("FAILED!"));
	Texts[TimeTextIndex]->SetColorAndOpacity(PlayerController->GetGameplay()->GetWin() ? WinColor : LoseColor);
}

void UPlayingScreen::Reset()
{
	if(TargetImageIndex >= 0 && TargetImageIndex < Images.Num())
	{
		Images[TargetImageIndex]->SetOpacity(0.0f);
	}

	if(GuessImageIndex >= 0 && GuessImageIndex < Images.Num())
	{
		Images[GuessImageIndex]->SetOpacity(1.0f);
	}	

	if(IsValid(PlayerController))
	{
		PlayerController->ContinueGame();
		
		if(const UGameplay* Gameplay = PlayerController->GetGameplay())
		{
			const int Level = Gameplay->GetLevel() + 1;
			SetLevel(Level);
			SetLives(Gameplay->GetLives());
			const_cast<UGameplay*>(Gameplay)->SetTime(Gameplay->GetTotalTimeLimit());
		}
	}
	
	SetMinimap();
	StartCountdown();

	ShowLevel(true);
	ShowLives(true);
	ShowPrompt(false);
	SetBallLocation();
	PlayingState = EPlayingState::Guessing;
}

