// Search for a Star 2023

#include "ProgressionData.h"

#include "Gameplay.h"
#include "STBPlayerController.h"
#include "Components/BoxComponent.h"
#include "Engine/StaticMeshActor.h"
#include "Kismet/GameplayStatics.h"

AProgressionData::AProgressionData()
{
}

const FProgressionLevelData* AProgressionData::SpawnLevel(int32 Level)
{
	FProgressionLevelData* LevelData = nullptr;
	
	if(Level >= 0 && Levels.Num() > Level)
	{
		LevelData = &Levels[Level];

		//Spawn Ground as Static Mesh Cube and align to scale of ball bounds' box extent
		FActorSpawnParameters SpawnParameters;
		GroundMesh = GetWorld()->SpawnActor<AStaticMeshActor>(LevelData->BallBounds.Origin, FRotator::ZeroRotator, SpawnParameters);
		if (GroundMesh)
		{
			GroundMesh->SetMobility(EComponentMobility::Movable);
			if (LevelData->GroundMesh.StaticMeshAsset)
			{
				GroundMesh->GetStaticMeshComponent()->SetStaticMesh(LevelData->GroundMesh.StaticMeshAsset);
			}
			if (!LevelData->GroundMesh.MeshAssetMaterials.IsEmpty())
			{
				for (int MatCount = 0; MatCount < LevelData->GroundMesh.MeshAssetMaterials.Num(); MatCount++)
				{
					GroundMesh->GetStaticMeshComponent()->SetMaterial(MatCount, LevelData->GroundMesh.MeshAssetMaterials[MatCount]);
				}
			}
			
			FVector GroundBounds;
			FVector GroundOrigin;
			GroundMesh->GetActorBounds(false, GroundOrigin, GroundBounds, false);
			GroundMesh->SetActorScale3D((FVector::One()/GroundBounds) * LevelData->BallBounds.BoxExtent * LevelData->GroundMesh.StaticMeshScale);
		}

		//Spawn Ball = Target Sight to get
		const FVector SightSpawnLocation = Cast<ASTBPlayerController>(UGameplayStatics::GetPlayerController(GetWorld(), 0))->GetGameplay()->GetBallLocation();
		TargetSightMesh = GetWorld()->SpawnActor<AStaticMeshActor>(SightSpawnLocation, FRotator::ZeroRotator, SpawnParameters);
		if(TargetSightMesh)
		{
			TargetSightMesh->SetMobility(EComponentMobility::Movable);
			if (LevelData->TargetSightMesh.StaticMeshAsset)
			{
				TargetSightMesh->GetStaticMeshComponent()->SetStaticMesh(LevelData->TargetSightMesh.StaticMeshAsset);
				TargetSightMesh->SetActorScale3D(LevelData->TargetSightMesh.StaticMeshScale);
			}
			if (!LevelData->GroundMesh.MeshAssetMaterials.IsEmpty())
			{
				for (int MatCount = 0; MatCount < LevelData->TargetSightMesh.MeshAssetMaterials.Num(); MatCount++)
				{
					TargetSightMesh->GetStaticMeshComponent()->SetMaterial(MatCount, LevelData->TargetSightMesh.MeshAssetMaterials[MatCount]);
				}
			}
		}

		
		//Spawn Enemies
		
		//Spawn Character
		for(int CharacterIndex = 0; CharacterIndex < LevelData->Characters.Num(); ++CharacterIndex)
		{
			FProgressionCharacterData& CharacterData = LevelData->Characters[CharacterIndex];
			if(CharacterData.Character.IsValid() || CharacterData.Character.LoadSynchronous())
			{
				SpawnParameters.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;
				AActor* SpawnedActor = GetWorld()->SpawnActor<AActor>(CharacterData.Character.Get(), CharacterData.Location, CharacterData.Rotation, SpawnParameters);
				SpawnedActors.Add(SpawnedActor);

				if(CharacterData.Animation.IsValid() || CharacterData.Animation.LoadSynchronous())
				{
					TArray<USceneComponent*> Components;
					SpawnedActor->GetRootComponent()->GetChildrenComponents(false, Components);

					for (UActorComponent* Component : Components)
					{
						if (Component->GetClass() == USkeletalMeshComponent::StaticClass())
						{
							CharacterData.SkeletalMesh = Cast<USkeletalMeshComponent>(Component);
							break;
						}
					}

					if(CharacterData.SkeletalMesh.IsValid())
					{
						USkeletalMeshComponent* SkeletalMesh = CharacterData.SkeletalMesh.Get();
						SkeletalMesh->SetAnimationMode(EAnimationMode::AnimationSingleNode);
						SkeletalMesh->SetAnimation(CharacterData.Animation.Get());
						SkeletalMesh->SetPlayRate(0.0f);
						SkeletalMesh->SetPosition(CharacterData.Position);
					}
				}
			}
		}
	}

	return LevelData;
}

void AProgressionData::CleanupLevel()
{
	for(AActor* SpawnedActor : SpawnedActors)
	{
		SpawnedActor->Destroy(false, false);
	}

	SpawnedActors.Empty();
	if (GroundMesh)
		GroundMesh->Destroy();
	if (TargetSightMesh)
		TargetSightMesh->Destroy();
}

int AProgressionData::GetNumLevels() const
{
	return Levels.Num();
}

AStaticMeshActor* AProgressionData::GetTargetSightActor() const
{
	return TargetSightMesh;
}
