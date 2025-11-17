#include "Platform.h"
#include "AssetController.h"

Platform::Platform()
{
	m_texture = nullptr;
	m_position = Point(0, 0);
	m_width = 0;
	m_height = 0;
}

Platform::~Platform()
{
}

void Platform::Initialize(string _texturePath, int _x, int _y, int _width, int _height)
{
	m_texture = Texture::Pool->GetResource();
	m_texture->Load(_texturePath);

	m_position = Point(_x, _y);
	m_width = _width;
	m_height = _height;
}

void Platform::Update(float _deltaTime)
{

}

void Platform::Render(Renderer* _renderer)
{
	Rect dest(
		m_position.X,
		m_position.Y,
		m_position.X + m_width,
		m_position.Y + m_height
	);

	_renderer->RenderTexture(m_texture, dest);
}

Rect Platform::GetCollisionBox()
{
	return Rect(
		m_position.X,
		m_position.Y,
		m_position.X + m_width,
		m_position.Y + m_height
	);
}