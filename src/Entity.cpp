#include <algorithm>
#include <cctype>
#include <iostream>
#include <string>

#include "./Entity.h"

Entity::Entity(EntityManager& manager): m_manager(manager)
{
    m_isActive = true;
    m_bang = false;
}

Entity::Entity(std::string name, LayerType layer, EntityManager& manager): m_name(name), m_layer(layer), m_manager(manager)
{
    m_isActive = true;
    m_bang = false;
}

void Entity::DisplayContents() const
{
    if (m_name.compare("Tile") != 0)
    {
        std::cout << "Entity Name: " << m_name << std::endl;
        for (auto mapElement: m_componentTypeMap)
        {
            std::string componentName = mapElement.first->name();
            componentName.erase(std::remove_if(std::begin(componentName), std::end(componentName), &isdigit), componentName.end());
            std::cout << "\tComponent <" << componentName << ">" << std::endl;
        }
    }
}

void Entity::Update(float deltaTime)
{
    for (auto& component: m_components)
    {
        component->Update(deltaTime);
    }
}

void Entity::Render()
{
    for (auto& component: m_components)
    {
        component->Render();
    }
}

void Entity::Destroy()
{
    m_isActive = false;
}

bool Entity::IsActive() const
{
    return m_isActive;
}
