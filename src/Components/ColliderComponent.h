#ifndef COLLIDERCOMPONENT_H
#define COLLIDERCOMPONENT_H

#include <SDL2/SDL.h>

#include "../Game.h"
#include "../EntityManager.h"
#include "../AssetManager.h"
#include "../TextureManager.h"
#include "./TransformComponent.h"

class ColliderComponent : public Component
{
    public:
        std::string m_colliderTag;
        bool m_colliderBoxActive;
        bool m_colliderBoxExists;
        SDL_RendererFlip m_spriteFlip = SDL_FLIP_NONE;
        SDL_Rect m_collider;
        SDL_Rect m_sourceRectangle;
        SDL_Rect m_destinationRectangle;
        TransformComponent* mp_transform;

        ColliderComponent(std::string colliderTag, int x, int y, int width, int height) 
        {
            m_colliderTag = colliderTag;
            m_collider = {x, y, width, height};
            m_colliderBoxActive = false;
            m_colliderBoxExists = false;
        }

        ColliderComponent(std::string colliderTag, std::string hitBoxId, int x, int y, int width, int height) 
        {
            m_colliderTag = colliderTag;
            m_collider = {x, y, width, height};
            m_colliderBoxActive = false;
            m_colliderBoxExists = true;
            SetTexture(hitBoxId);
        }
        
        void SetTexture(std::string assetTextureId)
        {
            mp_texture = Game::mp_assetManager->GetTexture(assetTextureId);
        }

        void Initialize() override 
        {
            if (m_owner->HasComponent<TransformComponent>())
            {
                mp_transform = m_owner->GetComponent<TransformComponent>();
                m_sourceRectangle = {0, 0, mp_transform->m_width, mp_transform->m_height};
                m_destinationRectangle = {m_collider.x, m_collider.y, m_collider.w, m_collider.h};
            }

        }

        void Update(float deltTime) override
        {
            m_collider.x = static_cast<int>(mp_transform->m_position.x);
            m_collider.y = static_cast<int>(mp_transform->m_position.y);
            m_collider.w = mp_transform->m_width * mp_transform->m_scale;
            m_collider.h = mp_transform->m_height * mp_transform->m_scale;

            m_destinationRectangle.x = m_collider.x - Game::m_camera.x;
            m_destinationRectangle.y = m_collider.y - Game::m_camera.y;
            m_destinationRectangle.w = mp_transform->m_width * mp_transform->m_scale;
            m_destinationRectangle.h = mp_transform->m_height * mp_transform->m_scale;

            std::string keyCode = std::to_string(Game::m_event.key.keysym.sym);
            std::string selector = "1073741882"; // Using f1
            
            if (Game::m_event.type == SDL_KEYDOWN && m_colliderBoxExists)
            {
                if (keyCode.compare(selector) == 0)
                {
                    m_colliderBoxActive = true;
                }
            }

            if (Game::m_event.type == SDL_KEYUP && m_colliderBoxExists)
            {
                if (keyCode.compare(selector) == 0)
                {
                    m_colliderBoxActive = false;
                }
            }
        }

        void Render() override
        {
            if (m_colliderBoxActive)
            {
                TextureManager::Draw(mp_texture, m_sourceRectangle, m_destinationRectangle, m_spriteFlip);
            }
        }

    private:
        SDL_Texture* mp_texture;
};

#endif
