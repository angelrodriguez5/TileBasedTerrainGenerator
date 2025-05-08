

#pragma once

#include "CoreMinimal.h"
#include "Unreal/Data/TileAsset.h"	

#include "TileConstraintRowData.generated.h"

USTRUCT()
struct TILEBASEDTERRAINGENERATOR_API FTileConstraintRowData : public FTableRowBase
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

	void OnDataTableChanged(const UDataTable* InDataTable, const FName InRowName) override;
};
