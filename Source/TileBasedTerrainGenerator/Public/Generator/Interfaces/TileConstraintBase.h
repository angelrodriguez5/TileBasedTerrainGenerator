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

	virtual const void* GetTypeId() const = 0;

	virtual bool operator==(const TileConstraintBase& other) const = 0;

};