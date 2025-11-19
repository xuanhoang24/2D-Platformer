#include "GameMap.h"

GameMap::GameMap()
{
    m_tileMap = nullptr;
}

GameMap::~GameMap()
{
    delete m_tileMap;
    m_tileMap = nullptr;
}

bool GameMap::Load(const string& _file)
{
    m_tileMap = new TileMap();
    return m_tileMap->Load(_file);
}

void GameMap::Render(Renderer* _renderer)
{
    if (m_tileMap)
        m_tileMap->Render(_renderer);
}
