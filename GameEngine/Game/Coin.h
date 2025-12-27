#ifndef COIN_H
#define COIN_H

#include "../Core/StandardIncludes.h"
#include "../Graphics/Renderer.h"
#include "../Graphics/AnimatedSpriteLoader.h"

class Camera;
class GameMap;

class Coin
{
public:
	Coin();
	virtual ~Coin();

	void Initialize(float x, float y);
	void Update(float _deltaTime);
	void Render(Renderer* _renderer, Camera* _camera);
	
	static vector<Coin*> SpawnCoinsFromMap(GameMap* _map);
	
	float GetWorldX() const { return m_worldX; }
	float GetWorldY() const { return m_worldY; }
	float GetWidth() const { return 16.0f; }
	float GetHeight() const { return 16.0f; }
	
	bool IsActive() const { return m_isActive; }
	void Collect() { m_isActive = false; }

private:
	AnimatedSpriteLoader* m_animLoader;
	float m_worldX;
	float m_worldY;
	bool m_isActive;
};

#endif // COIN_H
