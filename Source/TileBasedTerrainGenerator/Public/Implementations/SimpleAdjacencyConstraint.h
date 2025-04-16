#pragma once
#include "Interfaces/TileConstraintBase.h"

/// <summary>
/// This constraint forbids one tile to be adjacent to another
/// </summary>
class SimpleAdjacencyConstraint : public TileConstraintBase
{
public:
	SimpleAdjacencyConstraint(const std::shared_ptr<TileBase> tile, const std::shared_ptr<TileBase> other)
		: m_tile(tile), m_other(other)
	{};

	// Inherited via TileConstraintBase
	bool IsConstraintFollowed(const TileConstraintArgs& args) override;

private:
	const std::shared_ptr<TileBase> m_tile, m_other;

};