#ifndef SPATIALGRID_H
#define SPATIALGRID_H

#include "Entity.h"
#include "Components.h"
#include <vector>
#include <unordered_map>
#include <unordered_set>

/**
 * Spatial partitioning grid for O(n) collision detection.
 * 
 * Divides the world into cells. Entities are assigned to cells based on their AABB.
 * Collision checks only occur between entities in the same or adjacent cells.
 * 
 * Two-phase collision:
 * 1. Broad-phase: AABB overlap test (fast rejection)
 * 2. Narrow-phase: Detailed collision (pixel-perfect if needed)
 */
class SpatialGrid
{
public:
    SpatialGrid(int _cellSize = 64);
    
    // Clear all entities from the grid
    void Clear();
    
    // Insert an entity into the grid based on its position and collision box
    void Insert(Entity* _entity);
    
    // Update entity position in grid (call after movement)
    void Update(Entity* _entity);
    
    // Remove entity from grid
    void Remove(Entity* _entity);
    
    // Get all cells that are likely to collide with an entity (same type + adjacent cells)    
    std::vector<Entity*> GetNearbyEntities(Entity* _entity) const;
    
    // Get entities in a specific world region
    std::vector<Entity*> GetEntitiesInRegion(float _x, float _y, float _width, float _height) const;
    
    // Broad-phase AABB collision test
    static bool AABBOverlap(Entity* _a, Entity* _b);
    
    // Broad-phase AABB test with raw coordinates
    static bool AABBOverlap(float _ax, float _ay, float _aw, float _ah,
                            float _bx, float _by, float _bw, float _bh);

private:
    // Hash function for cell coordinates
    struct CellHash
    {
        size_t operator()(const std::pair<int, int>& _cell) const
        {
            return std::hash<int>()(_cell.first) ^ (std::hash<int>()(_cell.second) << 16);
        }
    };
    
    // Get cell coordinates for a world position
    std::pair<int, int> GetCell(float _x, float _y) const;
    
    // Get all cells that an AABB overlaps
    std::vector<std::pair<int, int>> GetCellsForAABB(float _x, float _y, float _w, float _h) const;
    
    int m_cellSize;
    
    // Map from cell coordinates to entities in that cell
    std::unordered_map<std::pair<int, int>, std::vector<Entity*>, CellHash> m_cells;
    
    // Track which cells each entity occupies (for fast removal/update)
    std::unordered_map<EntityID, std::vector<std::pair<int, int>>> m_entityCells;
};

#endif // SPATIALGRID_H
