#include "GameController.h"
#include "Renderer.h"

GameController::GameController()
{
    m_sdlEvent = { };
}

GameController::~GameController()
{
}

void GameController::RunGame()
{
    AssetController::Instance().Initialize(10000000); // Allocate 10MB
    Renderer* r = &Renderer::Instance();
    r->Initialize(800, 600);
    Point ws = r->GetWindowSize();

    Texture::Pool = new ObjectPool<Texture>();
    Texture* texture = Texture::Pool->GetResource();
    texture->Load("../Assets/Textures/Emoji.tga");

    while (m_sdlEvent.type != SDL_QUIT)
    {
        SDL_PollEvent(&m_sdlEvent);
        r->SetViewport(Rect(0, 0, ws.X, ws.Y));
        r->SetDrawColor(Color(255, 0, 0, 255));
        r->ClearScreen();
        r->SetViewport(Rect(0, 0, ws.X / 2, ws.Y / 2)); //Top-left
        r->RenderTexture(texture, Point(0, 0));
        r->SetViewport(Rect(ws.X / 2, 0, ws.X, ws.Y / 2)); //Top-right
        r->RenderTexture(texture, Rect(0, 0, ws.X / 2, ws.Y / 2));
        r->SetViewport(Rect(0, ws.Y/2, ws.X/2, ws.Y)); //Bottom-left
        r->RenderTexture(texture, Rect(0, 0, ws.X / 2, ws.Y / 2));
        r->SetViewport(Rect(ws.X / 2, ws.Y / 2, ws.X, ws.Y)); //Bottom-right
        r->RenderTexture(texture, Point(0, 0));
        SDL_RenderPresent(r->GetRenderer());
    }

    delete Texture::Pool;
    r->Shutdown();
}
