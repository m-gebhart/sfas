// Search for a Star 2023

#include "ProgressionData.h"

AProgressionData::AProgressionData()
{
}

const FProgressionLevelData* AProgressionData::SpawnLevel(int32 Level)
{
	FProgressionLevelData* LevelData = nullptr;

	if(Level >= 0 && Levels.Num() > Level)
	{
		LevelData = &Levels[Level];
		for(int CharacterIndex = 0; CharacterIndex < LevelData->Characters.Num(); ++CharacterIndex)
		{
			FProgressionCharacterData& CharacterData = LevelData->Characters[CharacterIndex];
			if(CharacterData.Character.IsValid() || CharacterData.Character.LoadSynchronous())
			{
				FActorSpawnParameters SpawnParameters;
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
}

int AProgressionData::GetNumLevels() const
{
	return Levels.Num();
}
