#ifndef SYSTEMS_H
#define SYSTEMS_H

#include "Entity.h"
#include "Components.h"
#include "SpatialGrid.h"
#include <vector>

class Renderer;
class Camera;
class ChunkMap;

/**
 * Systems contain all game logic.
 * 
 * Each system operates on entities that have specific components.
 * Ex: MovementSystem updates entities with Transform + Movement components.
 */

class System
{
public:
    virtual ~System() = default;
    virtual void Update(std::vector<Entity*>& _entities, float _deltaTime) {}
    virtual void Render(std::vector<Entity*>& _entities, Renderer* _renderer, Camera* _camera) {}
};

// Reads keyboard input and sets player velocity
class InputSystem : public System
{
public:
    void Update(std::vector<Entity*>& _entities, float _deltaTime) override;
};

// Applies gravity to entities with PhysicsComponent
class PhysicsSystem : public System
{
public:
    void Update(std::vector<Entity*>& _entities, float _deltaTime) override;
};

// Handles jump input, coyote time, and variable jump height
class JumpSystem : public System
{
public:
    void Update(std::vector<Entity*>& _entities, float _deltaTime) override;
};

// Handles dash ability triggered by shift key
class DashSystem : public System
{
public:
    void Update(std::vector<Entity*>& _entities, float _deltaTime) override;
};

// Handles punch attack - kills enemies in front of player
class PunchSystem : public System
{
public:
    void Update(std::vector<Entity*>& _entities, float _deltaTime) override;
};

// Applies velocity to position
class MovementSystem : public System
{
public:
    void Update(std::vector<Entity*>& _entities, float _deltaTime) override;
};

// Handles collision between player and world tiles
class CollisionSystem : public System
{
public:
    void SetChunkMap(ChunkMap* _map) { m_chunkMap = _map; }
    void Update(std::vector<Entity*>& _entities, float _deltaTime) override;
private:
    void HandlePlayerCollision(Entity* _entity);
    ChunkMap* m_chunkMap = nullptr;
};

// Moves enemies back and forth within patrol boundaries
class PatrolSystem : public System
{
public:
    void SetMapWidth(int _width) { m_mapWidth = _width; }
    void Update(std::vector<Entity*>& _entities, float _deltaTime) override;
private:
    int m_mapWidth = 0;
};

// Repositions entities for infinite scrolling
class ScrollSystem : public System
{
public:
    void SetParams(float _cameraX, int _screenWidth, int _mapWidth);
    void Update(std::vector<Entity*>& _entities, float _deltaTime) override;
private:
    float m_cameraX = 0;
    int m_screenWidth = 0;
    int m_mapWidth = 0;
};

// Updates invincibility timer and death state
class HealthSystem : public System
{
public:
    void Update(std::vector<Entity*>& _entities, float _deltaTime) override;
};

// Handles player vs enemy/coin collisions using spatial partitioning
// Uses grid-based broad-phase AABB tests before narrow-phase detection
class EntityCollisionSystem : public System
{
public:
    EntityCollisionSystem(int _cellSize = 64);
    
    // Rebuild spatial grid with all entities (call when entities added/removed)
    void RebuildGrid(std::vector<Entity*>& _entities);
    
    // Update entity position in grid (call after movement)
    void UpdateEntityInGrid(Entity* _entity);
    
    void Update(std::vector<Entity*>& _entities, float _deltaTime) override;
    int GetScore() const { return m_score; }
    void ResetScore() { m_score = 0; }
    
    // Stats for debugging
    int GetLastBroadPhaseChecks() const { return m_lastBroadPhaseChecks; }
    int GetLastNarrowPhaseChecks() const { return m_lastNarrowPhaseChecks; }
    
private:
    SpatialGrid m_spatialGrid;
    int m_score = 0;
    int m_lastBroadPhaseChecks = 0;
    int m_lastNarrowPhaseChecks = 0;
};

// Updates sprite animation based on entity state
class AnimationSystem : public System
{
public:
    void Update(std::vector<Entity*>& _entities, float _deltaTime) override;
};

// Renders all visible sprites
class RenderSystem : public System
{
public:
    void Render(std::vector<Entity*>& _entities, Renderer* _renderer, Camera* _camera) override;
};

#endif
