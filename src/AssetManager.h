#ifndef ASSETMANAGER_H
#define ASSETMANAGER_H

#include <map>
#include <string>
#include <SDL2/SDL_ttf.h>

#include "./TextureManager.h"
#include "./FontManager.h"
#include "./EntityManager.h"

class AssetManager
{
    public:
        AssetManager(EntityManager* manager);
        ~AssetManager();
        void ClearData();
        void AddTexture(std::string textureId, const char* filePath);
        void AddFont(std::string fontId, const char* filePath, int fontSize);
        SDL_Texture* GetTexture(std::string textureId);
        TTF_Font* GetFont(std::string fontId);
    private:
        EntityManager* mp_manager;
        std::map<std::string, SDL_Texture*> m_textures;
        std::map<std::string, TTF_Font*> m_fonts;
};


#endif
