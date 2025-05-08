#pragma once
#include "Generator/Interfaces/TileConstraintBase.h"

/// <summary>
/// This constraint forbids one tile to be adjacent to another
/// </summary>
class SimpleAdjacencyConstraint : public TileConstraintBase
{
public:
	SimpleAdjacencyConstraint(const TileBase* tile, const TileBase* other)
		: m_tile(tile), m_other(other)
	{};

	// Inherited via TileConstraintBase
	bool IsConstraintFollowed(const TileConstraintArgs& args) override;

private:
	const TileBase* m_tile;
	const TileBase* m_other;

};