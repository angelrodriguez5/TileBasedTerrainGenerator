#pragma once
#include "Generator/Interfaces/TileBase.h"
#include <memory>

struct TileConstraintArgs
{
	TileBase* tile;
	TileBase* other;
};

class TileConstraintBase
{
public:
	TileConstraintBase() = default;

	virtual bool IsConstraintFollowed(const TileConstraintArgs &args) = 0;

};