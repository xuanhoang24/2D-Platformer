#include "SpatialGrid.h"
#include "../Graphics/Renderer.h"
#include "../Graphics/Camera.h"
#include <algorithm>

SpatialGrid::SpatialGrid(int _cellSize)
    : m_cellSize(_cellSize)
{
}

void SpatialGrid::Clear()
{
    m_cells.clear();
    m_entityCells.clear();
}

std::pair<int, int> SpatialGrid::GetCell(float _x, float _y) const
{
    return { static_cast<int>(floor(_x / m_cellSize)), 
             static_cast<int>(floor(_y / m_cellSize)) };
}

std::vector<std::pair<int, int>> SpatialGrid::GetCellsForAABB(float _x, float _y, float _w, float _h) const
{
    std::vector<std::pair<int, int>> cells;
    
    int minCellX = static_cast<int>(floor(_x / m_cellSize));
    int maxCellX = static_cast<int>(floor((_x + _w) / m_cellSize));
    int minCellY = static_cast<int>(floor(_y / m_cellSize));
    int maxCellY = static_cast<int>(floor((_y + _h) / m_cellSize));
    
    for (int cx = minCellX; cx <= maxCellX; ++cx)
    {
        for (int cy = minCellY; cy <= maxCellY; ++cy)
        {
            cells.push_back({ cx, cy });
        }
    }
    
    return cells;
}

void SpatialGrid::Insert(Entity* _entity)
{
    if (!_entity || !_entity->IsActive()) return;
    
    auto* transform = _entity->GetComponent<TransformComponent>();
    auto* collision = _entity->GetComponent<CollisionComponent>();
    if (!transform || !collision) return;
    
    float x = transform->worldX + collision->offsetX;
    float y = transform->worldY + collision->offsetY;
    float w = collision->boxWidth;
    float h = collision->boxHeight;
    
    auto cells = GetCellsForAABB(x, y, w, h);
    EntityID id = _entity->GetID();
    
    for (const auto& cell : cells)
    {
        m_cells[cell].push_back(_entity);
    }
    
    m_entityCells[id] = cells;
}

void SpatialGrid::Remove(Entity* _entity)
{
    if (!_entity) return;
    
    EntityID id = _entity->GetID();
    auto it = m_entityCells.find(id);
    if (it == m_entityCells.end()) return;
    
    for (const auto& cell : it->second)
    {
        auto cellIt = m_cells.find(cell);
        if (cellIt != m_cells.end())
        {
            auto& entities = cellIt->second;
            entities.erase(std::remove(entities.begin(), entities.end(), _entity), entities.end());
            if (entities.empty())
            {
                m_cells.erase(cellIt);
            }
        }
    }
    
    m_entityCells.erase(it);
}

void SpatialGrid::Update(Entity* _entity)
{
    Remove(_entity);
    Insert(_entity);
}

std::vector<Entity*> SpatialGrid::GetNearbyEntities(Entity* _entity) const
{
    std::unordered_set<Entity*> nearby;
    
    if (!_entity) return {};
    
    auto* transform = _entity->GetComponent<TransformComponent>();
    auto* collision = _entity->GetComponent<CollisionComponent>();
    if (!transform || !collision) return {};
    
    float x = transform->worldX + collision->offsetX;
    float y = transform->worldY + collision->offsetY;
    float w = collision->boxWidth;
    float h = collision->boxHeight;
    
    // Get cells for this entity's AABB plus adjacent cells
    int minCellX = static_cast<int>(floor(x / m_cellSize)) - 1;
    int maxCellX = static_cast<int>(floor((x + w) / m_cellSize)) + 1;
    int minCellY = static_cast<int>(floor(y / m_cellSize)) - 1;
    int maxCellY = static_cast<int>(floor((y + h) / m_cellSize)) + 1;
    
    for (int cx = minCellX; cx <= maxCellX; ++cx)
    {
        for (int cy = minCellY; cy <= maxCellY; ++cy)
        {
            auto it = m_cells.find({ cx, cy });
            if (it != m_cells.end())
            {
                for (Entity* e : it->second)
                {
                    if (e != _entity && e->IsActive())
                    {
                        nearby.insert(e);
                    }
                }
            }
        }
    }
    
    return std::vector<Entity*>(nearby.begin(), nearby.end());
}

std::vector<Entity*> SpatialGrid::GetEntitiesInRegion(float _x, float _y, float _width, float _height) const
{
    std::unordered_set<Entity*> result;
    
    auto cells = GetCellsForAABB(_x, _y, _width, _height);
    
    for (const auto& cell : cells)
    {
        auto it = m_cells.find(cell);
        if (it != m_cells.end())
        {
            for (Entity* e : it->second)
            {
                if (e->IsActive())
                {
                    result.insert(e);
                }
            }
        }
    }
    
    return std::vector<Entity*>(result.begin(), result.end());
}

bool SpatialGrid::AABBOverlap(Entity* _a, Entity* _b)
{
    if (!_a || !_b) return false;
    
    auto* ta = _a->GetComponent<TransformComponent>();
    auto* ca = _a->GetComponent<CollisionComponent>();
    auto* tb = _b->GetComponent<TransformComponent>();
    auto* cb = _b->GetComponent<CollisionComponent>();
    
    if (!ta || !ca || !tb || !cb) return false;
    
    return AABBOverlap(
        ta->worldX + ca->offsetX, ta->worldY + ca->offsetY, ca->boxWidth, ca->boxHeight,
        tb->worldX + cb->offsetX, tb->worldY + cb->offsetY, cb->boxWidth, cb->boxHeight
    );
}

bool SpatialGrid::AABBOverlap(float _ax, float _ay, float _aw, float _ah,
                              float _bx, float _by, float _bw, float _bh)
{
    return _ax < _bx + _bw && _ax + _aw > _bx &&
           _ay < _by + _bh && _ay + _ah > _by;
}

void SpatialGrid::RenderDebug(Renderer* _renderer, Camera* _camera, float _viewportWidth, float _viewportHeight) const
{
    if (!_renderer || !_camera) return;
    
    // Calculate visible cell range based on camera position
    float cameraX = _camera->GetX();
    float cameraY = _camera->GetY();
    
    int minCellX = static_cast<int>(floor(cameraX / m_cellSize)) - 1;
    int maxCellX = static_cast<int>(floor((cameraX + _viewportWidth) / m_cellSize)) + 1;
    int minCellY = static_cast<int>(floor(cameraY / m_cellSize)) - 1;
    int maxCellY = static_cast<int>(floor((cameraY + _viewportHeight) / m_cellSize)) + 1;
    
    // Clamp Y to reasonable range
    if (minCellY < -5) minCellY = -5;
    if (maxCellY > 20) maxCellY = 20;
    
    for (int cx = minCellX; cx <= maxCellX; ++cx)
    {
        for (int cy = minCellY; cy <= maxCellY; ++cy)
        {
            // Calculate world position of cell
            float worldX = cx * m_cellSize;
            float worldY = cy * m_cellSize;
            
            // Convert to screen coordinates
            float screenX = _camera->WorldToScreenX(worldX);
            float screenY = worldY;
            
            // Check if cell has entities
            auto it = m_cells.find({ cx, cy });
            bool hasEntities = (it != m_cells.end() && !it->second.empty());
            
            // Green for has entities, red for empty
            if (hasEntities)
                _renderer->SetDrawColor(Color(0, 255, 0, 255));  // Green
            else
                _renderer->SetDrawColor(Color(255, 0, 0, 128));  // Red
            
            // Draw cell outline
            Rect cellRect(
                static_cast<unsigned int>(screenX),
                static_cast<unsigned int>(screenY),
                static_cast<unsigned int>(screenX + m_cellSize),
                static_cast<unsigned int>(screenY + m_cellSize)
            );
            _renderer->RenderRectangle(cellRect);
        }
    }
}
