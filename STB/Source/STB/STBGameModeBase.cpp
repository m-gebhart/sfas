// Search for a Star 2023


#include "STBGameModeBase.h"
#include "STBGameModes.h"
#include "STBPawn.h"
#include "STBPlayerController.h"
#include "Kismet/GameplayStatics.h"

ASTBGameModeBase::ASTBGameModeBase()
{
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnClassFinder(TEXT("/Game/Blueprints/BP_Player"));
	if (DefaultPawnClass)
		DefaultPawnClass = PlayerPawnClassFinder.Class;
	else
		DefaultPawnClass = ASTBPawn::StaticClass();
	
	PlayerControllerClass = ASTBPlayerController::StaticClass();
}

void ASTBGameModeBase::BeginPlay()
{
	Super::BeginPlay();
	SetupUI();
}

void ASTBGameModeBase::SetupUI() const
{
	ASTBPlayerController* Controller = Cast<ASTBPlayerController>(UGameplayStatics::GetPlayerController(this, 0));
	if(IsValid(Controller))
	{
		Controller->CreateUI();
	}
}
