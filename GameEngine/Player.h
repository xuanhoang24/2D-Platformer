#ifndef PLAYER_H
#define PLAYER_H

#include "StandardIncludes.h"
#include "Renderer.h"
#include "SpriteSheet.h"

class Player
{
public:
	Player();
	virtual ~Player();

	void Initialize();
	void Update(float _deltaTime);
	void Render(Renderer* _renderer);
	void HandleInput(SDL_Event _event);

private:
	SpriteSheet* m_sprite;
	Point m_position;

	float scale;
	float m_walkSpeed;
	float m_runSpeed;
	float m_veloX;
	float m_veloY;
	bool m_isRunning;
	bool m_shiftDown;
};

#endif // PLAYER_H

