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

	// Class static id. Copy to all subclasses to be able to identify them
	static const void* StaticTypeId()
	{
		static int id;
		return &id;
	}

	const void* GetTypeId() const override
	{
		return StaticTypeId();
	}

	bool operator==(const TileConstraintBase& other) const override
	{
		if (GetTypeId() != other.GetTypeId())
			return false;

		SimpleAdjacencyConstraint* otherConstraint = (SimpleAdjacencyConstraint*)&other;

		return (*m_tile == *otherConstraint->m_tile && *m_other == *otherConstraint->m_other)
			|| (*m_tile == *otherConstraint->m_other && *m_other == *otherConstraint->m_tile);
	}

private:
	const TileBase* m_tile;
	const TileBase* m_other;

};