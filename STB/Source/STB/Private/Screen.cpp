// Search for a Star 2023

#include "Screen.h"

#include "Components/CanvasPanel.h"
#include "Components/Image.h"
#include "Components/TextBlock.h"
#include "Kismet/GameplayStatics.h"

void UScreen::NativeConstruct()
{
	Super::NativeConstruct();

	PlayerController = Cast<ASTBPlayerController>(UGameplayStatics::GetPlayerController(GetWorld(), 0));

	const UClass * Class = GetClass();
	FProperty* Property = Class->PropertyLink;
	
	while(Property != nullptr)
	{
		if(Property->GetClass() == FObjectProperty::StaticClass())
		{
			const FObjectProperty * ObjectProperty = CastField<FObjectProperty>(Property);
			UObject * Object = ObjectProperty->GetObjectPropertyValue_InContainer(this);
			
			if(ObjectProperty->PropertyClass == UCanvasPanel::StaticClass())
			{
				if(UCanvasPanel * CanvasPanel = Cast<UCanvasPanel>(Object))
				{
					Canvas = CanvasPanel;
				}
			}
			else if(ObjectProperty->PropertyClass == UImage::StaticClass())
			{
				if(UImage * Image = Cast<UImage>(Object))
				{
					Images.Add(Image);
				}
			}			
			else if(ObjectProperty->PropertyClass == UTextBlock::StaticClass())
			{
				if(UTextBlock * Text = Cast<UTextBlock>(Object))
				{
					Texts.Add(Text);
				}
			}
		}
				
		Property = Property->PropertyLinkNext;
	}

	if(IsValid(Canvas))
	{
		Canvas->SetRenderOpacity(0.0f);
	}
	
	SetVisibility(ESlateVisibility::Hidden);
	State = EScreenState::Off;
}

void UScreen::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);
	TimeInState += InDeltaTime;

	if(IsValid(Canvas))
	{
		const float Progress = FMath::Clamp(TimeInState, 0.0f, 1.0f);
		if(State == EScreenState::AnimatingOn)
		{
			Canvas->SetRenderOpacity(Progress);

			if(Progress >= 1.0f)
			{
				SetState(EScreenState::On);
			}
		}
		else if(State == EScreenState::AnimatingOff)
		{
			Canvas->SetRenderOpacity(1.0f - Progress);

			if(Progress >= 1.0f)
			{
				SetState(EScreenState::Off);
			}
		}
	}
}

void UScreen::Show(bool bShow)
{
	if((bShow && State == EScreenState::Off) || (!bShow && State == EScreenState::On))
	{
		SetState(bShow ? EScreenState::AnimatingOn : EScreenState::AnimatingOff);
	}
}

void UScreen::Select_Implementation()
{
}

void UScreen::Back_Implementation()
{
}

void UScreen::Alt1_Implementation()
{
}

void UScreen::Alt2_Implementation()
{
}

void UScreen::OnShown_Implementation()
{
}

void UScreen::OnHidden_Implementation()
{
}

void UScreen::SetState(EScreenState InState)
{
	State = InState;
	TimeInState = 0.0f;
	
	switch(State)
	{
	case EScreenState::Off:
		SetVisibility(ESlateVisibility::Hidden);
		OnHidden();
		break;
	case EScreenState::On:
		if(bIsFocusable)
		{
			SetFocus();
			SetKeyboardFocus();
		}
		OnShown();
		break;		
	case EScreenState::AnimatingOn:
	case EScreenState::AnimatingOff:
	default:
		SetVisibility(ESlateVisibility::Visible);
		break;		
	}
}
