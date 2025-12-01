#ifndef TILE_MAP_H
#define TILE_MAP_H

#include "../Core/StandardIncludes.h"
#include <tmxlite/Map.hpp>
#include <tmxlite/TileLayer.hpp>
#include "../Graphics/Renderer.h"
#include "../Graphics/Texture.h"

class TileMap
{
public:
    TileMap();
    virtual ~TileMap();

    bool Load(const string& _path);
    void Render(Renderer* _renderer);

private:
    struct TilesetInfo
    {
        SDL_Texture* texture = nullptr;
        int firstGID = 0;

        int tileWidth = 0;
        int tileHeight = 0;

        int columns = 0;
        int imageWidth = 0;
        int imageHeight = 0;
    };

    struct LayerInfo
    {
        const tmx::TileLayer* layer = nullptr;
    };

    // Methods
    void LoadTilesets();
    void LoadLayers();
    TilesetInfo* FindTileset(int gid);

    // Members
    tmx::Map m_map;

    std::vector<TilesetInfo> m_tilesets;
    std::vector<LayerInfo>   m_layers;

    // Map info
    int m_mapWidth = 0;
    int m_mapHeight = 0;
    int m_tileWidth = 0;
    int m_tileHeight = 0;
};

#endif // TILE_MAP_H

