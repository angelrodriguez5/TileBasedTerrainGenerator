#pragma once
#include "Interfaces/TileBase.h"
#include <memory>

struct TileConstraintArgs
{
	std::shared_ptr<TileBase> tile, other;

	TileConstraintArgs() {};
};

class TileConstraintBase
{
public:
	TileConstraintBase() = default;

	virtual bool IsConstraintFollowed(const TileConstraintArgs &args) = 0;

};