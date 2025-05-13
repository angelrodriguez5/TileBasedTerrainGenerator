

#pragma once

#include "CoreMinimal.h"
#include "Unreal/Data/TileAsset.h"	
#include <Generator/Interfaces/TileConstraintBase.h>

#include "TileConstraintsAsset.generated.h"

USTRUCT(BlueprintType)
struct FTileConstraintInfo
{
	GENERATED_BODY()

public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	UTileAsset* tile;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (EditCondition = "!exclusiveAllowedTiles"))
	TArray<UTileAsset*> disallowedTiles;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	bool exclusiveAllowedTiles = false;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (EditCondition = "exclusiveAllowedTiles"))
	TArray<UTileAsset*> exclusiveTiles;
};

UCLASS()
class TILEBASEDTERRAINGENERATOR_API UTileConstraintsAsset : public UPrimaryDataAsset
{
	GENERATED_BODY()	

public: 

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TArray<FTileConstraintInfo> tileConstraintsInfo;

	std::vector<TileConstraintBase*> CreateTileConstraints(const std::vector<TileBase*>& tileSet);
};
