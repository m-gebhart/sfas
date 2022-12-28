// Search for a Star 2023

#include "Screens/PlayingScreen.h"

#include "AIController.h"
#include "Gameplay.h"
#include "STBPlayerCameraManager.h"
#include "Components/CanvasPanel.h"
#include "Components/CanvasPanelSlot.h"
#include "Components/Image.h"
#include "Components/TextBlock.h"


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
		else if(Texts[Count]->GetName().Contains("Lives"))
		{
			LivesTextFormat = Texts[Count]->Text;
			LivesTextIndex = Count;
		}
		else if(Texts[Count]->GetName().Contains("Prompt"))
		{
			PromptTextIndex = Count;
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
		//Fade In Animation after Guess
		if(PlayingState == EPlayingState::Showing)
		{
			if(TargetImageIndex >= 0 && TargetImageIndex < Images.Num())
			{
				Images[TargetImageIndex]->SetOpacity(FMath::Clamp(Images[TargetImageIndex]->ColorAndOpacity.A + InDeltaTime, 0.0f, 1.0f));

				if(GuessImageIndex >= 0 && GuessImageIndex < Images.Num())
				{
					Images[GuessImageIndex]->SetOpacity(1.0f - Images[TargetImageIndex]->ColorAndOpacity.A);
				}

				//Fully Faded In
				if(Images[TargetImageIndex]->ColorAndOpacity.A >= 1.0f)
				{
					PlayingState = EPlayingState::Shown;
				}
			}
		}

		//Set Cursor position
		if(PlayingState == EPlayingState::Guessing && GuessImageIndex >= 0 && GuessImageIndex < Images.Num())
		{
			const FVector2D GuessLocation = PlayerController->GetCurrentPlayerLocation();
			if(UCanvasPanelSlot* GuessSlot = Cast<UCanvasPanelSlot>(Images[GuessImageIndex]->Slot))
			{
				GuessSlot->SetPosition(GuessLocation*MinimapSize);
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

			DoReveal(bCorrect);
		}
	}
	else if(PlayingState == EPlayingState::Shown)
	{
		Reset();
	}
}

void UPlayingScreen::Show(bool bShow)
{
	Super::Show(bShow);
	SetMinimap();

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
		ASTBPlayerCameraManager* PlayerCameraManager = Cast<ASTBPlayerCameraManager>(PlayerController->PlayerCameraManager);
		MinimapSize = FVector2D(GEngine->GameViewport->Viewport->GetSizeXY())*PlayerCameraManager->MinimapScale;
		MinimapSlot->SetSize(MinimapSize);

		//Attach Level Text right below Minimap
		if (UCanvasPanelSlot* LevelTextSlot = Cast<UCanvasPanelSlot>(Texts[LevelTextIndex]->Slot))
		{
			LevelTextSlot->SetPosition(FVector2D(LevelTextSlot->GetPosition().X, MinimapSize.Y));
		}
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
			//Inverse Components (Minimap is X/-Y-Axis and Scene is Y/X-Axis)
			const FVector2D TargetPos(BallLocation.Y*MinimapSize.Y, MinimapSize.X-BallLocation.X*MinimapSize.X);
			TargetSlot->SetPosition(TargetPos);
		}

		Images[TargetImageIndex]->SetOpacity(1.0f);
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
	PlayingState = EPlayingState::Showing;
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
		}
	}

	ShowLevel(true);
	ShowLives(true);
	ShowPrompt(false);
	SetBallLocation();
	PlayingState = EPlayingState::Guessing;
}

