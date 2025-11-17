#ifndef PLATFORM_H
#define PLATFORM_H

#include "StandardIncludes.h"
#include "Renderer.h"
#include "SpriteSheet.h"

class Platform
{
public:
	Platform();
	virtual ~Platform();

	void Initialize(string _texturePath, int _x, int _y, int _width, int _height);
	void Update(float _deltaTime);
	void Render(Renderer* _renderer);

	Rect GetCollisionBox();

private:
	Texture* m_texture;
	Point m_position;
	int m_width;
	int m_height;
};
#endif // PLATFORM_H