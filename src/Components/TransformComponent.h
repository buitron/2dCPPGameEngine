#ifndef TRANSFORMERCOMPONENT_H
#define TRANSFORMERCOMPONENT_H

#include <iostream>
#include <string>
#include <SDL2/SDL.h>

#include "../EntityManager.h"
#include "../Component.h"
#include "../../lib/glm/glm.hpp"

class TransformComponent : public Component
{
    public:
        glm::vec2 m_position;
        glm::vec2 m_velocity;
        int m_width;
        int m_height;
        int m_scale;
        int m_facingDeg;

        TransformComponent(int posX, int posY, int velX, int velY, int w, int h, int s)
        {
            m_position = glm::vec2(posX, posY);
            m_velocity = glm::vec2(velX, velY);
            m_width = w;
            m_height = h;
            m_scale = s;
        }

        TransformComponent(int posX, int posY, int velX, int velY, int facingDeg, int w, int h, int s)
        {
            m_position = glm::vec2(posX, posY);
            m_velocity = glm::vec2(velX, velY);
            m_facingDeg = facingDeg;
            m_width = w;
            m_height = h;
            m_scale = s;
        }

        void Initialize() override {}

        void Update(float deltaTime) override
        {
            m_position.x += m_velocity.x * deltaTime;
            m_position.y += m_velocity.y * deltaTime;
        }

        void Render() override {}
};

#endif
