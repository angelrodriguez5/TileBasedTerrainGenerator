#pragma once
#include "Generator/Interfaces/TileBase.h"
#include <vector>
#include <set>
#include <memory>


struct CellIdx
{
	int x;
	int y;

	CellIdx() 
		: x(0), y(0)
	{}

	CellIdx(int x, int y)
		: x(x), y(y)
	{}

	bool operator==(const CellIdx& other)
	{
		return x == other.x && y == other.y;
	};
};

class TileMapBase
{
public:

	virtual std::vector<TileBase*> GetTileSet() = 0;

	virtual void SetTileSet(std::vector<TileBase*> tileSet) = 0;

	virtual std::vector<CellIdx> GetNeighbors(const CellIdx &cell) = 0;

	virtual std::vector<TileBase*> GetSuperpositionAt(const CellIdx &cell) = 0;

	virtual TileBase* GetTileAt(const CellIdx& cell) = 0;

	virtual bool IsCellCollapsed(const CellIdx& cell) = 0;

	// Returns wether or not the cell was collapsed correctly
	virtual bool CollapseCell(const CellIdx& cell, const TileBase* tile) = 0;

	// Returns wether or not the superposition at the cell changed
	virtual bool UpdateCellSuperposition(const CellIdx& cell, const std::vector<TileBase*>& newSuperposition) = 0;

	// Returns wether or not all the cells of the map are collapsed
	virtual bool CheckComplete() = 0;

	virtual void Reset() = 0;

	virtual int GetWidth() = 0;

	virtual int GetHeight() = 0;
};