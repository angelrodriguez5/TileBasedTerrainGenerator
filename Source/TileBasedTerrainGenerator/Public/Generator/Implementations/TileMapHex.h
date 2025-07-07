#pragma once
#include "Generator/Interfaces/TileMapBase.h"

/// <summary>
/// A hexagonal tile map. All calculations are based on the following asumptions:
///  - Cells will be positioned with a "pointy top" orientation
///  - The first row will start at the origin and the second row will share the top right edge
///  - The third row will again be aligned with the origin, as it continues in a zig-zag pattern
///  - That is, odd rows will be offset by half a tile size in the x direction
///  - Tile size is defined as follows:
///    - X = width = flat to flat
///    - Y = height = point to point
/// </summary>
class TileMapHex : public TileMapBase
{
public:
	TileMapHex(const int width, const int height);;

	// Inherited via TileMapBase
	std::vector<TileBase*> GetTileSet() override;
	void SetTileSet(std::vector<TileBase*> tileSet) override;
	std::vector<CellIdx> GetNeighbors(const CellIdx& cell) override;
	std::vector<TileBase*> GetSuperpositionAt(const CellIdx& cell) override;
	TileBase* GetTileAt(const CellIdx& cell) override;
	bool CheckComplete() override;
	bool IsCellCollapsed(const CellIdx& cell) override;
	bool CollapseCell(const CellIdx& cell, const TileBase* tile) override;
	bool UpdateCellSuperposition(const CellIdx& cell, const std::vector<TileBase*>& newSuperposition) override;
	void Reset() override;
	int GetWidth() override;
	int GetHeight() override;
	std::vector<double> GetCellTransformPosition(const CellIdx& cell, const std::vector<double>& tileSize, const std::vector<double>& origin) override;

protected:
	const int m_width, m_height;

	std::vector<TileBase*> m_tileSet;
	// The content of each cell is the index in m_possibleSuperpositions which 
	// correspond to the superposition in that cell
	std::vector<int> m_mapCells;
	// Store each possible superposition only once
	std::vector<std::vector<TileBase*>> m_possibleSuperpositions;

	int GetSuperpositionIndexAt(const CellIdx& cellIdx);
	void SetSuperpositionIndexAt(const CellIdx& cellIdx, const int superpositionIdx);


};