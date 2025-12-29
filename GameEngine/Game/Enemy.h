#ifndef ENEMY_H
#define ENEMY_H

#include "../Core/StandardIncludes.h"
#include "../Graphics/Renderer.h"
#include "../Graphics/AnimatedSpriteLoader.h"

class Camera;

enum class EnemyType
{
	Ghost,
	Mushroom
};

class Enemy
{
public:
	Enemy();
	virtual ~Enemy();

	void Initialize(float x, float y, EnemyType type);
	void Initialize(float x, float y, EnemyType type, float leftBoundary, float rightBoundary);
	void Update(float _deltaTime, float _cameraX, int _screenWidth, int _mapPixelWidth);
	void Render(Renderer* _renderer, Camera* _camera);
	
	float GetWorldX() const { return m_worldX; }
	float GetWorldY() const { return m_worldY; }
	float GetBaseX() const { return m_baseX; } // Original spawn X within map
	float GetBaseY() const { return m_baseY; } // Original spawn Y
	float GetWidth() const { return 16.0f; }
	float GetHeight() const { return 16.0f; }
	
	bool IsActive() const { return m_isActive; }
	void Destroy() { m_isActive = false; }
	void Reset();
	
	// Reposition enemy to next map instance in front of camera
	void RepositionAhead(float _cameraX, int _screenWidth, int _mapPixelWidth);
	
	EnemyType GetType() const { return m_type; }
	
	void GetCollisionBox(float& outX, float& outY, float& outWidth, float& outHeight) const
	{
		outX = m_worldX;
		outY = m_worldY;
		outWidth = GetWidth();
		outHeight = GetHeight();
	}

private:
	void RandomEnemy();
	
	AnimatedSpriteLoader* m_animLoader;
	float m_worldX;      // Current world X position
	float m_worldY;      // Current world Y position
	float m_baseX;       // Original spawn X within single map (0 to mapWidth)
	float m_baseY;       // Original spawn Y
	bool m_isActive;
	int m_currentMapInstance; // Which map instance this enemy is currently in
	EnemyType m_type;
	
	// Movement
	float m_moveSpeed;
	float m_direction; // -1 for left, 1 for right
	
	// Zone boundaries (base values within single map)
	float m_baseLeftBoundary;
	float m_baseRightBoundary;
};

#endif // ENEMY_H
