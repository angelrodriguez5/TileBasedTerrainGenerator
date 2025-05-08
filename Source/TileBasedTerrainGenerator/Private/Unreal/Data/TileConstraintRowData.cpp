


#include "Unreal/Data/TileConstraintRowData.h"

void FTileConstraintRowData::OnDataTableChanged(const UDataTable* InDataTable, const FName InRowName)
{
	if (exclusiveAllowedTiles)
	{
		if (disallowedTiles.Num() > 0)
		{
			disallowedTiles.Empty();
		}
	}
	else
	{
		if (exclusiveTiles.Num() > 0)
		{
			exclusiveTiles.Empty();
		}
	}	
}
