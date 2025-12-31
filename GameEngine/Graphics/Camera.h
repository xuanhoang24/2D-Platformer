#ifndef CAMERA_H
#define CAMERA_H

#include "../Core/StandardIncludes.h"

class Renderer;
class Entity;

class Camera
{
public:
	Camera();
	virtual ~Camera();

	void Update(float _deltaTime);
	void FollowEntity(Entity* _entity, Renderer* _renderer);

	float GetX() const { return m_x; }
	float GetY() const { return m_y; }
	float GetMaxX() const { return m_maxX; }

	void SetX(float _x) { m_x = _x; m_maxX = _x; }
	void SetY(float _y) { m_y = _y; }
	void SetPosition(float _x, float _y);
	void Reset();

	float WorldToScreenX(float _worldX) const;
	float WorldToScreenY(float _worldY) const;
	float ScreenToWorldX(float _screenX) const;
	float ScreenToWorldY(float _screenY) const;

private:
	float m_x;
	float m_y;
	float m_maxX;
};

#endif // CAMERA_H
