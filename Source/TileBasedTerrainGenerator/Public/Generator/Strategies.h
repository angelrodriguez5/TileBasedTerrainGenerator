#pragma once
#include "Generator/Interfaces/TileBase.h"
#include "Generator/Interfaces/TileMapBase.h"
#include <vector>

TileBase* RandomTileStrategy(TileMapBase& map, const CellIdx& cell);

TileBase* SimilarToNeighborTileStrategy(TileMapBase& map, const CellIdx& cell);

CellIdx LowestEntropyCellStrategy(TileMapBase& map);

CellIdx RandomUncollapsedCellStrategy(TileMapBase& map);