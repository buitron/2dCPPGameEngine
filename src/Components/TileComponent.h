#ifndef TILECOMPONENT_H
#define TILECOMPONENT_H

#include <SDL2/SDL.h>
#include "../../lib/glm/glm.hpp"
#include "../EntityManager.h"
#include "../AssetManager.h"

class TileComponent : public Component
{
    public:
        SDL_Texture* mp_texture;
        SDL_Rect m_sourceRectangle;
        SDL_Rect m_destinationRectangle;
        glm::vec2 m_position;

        TileComponent(int sourceRectX, int sourceRectY, int x, int y, int tileSize, int tileScale, std::string assetTextureId) 
        {
            mp_texture = Game::mp_assetManager->GetTexture(assetTextureId);

            m_sourceRectangle.x = sourceRectX;
            m_sourceRectangle.y = sourceRectY;
            m_sourceRectangle.w = tileSize;
            m_sourceRectangle.h = tileSize;

            m_destinationRectangle.x = x;
            m_destinationRectangle.y = y;
            m_destinationRectangle.w = tileSize * tileScale;
            m_destinationRectangle.h = tileSize * tileScale;

            m_position.x = x;
            m_position.y = y;
        }


        ~TileComponent() 
        {
            SDL_DestroyTexture(mp_texture);
        }

        void Update(float deltaTime) override 
        {
            m_destinationRectangle.x = m_position.x - Game::m_camera.x;
            m_destinationRectangle.y = m_position.y - Game::m_camera.y;
        }

        void Render() override
        {
            TextureManager::Draw(mp_texture, m_sourceRectangle, m_destinationRectangle, SDL_FLIP_NONE);
        }
};

#endif
