#include "Generator/Implementations/SimpleAdjacencyConstraint.h"

bool SimpleAdjacencyConstraint::IsConstraintFollowed(const TileConstraintArgs& args)
{
    // Check that args have tha same pair of tiles as this constraint
    if (   (*(args.tile) == *m_tile && *(args.other) == *m_other)
        || (*(args.tile) == *m_other && *(args.other) == *m_tile))
        return false;
    
    // If both tiles are in args then they cannot be adjacent
    return true;
}
