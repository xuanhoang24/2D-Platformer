#include "../Graphics/Camera.h"
#include "../Graphics/Renderer.h"
#include "../Game/Entity.h"
#include "../Game/Components.h"

Camera::Camera()
	: m_x(0.0f)
	, m_y(0.0f)
	, m_maxX(0.0f)
{
}

Camera::~Camera()
{
}

void Camera::Update(float _deltaTime)
{
}

void Camera::FollowEntity(Entity* _entity, Renderer* _renderer)
{
	if (!_entity || !_renderer) return;

	auto* transform = _entity->GetComponent<TransformComponent>();
	if (!transform) return;

	float entityWorldX = transform->worldX;
	float entityWidth = transform->width * transform->scale;
	
	Point logicalSize = _renderer->GetLogicalSize();
	int logicalWidth = logicalSize.X;

	float targetX = entityWorldX + entityWidth * 0.5f - logicalWidth * 0.5f;
	if (targetX < 0) targetX = 0;

	if (targetX > m_maxX)
	{
		m_maxX = targetX;
		m_x = targetX;
	}
	else
	{
		m_x = m_maxX;
	}
	
	m_y = 0;
}

void Camera::SetPosition(float _x, float _y)
{
	m_x = _x;
	m_y = _y;
}

float Camera::WorldToScreenX(float _worldX) const { return _worldX - m_x; }
float Camera::WorldToScreenY(float _worldY) const { return _worldY - m_y; }
float Camera::ScreenToWorldX(float _screenX) const { return _screenX + m_x; }
float Camera::ScreenToWorldY(float _screenY) const { return _screenY + m_y; }

void Camera::Reset()
{
	m_x = 0.0f;
	m_y = 0.0f;
	m_maxX = 0.0f;
}
