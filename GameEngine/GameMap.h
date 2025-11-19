#ifndef GAME_MAP_H
#define GAME_MAP_H

#include "StandardIncludes.h"
#include "TileMap.h"

class Renderer;

class GameMap
{
public:
    GameMap();
    ~GameMap();

    bool Load(const string& _file);
    void Render(Renderer* _renderer);

private:
    TileMap* m_tileMap = nullptr;
};

#endif // GAME_MAP_H
