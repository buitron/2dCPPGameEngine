#ifndef PROJECTILEEMITTERCOMPONENT_H
#define PROJECTILEEMITTERCOMPONENT_H

#include "../../lib/glm/glm.hpp"
#include "../EntityManager.h"
#include "../Components/TransformComponent.h"

class ProjectileEmitterComponent : public Component
{
    public:
        Entity* mp_parent;

        ProjectileEmitterComponent(Entity& parent, int speed, int range, bool shouldLoop)
        {
            mp_parent = &parent;
            m_speed = speed;
            m_range = range;
            m_shouldLoop = shouldLoop;
        }

        void Initialize() override
        {
            mp_transform = m_owner->GetComponent<TransformComponent>();
            mp_parentTransform = mp_parent->GetComponent<TransformComponent>();
            m_origin = glm::vec2(mp_transform->m_position.x, mp_transform->m_position.y);
            Direction();
        }

        void Update(float deltaTime) override
        {   
            if (glm::distance(mp_transform->m_position, m_origin) > m_range)
            {   
                Direction();
                m_origin = glm::vec2(mp_parentTransform->m_position.x, mp_parentTransform->m_position.y);

                if (m_shouldLoop)
                {
                    mp_transform->m_position.x = mp_parentTransform->m_position.x + (mp_parentTransform->m_width/2);
                    mp_transform->m_position.y = mp_parentTransform->m_position.y + (mp_parentTransform->m_height/2);
                }
                else
                {
                    m_owner->Destroy();
                }
            }
        }

        void Direction()
        {
            float angleRad = glm::radians(static_cast<float>(mp_parentTransform->m_facingDeg));
            mp_transform->m_velocity = glm::vec2(glm::cos(angleRad) * m_speed, glm::sin(angleRad) * m_speed);
        }

    private:
        TransformComponent* mp_transform;
        TransformComponent* mp_parentTransform;
        glm::vec2 m_origin;
        int m_speed;
        int m_range;
        float m_angleRad;
        bool m_shouldLoop;
};

#endif
