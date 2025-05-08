#include "Generator/Strategies.h"
#include "Generator/VectorUtils.h"
#include <random>

/// <summary>
/// Returns a random tile in the cell's superposition
/// </summary>
/// <returns>If no tile can be chosen, it should return a nullptr</returns>
TileBase* RandomTileStrategy(TileMapBase& map, const CellIdx& cell)
{
    auto tiles = map.GetSuperpositionAt(cell);
    if (tiles.size() == 0)
        return nullptr;

    std::random_device random_device;
    std::mt19937 engine{ random_device() };
    std::uniform_int_distribution<int> dist(0, tiles.size() - 1);

    return std::move(tiles[dist(engine)]);
}

/// <summary>
/// Returns a random tile based on neighboring tiles, the probability of choosing a certain
/// tile increases if there are neighboring collapsed cells that contain it
/// </summary>
/// <returns>If no tile can be chosen, it should return a nullptr</returns>
TileBase* SimilarToNeighborTileStrategy(TileMapBase& map, const CellIdx& cell)
{
    auto tiles = map.GetSuperpositionAt(cell);
    if (tiles.size() == 0)
        return nullptr;

    auto neighbors = map.GetNeighbors(cell);

    for (auto neighbor : neighbors)
    {
        if (map.IsCellCollapsed(neighbor))
        {
            auto neighborTile = map.GetSuperpositionAt(neighbor)[0];

            // Add a duplicate of the collapsed tile to the superposition
            // increasing the chance to pick it
            if (IsInVector(tiles, neighborTile))
                tiles.push_back(neighborTile);
        }
    }

    std::random_device random_device;
    std::mt19937 engine{ random_device() };
    std::uniform_int_distribution<int> dist(0, tiles.size() - 1);

    return std::move(tiles[dist(engine)]);

}

CellIdx LowestEntropyCellStrategy(TileMapBase& map)
{
    CellIdx cell, chosenCell;
    int minEntropy = std::_Max_int_dig;

    for (int i = 0; i < map.GetHeight(); i++)
    {
        cell.y = i;
        for (int j = 0; j < map.GetWidth(); j++)
        {
            cell.x = j;

            // Collapsed cells don't count
            if (map.IsCellCollapsed(cell))
                continue;

            int cellEntropy = map.GetSuperpositionAt(cell).size();
            if (cellEntropy < minEntropy)
                chosenCell = cell;
        }
    }

    return chosenCell;
}

CellIdx RandomUncollapsedCellStrategy(TileMapBase& map)
{
    std::random_device random_device;
    std::mt19937 engine{ random_device() };

    // Try to get a random uncollapsed cell
    int sx, sy;
    int triesAtRandom = 20;
    for (int i = 0; i < triesAtRandom; i++)
    {
        sx = std::uniform_int_distribution<int>{ 0, map.GetWidth() - 1 }(engine);
        sy = std::uniform_int_distribution<int>{ 0, map.GetHeight() - 1 }(engine);

        CellIdx cell(sx, sy);
        if (!map.IsCellCollapsed(cell))
            return cell;
    }

    // As more cells are collapsed, the above is less likely to succeed
    // to ensure every cell is picked, make a list of uncollapsed cells and return a random one
    std::vector<CellIdx> uncollapsed;
    for (int i = 0; i < map.GetHeight(); i++)
    {
        for (int j = 0; j < map.GetWidth(); j++)
        {
            CellIdx cell(j, i);
            if (!map.IsCellCollapsed(cell))
                uncollapsed.push_back(cell);
        }
    }

    if (uncollapsed.size() > 0)
        return uncollapsed[std::uniform_int_distribution<size_t>{ 0, uncollapsed.size() - 1 }(engine)];

    throw std::exception("RandomUncollapsedCellStrategy: all map cells are collapsed");
}
