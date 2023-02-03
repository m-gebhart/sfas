// Search for a Star 2023

#pragma once

#include "CoreMinimal.h"
#include "Engine/StaticMeshActor.h"
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
struct FProgressionMovementData
{
	GENERATED_BODY();

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	bool bOverwriteMovement;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (EditCondition = "bOverwriteMovement", EditConditionHides))
	float Speed;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (EditCondition = "bOverwriteMovement", EditConditionHides))
	float Acceleration;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (EditCondition = "bOverwriteMovement", EditConditionHides))
	float NormalDeceleration;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (EditCondition = "bOverwriteMovement", EditConditionHides))
	float BrakeDeceleration;
};

USTRUCT(BlueprintType)
struct FProgressionStaticMeshData
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadonly)
	UStaticMesh* StaticMeshAsset;

	UPROPERTY(EditAnywhere, BlueprintReadonly)
	FVector StaticMeshScale = {1.0f, 1.0f, 1.0f};

	UPROPERTY(EditAnywhere, BlueprintReadonly)
	TArray<UMaterial*> MeshAssetMaterials;
};

USTRUCT(BlueprintType)
struct FProgressionLevelData
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TArray<FProgressionCharacterData> Characters;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FProgressionMovementData Movement;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FBoxSphereBounds BallBounds;

    UPROPERTY(EditAnywhere, BlueprintReadOnly)
    FProgressionStaticMeshData GroundMesh;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
    FProgressionStaticMeshData TargetSightMesh;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float RequiredDistance;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float TimeLimit;	
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
	AStaticMeshActor* GetTargetSightActor() const;
	FProgressionLevelData* GetDataFromLevelIndex(int index);

protected:

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TArray<FProgressionLevelData> Levels;

#if WITH_EDITOR
	friend class FProgressionDetails;
#endif

private:

	UPROPERTY(VisibleAnywhere)
	TArray<AActor*> SpawnedActors;

	UPROPERTY(VisibleAnywhere)
	AStaticMeshActor* GroundMesh;

	UPROPERTY(VisibleAnywhere)
	AStaticMeshActor* TargetSightMesh;
};
