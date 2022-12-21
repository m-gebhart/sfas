// Search for a Star 2023

#pragma once

#include "CoreMinimal.h"
#include "STBPlayerController.h"
#include "Blueprint/UserWidget.h"
#include "Screen.generated.h"

class UImage;
class UTextBlock;
class UCanvasPanel;


UENUM(BlueprintType)
enum class EScreenState : uint8
{
	Off,
	AnimatingOn,
	On,
	AnimatingOff,

	NumStates
};

/**
 * Class to control the functionality of a single menu
 */
UCLASS()
class STB_API UScreen : public UUserWidget
{
	GENERATED_BODY()

public:

	virtual void NativeConstruct() override;
	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;

	UFUNCTION(BlueprintCallable)
	virtual void Show(bool bShow);
	
	UFUNCTION(BlueprintNativeEvent)
	void Select();
	virtual void Select_Implementation();

	UFUNCTION(BlueprintNativeEvent)
	void Back();
	virtual void Back_Implementation();

	UFUNCTION(BlueprintNativeEvent)
	void Alt1();
	virtual void Alt1_Implementation();

	UFUNCTION(BlueprintNativeEvent)
	void Alt2();
	virtual void Alt2_Implementation();

	UFUNCTION(BlueprintNativeEvent)
	void OnShown();
	virtual void OnShown_Implementation();

	UFUNCTION(BlueprintNativeEvent)
	void OnHidden();
	virtual void OnHidden_Implementation();
	
protected:

	UFUNCTION(BlueprintCallable)
	void SetState(EScreenState InState);

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	ASTBPlayerController * PlayerController;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	UCanvasPanel * Canvas;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TArray<UImage*> Images;	
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TArray<UTextBlock*> Texts;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	EScreenState State;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	float TimeInState;
};
