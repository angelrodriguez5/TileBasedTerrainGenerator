


#include "Unreal/Data/TileAsset.h"


UTileAsset::UTileAsset()
{
	tileID = GetTypeHash(GetPrimaryAssetId());
}

UTileAsset::~UTileAsset()
{
}
