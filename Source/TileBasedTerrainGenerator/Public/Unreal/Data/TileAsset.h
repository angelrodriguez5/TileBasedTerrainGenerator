

#pragma once

#include "CoreMinimal.h"
#include "Generator/Interfaces/TileBase.h"

#include "TileAsset.generated.h"

/**
 * 
 */
UCLASS()
class TILEBASEDTERRAINGENERATOR_API UTileAsset : public UPrimaryDataAsset, public TileBase
{

	GENERATED_BODY()

public:
	UTileAsset();
	~UTileAsset();

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TSubclassOf<AActor> tileActor;

};
