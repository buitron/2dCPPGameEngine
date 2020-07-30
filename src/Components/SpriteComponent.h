#ifndef SPRITECOMPONENT_H
#define SPRITECOMPONENT_H

#include <SDL2/SDL.h>
#include <string>

#include "../Animation.h"
#include "../AssetManager.h"
#include "../TextureManager.h"
#include "./TransformComponent.h"

class SpriteComponent : public Component
{
    public:
        SDL_RendererFlip m_spriteFlip = SDL_FLIP_NONE;

        SpriteComponent(std::string id) 
        {
            m_isAnimated = false;
            m_isFixed = false;
            SetTexture(id);
        }

        SpriteComponent(std::string id, int numFrames, int animationSpeed, bool hasDirections, bool isFixed)
        {
            m_isAnimated = true;
            m_numFrames = numFrames;
            m_animationSpeed = animationSpeed;
            m_isFixed = isFixed;

            if (hasDirections)
            {
                Animation downAnimation = Animation(0, m_numFrames, m_animationSpeed);
                Animation rightAnimation = Animation(1, m_numFrames, m_animationSpeed);
                Animation leftAnimation = Animation(2, m_numFrames, m_animationSpeed);
                Animation upAnimation = Animation(3, m_numFrames, m_animationSpeed);

                m_animations.emplace("DownAnimation", downAnimation);
                m_animations.emplace("RightAnimation",rightAnimation);
                m_animations.emplace("LeftAnimation", leftAnimation);
                m_animations.emplace("UpAnimation", upAnimation);

                m_animationIndex = 0;
                m_currentAnimationName = "DownAnimation";
            }
            else
            {
                Animation singleAnimation = Animation(0, numFrames, animationSpeed);
                m_animations.emplace("SingleAnimation", singleAnimation);
                m_animationIndex = 0;
                m_currentAnimationName = "SingleAnimation";
            
            }
    
            Play(m_currentAnimationName);
            SetTexture(id);
        }

        void Play(std::string animationName)
        {
            m_numFrames = m_animations[animationName].m_numFrames;
            m_animationIndex = m_animations[animationName].m_index;
            m_animationSpeed = m_animations[animationName].m_animationSpeed;
            m_currentAnimationName = animationName; 

        }
        
        void SetTexture(std::string assetTextureId)
        {
            mp_texture = Game::mp_assetManager->GetTexture(assetTextureId);
            SDL_RendererFlip m_spriteFlip = SDL_FLIP_NONE;
        }

        void Initialize() override
        {
            mp_transform = m_owner->GetComponent<TransformComponent>();
            m_sourceRectangle.x = 0;
            m_sourceRectangle.y = 0;
            m_sourceRectangle.w = mp_transform->m_width;
            m_sourceRectangle.h = mp_transform->m_height;
        }

        void Update(float deltaTime) override
        {
            if (m_isAnimated)
            {
                m_sourceRectangle.x = (m_sourceRectangle.w * static_cast<int>((SDL_GetTicks() / m_animationSpeed) % m_numFrames));
            }
            m_sourceRectangle.y = m_animationIndex * mp_transform->m_height;

            m_destinationRectangle.x = static_cast<int>(mp_transform->m_position.x) - (m_isFixed ? 0 : Game::m_camera.x);
            m_destinationRectangle.y = static_cast<int>(mp_transform->m_position.y) - (m_isFixed ? 0 : Game::m_camera.y);
            m_destinationRectangle.w = mp_transform->m_width * mp_transform->m_scale;
            m_destinationRectangle.h = mp_transform->m_height * mp_transform->m_scale;
        }

        void Render() override
        {
            TextureManager::Draw(mp_texture, m_sourceRectangle, m_destinationRectangle, m_spriteFlip);
        }

    private:
        TransformComponent* mp_transform;
        SDL_Texture* mp_texture;
        SDL_Rect m_sourceRectangle;
        SDL_Rect m_destinationRectangle;
        bool m_isAnimated;
        bool m_isFixed;
        int m_numFrames;
        int m_animationSpeed;
        unsigned int m_animationIndex = 0;
        std::string m_currentAnimationName;
        std::map<std::string, Animation> m_animations;
};

#endif
