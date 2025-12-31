#ifndef SYSTEMS_H
#define SYSTEMS_H

#include "Entity.h"
#include "Components.h"
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
    virtual void Update(std::vector<Entity*>& entities, float deltaTime) {}
    virtual void Render(std::vector<Entity*>& entities, Renderer* renderer, Camera* camera) {}
};

// Reads keyboard input and sets player velocity
class InputSystem : public System
{
public:
    void Update(std::vector<Entity*>& entities, float deltaTime) override;
};

// Applies gravity to entities with PhysicsComponent
class PhysicsSystem : public System
{
public:
    void Update(std::vector<Entity*>& entities, float deltaTime) override;
};

// Handles jump input, coyote time, and variable jump height
class JumpSystem : public System
{
public:
    void Update(std::vector<Entity*>& entities, float deltaTime) override;
};

// Handles dash ability triggered by shift key
class DashSystem : public System
{
public:
    void Update(std::vector<Entity*>& entities, float deltaTime) override;
};

// Handles punch attack - kills enemies in front of player
class PunchSystem : public System
{
public:
    void Update(std::vector<Entity*>& entities, float deltaTime) override;
};

// Applies velocity to position
class MovementSystem : public System
{
public:
    void Update(std::vector<Entity*>& entities, float deltaTime) override;
};

// Handles collision between player and world tiles
class CollisionSystem : public System
{
public:
    void SetChunkMap(ChunkMap* map) { m_chunkMap = map; }
    void Update(std::vector<Entity*>& entities, float deltaTime) override;
private:
    void HandlePlayerCollision(Entity* entity);
    ChunkMap* m_chunkMap = nullptr;
};

// Moves enemies back and forth within patrol boundaries
class PatrolSystem : public System
{
public:
    void SetMapWidth(int width) { m_mapWidth = width; }
    void Update(std::vector<Entity*>& entities, float deltaTime) override;
private:
    int m_mapWidth = 0;
};

// Repositions entities for infinite scrolling
class ScrollSystem : public System
{
public:
    void SetParams(float cameraX, int screenWidth, int mapWidth);
    void Update(std::vector<Entity*>& entities, float deltaTime) override;
private:
    float m_cameraX = 0;
    int m_screenWidth = 0;
    int m_mapWidth = 0;
};

// Updates invincibility timer and death state
class HealthSystem : public System
{
public:
    void Update(std::vector<Entity*>& entities, float deltaTime) override;
};

// Handles player vs enemy/coin collisions
class EntityCollisionSystem : public System
{
public:
    void Update(std::vector<Entity*>& entities, float deltaTime) override;
    int GetScore() const { return m_score; }
    void ResetScore() { m_score = 0; }
private:
    int m_score = 0;
};

// Updates sprite animation based on entity state
class AnimationSystem : public System
{
public:
    void Update(std::vector<Entity*>& entities, float deltaTime) override;
};

// Renders all visible sprites
class RenderSystem : public System
{
public:
    void Render(std::vector<Entity*>& entities, Renderer* renderer, Camera* camera) override;
};

#endif
