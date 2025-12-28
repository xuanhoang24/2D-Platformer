#include "../Game/Enemy.h"
#include "../Game/GameMap.h"
#include "../Graphics/Camera.h"
#include "../Core/Timing.h"

Enemy::Enemy()
{
	m_animLoader = nullptr;
	m_worldX = 0.0f;
	m_worldY = 0.0f;
	m_baseX = 0.0f;
	m_baseY = 0.0f;
	m_isActive = true;
	m_currentMapInstance = 0;
	m_type = EnemyType::Ghost;
	m_moveSpeed = 50.0f;
	m_direction = 1.0f;
	m_baseLeftBoundary = 0.0f;
	m_baseRightBoundary = 0.0f;
	m_gameMap = nullptr;
}

Enemy::~Enemy()
{
	if (m_animLoader)
	{
		delete m_animLoader;
		m_animLoader = nullptr;
	}
}

void Enemy::Initialize(float x, float y, EnemyType type)
{
}

void Enemy::Initialize(float x, float y, EnemyType type, float leftBoundary, float rightBoundary)
{
	// Store base position within single map
	m_baseX = x - (GetWidth() * 0.5f);
	m_baseY = y - GetHeight();
	
	// Initial world position is same as base
	m_worldX = m_baseX;
	m_worldY = m_baseY;
	
	// Store base boundaries within single map
	m_baseLeftBoundary = leftBoundary;
	m_baseRightBoundary = rightBoundary;
	
	m_isActive = true;
	m_currentMapInstance = 0;
	m_type = type;
	
	RandomEnemy();
	
	// Random initial direction
	m_direction = (rand() % 2 == 0) ? -1.0f : 1.0f;
}

void Enemy::Update(float _deltaTime, float _cameraX, int _screenWidth, int _mapPixelWidth)
{
	// Check if enemy went behind camera (left of screen) - reposition ahead
	float cameraLeftEdge = _cameraX;
	float enemyRightEdge = m_worldX + GetWidth();
	
	if (enemyRightEdge < cameraLeftEdge - 50.0f) // 50px buffer behind camera
	{
		RepositionAhead(_cameraX, _screenWidth, _mapPixelWidth);
	}
	
	if (!m_isActive)
		return;
	
	// Calculate current boundaries based on map instance
	float mapOffset = m_currentMapInstance * _mapPixelWidth;
	float leftBound = m_baseLeftBoundary + mapOffset;
	float rightBound = m_baseRightBoundary + mapOffset;
	
	// Move in current direction
	m_worldX += m_moveSpeed * m_direction * _deltaTime;
	
	// Check if enemy has reached zone boundaries
	if (m_worldX >= rightBound)
	{
		m_direction = -1.0f;
		m_worldX = rightBound;
	}
	else if (m_worldX <= leftBound)
	{
		m_direction = 1.0f;
		m_worldX = leftBound;
	}
	
	// Check map collision
	if (m_gameMap)
	{
		float wallX;
		float width = GetWidth();
		float height = GetHeight();
		
		if (m_direction > 0)
		{
			if (m_gameMap->CheckCollisionLeft(m_worldX, m_worldY, width, height, wallX))
			{
				m_worldX = wallX - width;
				m_direction = -1.0f;
			}
		}
		else
		{
			if (m_gameMap->CheckCollisionRight(m_worldX, m_worldY, width, height, wallX))
			{
				m_worldX = wallX;
				m_direction = 1.0f;
			}
		}
	}
}

void Enemy::RepositionAhead(float _cameraX, int _screenWidth, int _mapPixelWidth)
{
	// Calculate which map instance is ahead of camera (right side of screen + buffer)
	float aheadX = _cameraX + _screenWidth + 100.0f; // 100px buffer ahead
	int targetMapInstance = (int)floor(aheadX / _mapPixelWidth);
	
	// Make sure moving forward, not backward
	if (targetMapInstance <= m_currentMapInstance)
		targetMapInstance = m_currentMapInstance + 1;
	
	// Update to new map instance
	m_currentMapInstance = targetMapInstance;
	
	// Reset position to base position in new map instance
	float mapOffset = m_currentMapInstance * _mapPixelWidth;
	m_worldX = m_baseX + mapOffset;
	m_worldY = m_baseY;
	
	// Reactivate and reset direction
	m_isActive = true;
	m_direction = (rand() % 2 == 0) ? -1.0f : 1.0f;
}

vector<Enemy*> Enemy::SpawnEnemiesFromMap(GameMap* _map)
{
	vector<Enemy*> enemies;
	
	if (!_map)
		return enemies;
	
	srand((unsigned int)time(nullptr));
	
	const vector<pair<float, float>>& spawnPoints = _map->GetEnemySpawnPoints();
	
	for (const auto& spawn : spawnPoints)
	{
		Enemy* enemy = new Enemy();
		
		EnemyType randomType = (rand() % 2 == 0) ? EnemyType::Ghost : EnemyType::Mushroom;
		
		float leftX = 0.0f;
		float rightX = 0.0f;
		_map->GetEnemyZoneBoundaries(spawn.first, spawn.second, leftX, rightX);
		
		enemy->Initialize(spawn.first, spawn.second, randomType, leftX, rightX);
		enemies.push_back(enemy);
	}
	
	return enemies;
}

void Enemy::Render(Renderer* _renderer, Camera* _camera)
{
	if (!m_isActive)
		return;
	
	float width = GetWidth();
	float height = GetHeight();
	
	// Convert world position to screen position
	float screenX = _camera ? _camera->WorldToScreenX(m_worldX) : m_worldX;
	float screenY = m_worldY;
	
	// Only render if on screen (with buffer for smooth appearance)
	Point screenSize = _renderer->GetWindowSize();
	if (screenX < -width || screenX > screenSize.X + width)
		return;
	
	Rect destRect(
		(unsigned)screenX,
		(unsigned)screenY,
		(unsigned)(screenX + width),
		(unsigned)(screenY + height));
	
	string currentAnim = "idle";
	
	Rect srcRect = m_animLoader->UpdateAnimation(currentAnim, Timing::Instance().GetDeltaTime());
	Texture* currentTexture = m_animLoader->GetTexture(currentAnim);
	
	if (currentTexture)
		_renderer->RenderTexture(currentTexture, srcRect, destRect);
}

void Enemy::Reset()
{
	// Reset to original spawn position
	m_worldX = m_baseX;
	m_worldY = m_baseY;
	m_currentMapInstance = 0;
	m_isActive = true;
	m_direction = (rand() % 2 == 0) ? -1.0f : 1.0f;
	
	// Randomize enemy type on reset
	EnemyType newType = (rand() % 2 == 0) ? EnemyType::Ghost : EnemyType::Mushroom;
	if (newType != m_type)
	{
		m_type = newType;
		RandomEnemy();
	}
}

void Enemy::RandomEnemy()
{
	if (m_animLoader)
	{
		delete m_animLoader;
		m_animLoader = nullptr;
	}
	
	m_animLoader = new AnimatedSpriteLoader();
	
	if (m_type == EnemyType::Ghost)
	{
		m_animLoader->LoadAnimation("idle", "../Assets/Textures/Enemy/ghost1_fly.png", 1, 6, 16, 16, 6, 10.0f);
		m_moveSpeed = 30.0f;
	}
	else
	{
		m_animLoader->LoadAnimation("idle", "../Assets/Textures/Enemy/mushroom-walk.png", 1, 10, 16, 16, 10, 10.0f);
		m_moveSpeed = 40.0f;
	}
}
