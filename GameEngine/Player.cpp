#include "Player.h"
#include "FileController.h"
#include "Timing.h"
#include "InputController.h"
#include "Keyboard.h"

Player::Player()
{
	m_sprite = nullptr;
	m_position = Point(100, 300);
	scale = 2.0f;
	m_walkSpeed = 100.0f; // pixels per second
	m_runSpeed = 200.0f;
	m_veloX = 0;
	m_veloY = 0;
	m_isRunning = false;
	m_shiftDown = false;
}

Player::~Player()
{
}

void Player::Initialize()
{
	m_sprite = SpriteSheet::Pool->GetResource();
	m_sprite->Load("../Assets/Textures/Warrior.tga");
	m_sprite->SetSize(17, 6, 69, 44);
	m_sprite->AddAnimation(EN_AN_IDLE, 0, 6, 6.0f);
	m_sprite->AddAnimation(EN_AN_RUN, 6, 8, 6.0f);
}

void Player::Update(float _deltaTime)
{
	// Move player
	m_position.X += (int)(m_veloX * _deltaTime);

	// Animation logic
	if (m_veloX == 0)
		m_sprite->Update(EN_AN_IDLE, _deltaTime);
	else if (m_isRunning)
		m_sprite->Update(EN_AN_RUN, _deltaTime);
	else
		m_sprite->Update(EN_AN_IDLE, _deltaTime);
}

void Player::Render(Renderer* _renderer)
{
	float width = 69 * scale;
	float height = 44 * scale;

	float x = m_position.X;
	float y = m_position.Y;

	// Destination on the screen
	Rect destRect( (unsigned)x, (unsigned)y,
		(unsigned)(x + width), (unsigned)(y + height));

	// Get the part of the sprite sheet for the current animation frame
	Rect srcRect(0, 0, 0, 0);

	if (m_veloX == 0)
		srcRect = m_sprite->Update(EN_AN_IDLE, 0);
	else if (m_isRunning)
		srcRect = m_sprite->Update(EN_AN_RUN, 0);
	else
		srcRect = m_sprite->Update(EN_AN_IDLE, 0);

	_renderer->RenderTexture(m_sprite, srcRect, destRect);
}

void Player::HandleInput(SDL_Event _event)
{
	Keyboard* kb = InputController::Instance().KB();

	// SHIFT DOWN
	if (kb->KeyDown(_event, SDLK_LSHIFT) || kb->KeyDown(_event, SDLK_RSHIFT))
		m_shiftDown = true;

	if (kb->KeyUp(_event, SDLK_LSHIFT) || kb->KeyUp(_event, SDLK_RSHIFT))
		m_shiftDown = false;

	// A Key
	if (kb->KeyDown(_event, SDLK_a))
	{
		float speed = m_shiftDown ? m_runSpeed : m_walkSpeed;

		m_veloX = -speed;
		m_isRunning = m_shiftDown;
	}

	// D Key
	if (kb->KeyDown(_event, SDLK_d))
	{
		float speed = m_shiftDown ? m_runSpeed : m_walkSpeed;

		m_veloX = speed;
		m_isRunning = m_shiftDown;
	}

	// RELEASE A or D
	if (kb->KeyUp(_event, SDLK_a) || kb->KeyUp(_event, SDLK_d))
	{
		m_veloX = 0;
		m_isRunning = false;
	}
}