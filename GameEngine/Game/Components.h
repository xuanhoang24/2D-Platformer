#ifndef COMPONENTS_H
#define COMPONENTS_H

#include "Entity.h"
#include "../Graphics/AnimatedSpriteLoader.h"
#include <functional>

// World position, size, and scale
struct TransformComponent : Component
{
    float worldX = 0, worldY = 0;   // Current world position
    float baseX = 0, baseY = 0;     // Original spawn position (for reset)
    float width = 16, height = 16;
    float scale = 1;
    int mapInstance = 0;            // Which map chunk instance this belongs to
};

// Sprite rendering and animation
struct SpriteComponent : Component
{
    AnimatedSpriteLoader* animLoader = nullptr;
    string currentAnimation = "idle";
    bool facingRight = true;
    bool visible = true;
    bool flickering = false;
    int flickerCounter = 0;
    ~SpriteComponent() override { delete animLoader; }
};

// Movement velocity and speed settings
struct MovementComponent : Component
{
    float velocityX = 0, velocityY = 0;
    float walkSpeed = 150, runSpeed = 250;   // Player speeds
    float moveSpeed = 50;                    // AI movement speed
    float direction = 1;                     // 1 = right, -1 = left
};

// Gravity and ground detection
struct PhysicsComponent : Component
{
    float gravity = 980;
    bool isGrounded = false;
    bool useGravity = true;
};

// Jump mechanics with coyote time
struct JumpComponent : Component
{
    bool jumpPressed = false;
    bool isJumping = false;
    float jumpForce = -300;
    float jumpHoldForce = -400;     // Extra force while holding jump
    float jumpMaxHoldTime = 0.5f;
    float jumpHoldTimer = 0;
    float coyoteTime = 0.12f;       // Grace period after leaving ground
    float coyoteTimer = 0;
};

enum class ColliderType { Player, Enemy, Coin, Obstacle };

// Collision detection box
struct CollisionComponent : Component
{
    float offsetX = 0, offsetY = 0;         // Offset from transform position
    float boxWidth = 16, boxHeight = 16;
    ColliderType type = ColliderType::Obstacle;
    bool isTrigger = false;                 // Triggers don't block movement
};

// Health, damage, and death state
struct HealthComponent : Component
{
    int health = 3, maxHealth = 3;
    float invincibleTimer = 0;
    float invincibleDuration = 1.5f;
    bool isInvincible = false;
    bool isDead = false;
    bool isFullyDead = false;       // After death animation completes
    float deathTimer = 0;
    float deathDuration = 1;
};

// AI patrol boundaries
struct PatrolComponent : Component
{
    float leftBoundary = 0, rightBoundary = 0;
    float baseLeftBoundary = 0, baseRightBoundary = 0;  // Original bounds
};

enum class CollectibleType { Coin1, Coin2, Diamond };

// Collectible item data
struct CollectibleComponent : Component
{
    CollectibleType type = CollectibleType::Coin1;
    int pointValue = 10;
    bool collected = false;
};

enum class EnemyVariant { Ghost, Mushroom };

// Enemy type identifier
struct EnemyComponent : Component
{
    EnemyVariant variant = EnemyVariant::Ghost;
    bool destroyed = false;
};

// Tag component to identify player entity
struct PlayerTag : Component {};

// Tag component for entities that receive input 
struct InputComponent : Component {};

// Dash ability
struct DashComponent : Component
{
    bool dashPressed = false;
    bool isDashing = false;
    float dashSpeed = 500;          // Speed during dash
    float dashDuration = 0.15f;     // How long dash lasts
    float dashTimer = 0;
    float dashCooldown = 0.5f;      // Time between dashes
    float cooldownTimer = 0;
};

// Punch attack ability
struct PunchComponent : Component
{
    bool punchPressed = false;
    bool isPunching = false;
    float punchDuration = 0.3f;     // How long punch animation lasts
    float punchTimer = 0;
    float punchRange = 25;          // Hit enemies within 25 pixels
    bool hasHit = false;            // Prevent multiple hits per punch
};

// Infinite scroll repositioning
struct ScrollableComponent : Component
{
    bool shouldReposition = true;
    std::function<void(Entity*)> onReposition;  // Callback when repositioned
};

#endif // COMPONENTS_H
