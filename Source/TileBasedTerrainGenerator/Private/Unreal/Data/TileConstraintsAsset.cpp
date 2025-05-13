#include "Unreal/Data/TileConstraintsAsset.h"
#include "Generator/Implementations/SimpleAdjacencyConstraint.h"
#include "Generator/VectorUtils.h"

std::vector<TileConstraintBase*> UTileConstraintsAsset::CreateTileConstraints(const std::vector<TileBase*>& tileSet)
{
    std::vector<TileConstraintBase*> constraints;

	for (auto& constraintInfo : tileConstraintsInfo)
	{
		// Get array of disallowed tiles
		TArray<TileBase*> disallowedTiles;
		if (constraintInfo.exclusiveAllowedTiles)
		{
			// If exclusiveAllowedTiles is checked, then every tile in the tileSet
			// is disallowed except for the exclusive tiles
			for (auto* tile : tileSet)
			{
				bool found = false;
				for (auto* exclusiveTile : constraintInfo.exclusiveTiles)
				{
					if (*tile == *exclusiveTile)
					{
						found = true;
						break;
					}
				}

				if (!found)
				{
					disallowedTiles.Add(tile);
				}
			}
		}
		else
		{
			disallowedTiles.Append(constraintInfo.disallowedTiles);
		}

		for (auto* disallowedTile : disallowedTiles)
		{
			// Create a new constraint for each combination of tiles
			TileConstraintBase* newConstraint = new SimpleAdjacencyConstraint(constraintInfo.tile, disallowedTile);

			// Avoid duplicated constraints
			bool found = false;
			for (auto* constraint : constraints)
			{
				if (*newConstraint == *constraint)
				{
					found = true;
					break;
				}
			}

			if (!found)
			{
				constraints.push_back(newConstraint);
			}
			else
			{
				delete newConstraint;
			}
		}
	}

    return constraints;
}
