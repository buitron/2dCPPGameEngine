#ifndef MAP_H
#define MAP_H

#include <string>

class Map
{
    public:
        Map(std::string textureId, int scale, int tileSize);
        ~Map();
        int GetScale() const;
        void LoadMap(std::string filePath, int mapSizeX, int mapSizeY);
        void AddTile(int sourceX, int sourceY, int x, int y);
    private:
        std::string m_textureId;
        int m_scale;
        int m_tileSize;
};

#endif
