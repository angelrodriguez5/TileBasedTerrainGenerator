#pragma once

class TileBase
{
public:
	TileBase() : tileID(0) {};
	TileBase(uint32_t tileID) : tileID(tileID) {};

	uint32_t tileID;

	bool operator==(const TileBase& other) const
	{
		return tileID == other.tileID;
	}
};
