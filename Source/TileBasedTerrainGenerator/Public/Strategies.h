#pragma once
#include "Interfaces/TileBase.h"
#include "Interfaces/TileMapBase.h"
#include <vector>

std::shared_ptr<TileBase> RandomTileStrategy(TileMapBase& map, const CellIdx& cell);

std::shared_ptr<TileBase> SimilarToNeighborTileStrategy(TileMapBase &map, const CellIdx& cell);

CellIdx LowestEntropyCellStrategy(TileMapBase& map);

CellIdx RandomUncollapsedCellStrategy(TileMapBase& map);