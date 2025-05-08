


#include "Unreal/Data/TileAsset.h"


UTileAsset::UTileAsset()
{
	tileID = GetTypeHash(GetPrimaryAssetId());
	tileActor = nullptr;
}

UTileAsset::~UTileAsset()
{
}
