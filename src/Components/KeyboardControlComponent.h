#ifndef KEYBOARDCONTROLCOMPONENT_H
#define KEYBOARDCONTROLCOMPONENT_H

#include "../Game.h"
#include "../Constants.h"
#include "../EntityManager.h"
#include "../Components/TransformComponent.h"
#include "../Components/SpriteComponent.h"
#include "../Components/ProjectileEmitterComponent.h"

class KeyboardControlComponent : public Component
{
    public:
        std::string m_upKey;
        std::string m_downKey;
        std::string m_rightKey;
        std::string m_leftKey;
        std::string m_shootKey;
        std::string m_entityName;

        TransformComponent* mp_transform;
        SpriteComponent* mp_sprite;
        ProjectileEmitterComponent* mp_projectile;

        KeyboardControlComponent() {}

        KeyboardControlComponent(std::string upKey, std::string rightKey, std::string downKey, std::string leftKey, std::string shootKey)
        {
            m_upKey = GetSDLKeyStringCode(upKey);
            m_rightKey = GetSDLKeyStringCode(rightKey);
            m_downKey = GetSDLKeyStringCode(downKey);
            m_leftKey = GetSDLKeyStringCode(leftKey);
            m_shootKey = GetSDLKeyStringCode(shootKey);
        }

        std::string GetSDLKeyStringCode(std::string key)
        {   
            if (key.compare("up") == 0) return "1073741906";
            if (key.compare("down") == 0) return "1073741905";
            if (key.compare("left") == 0) return "1073741904";
            if (key.compare("right") == 0) return "1073741903";
            if (key.compare("space") == 0) return "32";
            return std::to_string(static_cast<int>(key[0]));
        }

        void Initialize() override
        {
            m_entityName = m_owner->m_name;
            mp_transform = m_owner->GetComponent<TransformComponent>();
            mp_sprite = m_owner->GetComponent<SpriteComponent>();
            mp_projectile = m_owner->GetComponent<ProjectileEmitterComponent>();
        }

        void Update(float deltTime) override 
        {
            if (Game::m_event.type == SDL_KEYDOWN)
            {
                std::string keyCode = std::to_string(Game::m_event.key.keysym.sym);

                if (keyCode.compare(m_upKey) == 0)
                {
                    if (mp_transform->m_position.y > 0)
                    {
                        mp_transform->m_velocity.y = -80;
                        mp_transform->m_velocity.x = 0;
                        mp_transform->m_facingDeg = 270;
                        mp_sprite->Play("UpAnimation");
                    }
                    else
                    {
                        mp_transform->m_velocity.y = 0;
                    }
                }

                if (keyCode.compare(m_rightKey) == 0)
                {
                    if (mp_transform->m_position.x < (WINDOW_WIDTH * Game::mp_map->GetScale()) - mp_transform->m_width)
                    {
                     
                        mp_transform->m_velocity.y = 0;
                        mp_transform->m_velocity.x = 80;
                        mp_transform->m_facingDeg = 0;
                        mp_sprite->Play("RightAnimation");
                    }
                    else
                    {
                        mp_transform->m_velocity.x = 0;
                    }
                }

                if (keyCode.compare(m_downKey) == 0)
                {
                    if (mp_transform->m_position.y < (WINDOW_HEIGHT * Game::mp_map->GetScale()) - mp_transform->m_height)
                    {
                        mp_transform->m_velocity.y = 80;
                        mp_transform->m_velocity.x = 0;
                        mp_transform->m_facingDeg = 90;
                        mp_sprite->Play("DownAnimation");
                    }
                    else
                    {
                        mp_transform->m_velocity.y = 0;
                    }
                }

                if (keyCode.compare(m_leftKey) == 0)
                {
                    if (mp_transform->m_position.x > 0)
                    {
                        mp_transform->m_velocity.y = 0;
                        mp_transform->m_velocity.x = -80;
                        mp_transform->m_facingDeg = 180;
                        mp_sprite->Play("LeftAnimation");
                    }
                    else
                    {
                        mp_transform->m_velocity.x = 0;
                    }
                }

                if (keyCode.compare(m_shootKey) == 0)
                {   

                    m_owner->m_bang = true;
                    // m_owner->GetSibling("projectile")->GetComponent<ProjectileEmitterComponent>()->Shoot();
                }
            }

            if (Game::m_event.type == SDL_KEYUP)
            {
                std::string keyCode = std::to_string(Game::m_event.key.keysym.sym);

                if (keyCode.compare(m_upKey) == 0)
                {
                    mp_transform->m_velocity.y = 0;
                }
                if (keyCode.compare(m_rightKey) == 0)
                {
                    mp_transform->m_velocity.x = 0;
                }
                if (keyCode.compare(m_downKey) == 0)
                {
                    mp_transform->m_velocity.y = 0;
                }
                if (keyCode.compare(m_leftKey) == 0)
                {
                    mp_transform->m_velocity.x = 0;
                }
                if (keyCode.compare(m_shootKey) == 0)
                {
                    m_owner->m_bang = false;
                }

            }
        }
};

#endif
