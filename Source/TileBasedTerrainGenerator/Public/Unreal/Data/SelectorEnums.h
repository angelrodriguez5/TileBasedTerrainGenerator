

#pragma once

#include <memory>

#include "CoreMinimal.h"
#include "Generator/Implementations/TileMapSquare.h"
#include "Generator/Strategies.h"

#include "SelectorEnums.generated.h"

// TILE MAP TYPES
UENUM(BlueprintType)
enum class ETileMapType : uint8
{
	Square,
	Hexagonal
};

inline std::shared_ptr<TileMapBase> CreateTileMap(ETileMapType tileMapType, int width, int height)
{
	switch (tileMapType)
	{
	case ETileMapType::Square:
		return std::make_shared<TileMapSquare>(width, height);
	case ETileMapType::Hexagonal:
		//return new TileMapHexagonal(width, height);
	default:
		return nullptr;
	}
}

// TILE STRATEGIES
UENUM(BlueprintType)
enum class ETileStrategy : uint8
{
	RandomTile,
	SimilarToNeighborTile,
};

inline std::function<TileBase* (TileMapBase&, const CellIdx&)> GetTileStrategy(ETileStrategy tileStrategy)
{
	switch (tileStrategy)
	{
	case ETileStrategy::RandomTile:
		return RandomTileStrategy;
	case ETileStrategy::SimilarToNeighborTile:
		return SimilarToNeighborTileStrategy;
	default:
		return nullptr;
	}
}

// CELL STRATEGIES
UENUM(BlueprintType)
enum class ECellStrategy : uint8
{
	LowestEntropy,
	RandomUncollapsed,
};

inline std::function<CellIdx(TileMapBase&)> GetCellStrategy(ECellStrategy cellStrategy)
{
	switch (cellStrategy)
	{
	case ECellStrategy::LowestEntropy:
		return LowestEntropyCellStrategy;
	case ECellStrategy::RandomUncollapsed:
		return RandomUncollapsedCellStrategy;
	default:
		return nullptr;
	}
}