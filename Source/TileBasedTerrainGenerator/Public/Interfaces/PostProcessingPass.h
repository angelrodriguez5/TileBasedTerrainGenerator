#pragma once
#include <memory>
#include "Interfaces/TileMapBase.h"

class PostProcessingPassBase
{
public:
	virtual void PostProcessMap(std::shared_ptr<TileMapBase> map) = 0;
};