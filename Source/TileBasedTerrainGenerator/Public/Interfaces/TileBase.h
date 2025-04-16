#pragma once

class TileBase
{
public:
	TileBase(int id) : id(id) {};

	int id;

	virtual void Print() {};

	bool operator==(const TileBase& other) const
	{
		return id == other.id;
	}
};

class TestConsoleTile : public TileBase
{
public:
	TestConsoleTile(int id, char representation)
		: TileBase(id)
		, representation(representation)
	{}

	char representation;

	void Print() override;
};