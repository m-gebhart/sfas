// Search for a Star 2023

#pragma once

#include "CoreMinimal.h"
#include "ProgressionData.generated.h"

USTRUCT(BlueprintType)
struct FProgressionCharacterData
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TSoftClassPtr<AActor> Character;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TSoftObjectPtr<UAnimationAsset> Animation;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FVector Location;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FRotator Rotation;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float Position;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TSoftObjectPtr<USkeletalMeshComponent> SkeletalMesh;	
};

USTRUCT(BlueprintType)
struct FProgressionLevelData
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TArray<FProgressionCharacterData> Characters;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FBoxSphereBounds BallBounds;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float RequiredDistance;	
};

UCLASS()
class STB_API AProgressionData : public AActor
{
	GENERATED_BODY()

public:

	AProgressionData();

	const FProgressionLevelData* SpawnLevel(int32 Level);
	void CleanupLevel();
	int GetNumLevels() const; 

protected:

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TArray<FProgressionLevelData> Levels;

#if WITH_EDITOR
	friend class FProgressionDetails;
#endif

private:

	UPROPERTY(VisibleAnywhere)
	TArray<AActor*> SpawnedActors;
	
};
