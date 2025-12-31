#include "Systems.h"
#include "ChunkMap.h"
#include "../Graphics/Renderer.h"
#include "../Graphics/Camera.h"
#include "../Core/Timing.h"

void InputSystem::Update(std::vector<Entity*>& entities, float deltaTime)
{
    const Uint8* keys = SDL_GetKeyboardState(NULL);
    for (auto* entity : entities)
    {
        if (!entity || !entity->IsActive() || !entity->HasComponent<PlayerTag>()) continue;
        auto* movement = entity->GetComponent<MovementComponent>();
        auto* sprite = entity->GetComponent<SpriteComponent>();
        auto* jump = entity->GetComponent<JumpComponent>();
        auto* dash = entity->GetComponent<DashComponent>();
        auto* punch = entity->GetComponent<PunchComponent>();
        if (!movement) continue;

        bool shift = keys[SDL_SCANCODE_LSHIFT] || keys[SDL_SCANCODE_RSHIFT];
        
        // Dash - shift press
        if (dash)
        {
            static bool prevShift = false;
            bool shiftJustPressed = shift && !prevShift;
            prevShift = shift;
            
            if (shiftJustPressed && !dash->isDashing && dash->cooldownTimer <= 0)
            {
                dash->dashPressed = true;
            }
        }

        // Punch input - Left mouse click
        if (punch && !punch->isPunching)
        {
            static bool prevClick = false;
            Uint32 mouseState = SDL_GetMouseState(NULL, NULL);
            bool leftClick = (mouseState & SDL_BUTTON(SDL_BUTTON_LEFT)) != 0;
            bool clickJustPressed = leftClick && !prevClick;
            prevClick = leftClick;
            
            if (clickJustPressed)
            {
                punch->punchPressed = true;
            }
        }

        // Don't allow normal movement control during dash or punch
        if ((dash && dash->isDashing) || (punch && punch->isPunching)) continue;

        float speed = movement->walkSpeed;

        if (keys[SDL_SCANCODE_A]) { movement->velocityX = -speed; if (sprite) sprite->facingRight = false; }
        else if (keys[SDL_SCANCODE_D]) { movement->velocityX = speed; if (sprite) sprite->facingRight = true; }
        else movement->velocityX = 0;

        if (jump) jump->jumpPressed = keys[SDL_SCANCODE_SPACE];
    }
}

void PhysicsSystem::Update(std::vector<Entity*>& entities, float deltaTime)
{
    for (auto* entity : entities)
    {
        if (!entity || !entity->IsActive()) continue;
        auto* movement = entity->GetComponent<MovementComponent>();
        auto* physics = entity->GetComponent<PhysicsComponent>();
        if (movement && physics && physics->useGravity)
            movement->velocityY += physics->gravity * deltaTime;
    }
}

void JumpSystem::Update(std::vector<Entity*>& entities, float deltaTime)
{
    for (auto* entity : entities)
    {
        if (!entity || !entity->IsActive()) continue;
        auto* movement = entity->GetComponent<MovementComponent>();
        auto* physics = entity->GetComponent<PhysicsComponent>();
        auto* jump = entity->GetComponent<JumpComponent>();
        if (!movement || !physics || !jump) continue;

        jump->coyoteTimer = physics->isGrounded ? jump->coyoteTime : jump->coyoteTimer - deltaTime;

        if (jump->jumpPressed)
        {
            if (!jump->isJumping && (physics->isGrounded || jump->coyoteTimer > 0))
            {
                jump->isJumping = true;
                physics->isGrounded = false;
                movement->velocityY = jump->jumpForce;
                jump->jumpHoldTimer = jump->jumpMaxHoldTime;
            }
            if (jump->isJumping && jump->jumpHoldTimer > 0)
            {
                movement->velocityY += jump->jumpHoldForce * deltaTime;
                jump->jumpHoldTimer -= deltaTime;
            }
        }
    }
}

void DashSystem::Update(std::vector<Entity*>& entities, float deltaTime)
{
    for (auto* entity : entities)
    {
        if (!entity || !entity->IsActive()) continue;
        auto* movement = entity->GetComponent<MovementComponent>();
        auto* sprite = entity->GetComponent<SpriteComponent>();
        auto* dash = entity->GetComponent<DashComponent>();
        if (!movement || !dash) continue;

        // Update cooldown timer
        if (dash->cooldownTimer > 0)
            dash->cooldownTimer -= deltaTime;

        // Start dash
        if (dash->dashPressed && !dash->isDashing && dash->cooldownTimer <= 0)
        {
            dash->isDashing = true;
            dash->dashTimer = dash->dashDuration;
            dash->dashPressed = false;
            
            // Dash in facing direction
            float direction = (sprite && !sprite->facingRight) ? -1.0f : 1.0f;
            movement->velocityX = dash->dashSpeed * direction;
            movement->velocityY = 0;  // Cancel vertical movement during dash
        }

        // Update dash
        if (dash->isDashing)
        {
            dash->dashTimer -= deltaTime;
            if (dash->dashTimer <= 0)
            {
                dash->isDashing = false;
                dash->cooldownTimer = dash->dashCooldown;
                movement->velocityX = 0;
            }
        }
        
        dash->dashPressed = false;
    }
}

void PunchSystem::Update(std::vector<Entity*>& entities, float deltaTime)
{
    // Find player
    Entity* player = nullptr;
    for (auto* entity : entities)
        if (entity && entity->IsActive() && entity->HasComponent<PlayerTag>()) { player = entity; break; }
    if (!player) return;

    auto* punch = player->GetComponent<PunchComponent>();
    auto* transform = player->GetComponent<TransformComponent>();
    auto* sprite = player->GetComponent<SpriteComponent>();
    auto* movement = player->GetComponent<MovementComponent>();
    if (!punch || !transform) return;

    // Start punch
    if (punch->punchPressed && !punch->isPunching)
    {
        punch->isPunching = true;
        punch->punchTimer = punch->punchDuration;
        punch->hasHit = false;
        punch->punchPressed = false;
        
        // Stop movement during punch
        if (movement) movement->velocityX = 0;
    }

    // Update punch
    if (punch->isPunching)
    {
        punch->punchTimer -= deltaTime;
        
        // Check for enemy hits (only once per punch)
        if (!punch->hasHit)
        {
            float playerCenterX = transform->worldX + transform->width / 2;
            float playerCenterY = transform->worldY + transform->height / 2;
            float direction = (sprite && !sprite->facingRight) ? -1.0f : 1.0f;
            
            for (auto* entity : entities)
            {
                if (!entity || !entity->IsActive() || entity == player) continue;
                auto* enemy = entity->GetComponent<EnemyComponent>();
                if (!enemy || enemy->destroyed) continue;
                
                auto* enemyTransform = entity->GetComponent<TransformComponent>();
                if (!enemyTransform) continue;
                
                float enemyCenterX = enemyTransform->worldX + enemyTransform->width / 2;
                float enemyCenterY = enemyTransform->worldY + enemyTransform->height / 2;
                
                // Check if enemy is in facing direction
                float dx = enemyCenterX - playerCenterX;
                if ((direction > 0 && dx < 0) || (direction < 0 && dx > 0)) continue;
                
                float dy = enemyCenterY - playerCenterY;
                float dist = sqrt(dx * dx + dy * dy);
                
                // Only hit if within 5 pixels
                if (dist <= punch->punchRange)
                {
                    enemy->destroyed = true;
                    entity->SetActive(false);
                    punch->hasHit = true;
                    break;
                }
            }
        }
        
        if (punch->punchTimer <= 0)
        {
            punch->isPunching = false;
        }
    }
    
    punch->punchPressed = false;
}

void MovementSystem::Update(std::vector<Entity*>& entities, float deltaTime)
{
    for (auto* entity : entities)
    {
        if (!entity || !entity->IsActive()) continue;
        auto* transform = entity->GetComponent<TransformComponent>();
        auto* movement = entity->GetComponent<MovementComponent>();
        if (transform && movement)
        {
            transform->worldX += movement->velocityX * deltaTime;
            transform->worldY += movement->velocityY * deltaTime;
        }
    }
}

void CollisionSystem::Update(std::vector<Entity*>& entities, float deltaTime)
{
    if (!m_chunkMap) return;
    for (auto* entity : entities)
    {
        if (!entity || !entity->IsActive()) continue;
        auto* collision = entity->GetComponent<CollisionComponent>();
        if (collision && !collision->isTrigger && collision->type == ColliderType::Player)
            HandlePlayerCollision(entity);
    }
}

void CollisionSystem::HandlePlayerCollision(Entity* entity)
{
    auto* transform = entity->GetComponent<TransformComponent>();
    auto* movement = entity->GetComponent<MovementComponent>();
    auto* physics = entity->GetComponent<PhysicsComponent>();
    auto* collision = entity->GetComponent<CollisionComponent>();
    auto* jump = entity->GetComponent<JumpComponent>();
    if (!transform || !movement || !collision) return;

    float colX = transform->worldX + collision->offsetX;
    float colY = transform->worldY + collision->offsetY;
    float colWidth = collision->boxWidth;
    float colHeight = collision->boxHeight;

    // Horizontal
    float wallX;
    if (movement->velocityX > 0 && m_chunkMap->CheckCollisionLeft(colX, colY, colWidth, colHeight, wallX))
    {
        transform->worldX = wallX - colWidth - collision->offsetX;
        movement->velocityX = 0;
    }
    else if (movement->velocityX < 0 && m_chunkMap->CheckCollisionRight(colX, colY, colWidth, colHeight, wallX))
    {
        transform->worldX = wallX - collision->offsetX;
        movement->velocityX = 0;
    }

    colX = transform->worldX + collision->offsetX;
    colY = transform->worldY + collision->offsetY;
    if (physics) physics->isGrounded = false;

    // Vertical
    float groundY;
    if (movement->velocityY >= 0)
    {
        if (m_chunkMap->CheckCollisionTop(colX, colY, colWidth, colHeight, groundY))
        {
            transform->worldY = groundY - colHeight - collision->offsetY;
            movement->velocityY = 0;
            if (physics) physics->isGrounded = true;
            if (jump) jump->isJumping = false;
        }
        else if (transform->worldY + transform->height >= 500)
        {
            transform->worldY = 500 - transform->height;
            movement->velocityY = 0;
            if (physics) physics->isGrounded = true;
            if (jump) jump->isJumping = false;
        }
    }
    else
    {
        float ceilY;
        if (m_chunkMap->CheckCollisionBottom(colX, colY, colWidth, colHeight, ceilY))
        {
            transform->worldY = ceilY - collision->offsetY;
            movement->velocityY = 0;
            if (jump) jump->jumpHoldTimer = 0;
        }
    }
}

void PatrolSystem::Update(std::vector<Entity*>& entities, float deltaTime)
{
    for (auto* entity : entities)
    {
        if (!entity || !entity->IsActive()) continue;
        auto* transform = entity->GetComponent<TransformComponent>();
        auto* movement = entity->GetComponent<MovementComponent>();
        auto* patrol = entity->GetComponent<PatrolComponent>();
        if (!transform || !movement || !patrol) continue;

        float offset = transform->mapInstance * m_mapWidth;
        float leftBound = patrol->baseLeftBoundary + offset;
        float rightBound = patrol->baseRightBoundary + offset;

        transform->worldX += movement->moveSpeed * movement->direction * deltaTime;
        if (transform->worldX >= rightBound)
        {
            movement->direction = -1;
            transform->worldX = rightBound;
        }
        else if (transform->worldX <= leftBound)
        {
            movement->direction = 1;
            transform->worldX = leftBound;
        }
    }
}

void ScrollSystem::SetParams(float cameraX, int screenWidth, int mapWidth)
{
    m_cameraX = cameraX;
    m_screenWidth = screenWidth;
    m_mapWidth = mapWidth;
}

void ScrollSystem::Update(std::vector<Entity*>& entities, float deltaTime)
{
    if (m_mapWidth == 0) return;
    for (auto* entity : entities)
    {
        if (!entity || !entity->IsActive()) continue;
        auto* transform = entity->GetComponent<TransformComponent>();
        auto* scrollable = entity->GetComponent<ScrollableComponent>();
        if (!transform || !scrollable || !scrollable->shouldReposition) continue;

        if (transform->worldX + transform->width < m_cameraX - 50)
        {
            float ahead = m_cameraX + m_screenWidth + 100;
            int instance = (int)floor(ahead / m_mapWidth);
            if (instance <= transform->mapInstance) instance = transform->mapInstance + 1;
            transform->mapInstance = instance;
            transform->worldX = transform->baseX + instance * m_mapWidth;
            transform->worldY = transform->baseY;
            entity->SetActive(true);
            if (scrollable->onReposition) scrollable->onReposition(entity);
        }
    }
}

void HealthSystem::Update(std::vector<Entity*>& entities, float deltaTime)
{
    for (auto* entity : entities)
    {
        if (!entity || !entity->IsActive()) continue;
        auto* health = entity->GetComponent<HealthComponent>();
        if (!health) continue;

        if (health->isDead)
        {
            health->deathTimer += deltaTime;
            if (health->deathTimer >= health->deathDuration) health->isFullyDead = true;
        }
        else if (health->isInvincible)
        {
            health->invincibleTimer -= deltaTime;
            if (health->invincibleTimer <= 0)
            {
                health->isInvincible = false;
                health->invincibleTimer = 0;
            }
        }
    }
}

void EntityCollisionSystem::Update(std::vector<Entity*>& entities, float deltaTime)
{
    m_score = 0;
    Entity* player = nullptr;
    for (auto* entity : entities)
        if (entity && entity->IsActive() && entity->HasComponent<PlayerTag>()) { player = entity; break; }
    if (!player) return;

    auto* playerTransform = player->GetComponent<TransformComponent>();
    auto* playerCollision = player->GetComponent<CollisionComponent>();
    auto* playerMovement = player->GetComponent<MovementComponent>();
    auto* playerHealth = player->GetComponent<HealthComponent>();
    if (!playerTransform || !playerCollision) return;

    float playerX = playerTransform->worldX + playerCollision->offsetX;
    float playerY = playerTransform->worldY + playerCollision->offsetY;
    float playerWidth = playerCollision->boxWidth;
    float playerHeight = playerCollision->boxHeight;

    for (auto* entity : entities)
    {
        if (!entity || !entity->IsActive() || entity == player) continue;
        auto* entityTransform = entity->GetComponent<TransformComponent>();
        auto* entityCollision = entity->GetComponent<CollisionComponent>();
        if (!entityTransform || !entityCollision) continue;

        float entityX = entityTransform->worldX + entityCollision->offsetX;
        float entityY = entityTransform->worldY + entityCollision->offsetY;
        float entityWidth = entityCollision->boxWidth;
        float entityHeight = entityCollision->boxHeight;

        bool overlapping = playerX < entityX + entityWidth && playerX + playerWidth > entityX &&
                           playerY < entityY + entityHeight && playerY + playerHeight > entityY;
        if (!overlapping) continue;

        auto* collectible = entity->GetComponent<CollectibleComponent>();
        if (collectible && !collectible->collected)
        {
            collectible->collected = true;
            entity->SetActive(false);
            m_score += collectible->pointValue;
            continue;
        }

        auto* enemy = entity->GetComponent<EnemyComponent>();
        if (enemy && !enemy->destroyed)
        {
            bool falling = playerMovement && playerMovement->velocityY > 0;
            if (playerY + playerHeight <= entityY + 8 && falling)
            {
                enemy->destroyed = true;
                entity->SetActive(false);
                m_score += 50;
                if (playerMovement)
                {
                    auto* jump = player->GetComponent<JumpComponent>();
                    if (jump)
                    {
                        playerMovement->velocityY = jump->jumpForce * 0.5f;
                        jump->isJumping = true;
                    }
                    auto* physics = player->GetComponent<PhysicsComponent>();
                    if (physics) physics->isGrounded = false;
                }
            }
            else if (playerHealth && !playerHealth->isInvincible && !playerHealth->isDead)
            {
                playerHealth->health--;
                if (playerHealth->health <= 0)
                {
                    playerHealth->health = 0;
                    playerHealth->isDead = true;
                    playerHealth->deathTimer = 0;
                    if (playerMovement)
                    {
                        playerMovement->velocityX = 0;
                        playerMovement->velocityY = 0;
                    }
                }
                else
                {
                    playerHealth->isInvincible = true;
                    playerHealth->invincibleTimer = playerHealth->invincibleDuration;
                }
            }
        }
    }
}

void AnimationSystem::Update(std::vector<Entity*>& entities, float deltaTime)
{
    for (auto* entity : entities)
    {
        if (!entity || !entity->IsActive()) continue;
        auto* sprite = entity->GetComponent<SpriteComponent>();
        auto* movement = entity->GetComponent<MovementComponent>();
        auto* physics = entity->GetComponent<PhysicsComponent>();
        auto* health = entity->GetComponent<HealthComponent>();
        auto* punch = entity->GetComponent<PunchComponent>();
        if (!sprite) continue;

        if (movement)
        {
            if (movement->velocityX > 0) sprite->facingRight = true;
            else if (movement->velocityX < 0) sprite->facingRight = false;
        }

        if (entity->HasComponent<PlayerTag>() && health)
        {
            sprite->flickering = health->isInvincible;
            if (health->isDead) sprite->currentAnimation = "hurt";
            else if (punch && punch->isPunching) sprite->currentAnimation = "punch";
            else if (physics && !physics->isGrounded) sprite->currentAnimation = "jumpandfall";
            else if (movement && movement->velocityX != 0) sprite->currentAnimation = "run";
            else sprite->currentAnimation = "idle";
        }
    }
}

void RenderSystem::Render(std::vector<Entity*>& entities, Renderer* renderer, Camera* camera)
{
    for (auto* entity : entities)
    {
        if (!entity || !entity->IsActive()) continue;
        auto* transform = entity->GetComponent<TransformComponent>();
        auto* sprite = entity->GetComponent<SpriteComponent>();
        if (!transform || !sprite || !sprite->visible || !sprite->animLoader) continue;

        if (sprite->flickering && ++sprite->flickerCounter % 6 < 3) continue;

        float width = transform->width * transform->scale;
        float height = transform->height * transform->scale;
        float screenX = camera ? camera->WorldToScreenX(transform->worldX) : transform->worldX;
        float screenY = transform->worldY;

        Point windowSize = renderer->GetWindowSize();
        if (screenX < -width || screenX > windowSize.X + width || screenY + height < 0) continue;

        Rect destRect = sprite->facingRight
            ? Rect((unsigned)screenX, (unsigned)(screenY < 0 ? 0 : screenY), (unsigned)(screenX + width), (unsigned)(screenY + height))
            : Rect((unsigned)(screenX + width), (unsigned)(screenY < 0 ? 0 : screenY), (unsigned)screenX, (unsigned)(screenY + height));

        Rect srcRect = sprite->animLoader->UpdateAnimation(sprite->currentAnimation, Timing::Instance().GetDeltaTime());
        Texture* texture = sprite->animLoader->GetTexture(sprite->currentAnimation);
        if (texture) renderer->RenderTexture(texture, srcRect, destRect);
    }
}
