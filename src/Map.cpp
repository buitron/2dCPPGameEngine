#include <fstream>

#include "./Map.h"
#include "./Game.h"
#include "./EntityManager.h"
#include "./Components/TileComponent.h"

extern EntityManager manager; 

Map::Map(std::string textureId, int scale, int tileSize): m_textureId(textureId), m_scale(scale), m_tileSize(tileSize) {}

int Map::GetScale() const
{
    return m_scale;
}

void Map::LoadMap(std::string filePath, int mapSizeX, int mapSizeY)
{
    std::fstream mapFile;
    mapFile.open(filePath);

    for (int y = 0; y < mapSizeY; ++y)
    {
        for (int x = 0; x < mapSizeX; ++x)
        {
            char ch;
            mapFile.get(ch);
            int sourceRectY = atoi(&ch) * m_tileSize;
            mapFile.get(ch);
            int sourceRectX = atoi(&ch) * m_tileSize;
            AddTile(sourceRectX, sourceRectY, x * (m_scale * m_tileSize), y * (m_scale * m_tileSize));
            mapFile.ignore();
        }
    }
    mapFile.close();
}

void Map::AddTile(int sourceRectX, int sourceRectY, int x, int y)
{
    Entity& newTile(manager.AddEntity("Tile", TILEMAP_LAYER));
    newTile.AddComponent<TileComponent>(sourceRectX, sourceRectY, x, y, m_tileSize, m_scale,  m_textureId);
}
