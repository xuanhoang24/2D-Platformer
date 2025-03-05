#include "RenderTarget.h"
#include "Renderer.h"

RenderTarget::RenderTarget()
{
	m_texture = nullptr;
	m_width = 0;
	m_height = 0;
	m_rotation = 0;
}

RenderTarget::~RenderTarget()
{
	if (m_texture != nullptr)
	{
		SDL_DestroyTexture(m_texture);
		m_texture = nullptr;
	}
}

void RenderTarget::Create(unsigned int _width, unsigned int _height)
{
	m_width = _width;
	m_height = _height;

	m_texture = SDL_CreateTexture(Renderer::Instance().GetRenderer(), SDL_PIXELFORMAT_RGBA8888,
		SDL_TEXTUREACCESS_TARGET, m_width, m_height);
	M_ASSERT(m_texture != nullptr, "Unable to create render target");
}

void RenderTarget::Start()
{
	SDL_SetRenderTarget(Renderer::Instance().GetRenderer(), m_texture);
}

void RenderTarget::Stop()
{
	SDL_SetRenderTarget(Renderer::Instance().GetRenderer(), nullptr);
}
void RenderTarget::Render(float _deltaTime)
{
	m_rotation += 10 * _deltaTime;
	Point ws = Renderer::Instance().GetWindowSize();
	Renderer::Instance().RenderTexture(m_texture, Rect(0, 0, ws.X, ws.Y), Rect(0, 0, ws.X, ws.Y), m_rotation);
}

