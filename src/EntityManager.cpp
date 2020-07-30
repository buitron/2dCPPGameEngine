#include "./EntityManager.h"
#include "./Collision.h"
#include "./Components/ColliderComponent.h"

void EntityManager::ListAllEntities() const
{
    for (auto &entity: m_entities)
    {
        entity->DisplayContents();
    }
}

void EntityManager::ClearData()
{
    for (auto &entity: m_entities)
    {
        entity->Destroy();
    }
}

bool EntityManager::HasNoEntities()
{
    return m_entities.size() == 0;
}

void EntityManager::Update(float deltaTime)
{
    for (int i = 0; i < static_cast<int>(m_entities.size()); ++i)
    {
        m_entities[i]->Update(deltaTime);
        DestroyInactiveEntities(m_entities[i], i);
    }
}

void EntityManager::DestroyInactiveEntities(Entity* p_entity, int iterPos)
{
    if (!p_entity->IsActive())
    {
        m_entities.erase(m_entities.begin() + iterPos);
    }
}

void EntityManager::Render()
{
    for (unsigned int layerNumber = 0; layerNumber < NUM_LAYERS; ++layerNumber)
    {
        for (auto& entity: GetEntitiesByLayer(static_cast<LayerType>(layerNumber)))
        {
            entity->Render();
        }
    }
}

CollisionType EntityManager::CheckCollisions() const
{
    ColliderComponent* p_thisCollider;
    ColliderComponent* p_thatCollider;
    bool firstSearch = true;
    
    for(auto& entity: m_entities)
    {
        if (entity->HasComponent<ColliderComponent>())
        {
            if (firstSearch)
            {
                p_thisCollider = entity->GetComponent<ColliderComponent>();
                firstSearch = false;
            }
            else
            {
                p_thatCollider = entity->GetComponent<ColliderComponent>();
    
                if (Collision::CheckRectangleCollision(p_thisCollider->m_collider, p_thatCollider->m_collider))
                {
                    if (p_thisCollider->m_colliderTag.compare("player") == 0 && p_thatCollider->m_colliderTag.compare("enemy") == 0) return PLAYER_ENEMY_COLLISION;
                    if (p_thisCollider->m_colliderTag.compare("player") == 0 && p_thatCollider->m_colliderTag.compare("projectile") == 0) return PLAYER_PROJECTILE_COLLISION;
                    if (p_thisCollider->m_colliderTag.compare("enemy") == 0 && p_thatCollider->m_colliderTag.compare("friendly-projectile") == 0) return ENEMY_PROJECTILE_COLLISION;
                    if (p_thisCollider->m_colliderTag.compare("player") == 0 && p_thatCollider->m_colliderTag.compare("level-complete") == 0) return PLAYER_LEVEL_COMPLETE_COLLISION;
                }
            }
        }
    }
    return NO_COLLISION;
}

Entity& EntityManager::AddEntity(std::string entityName, LayerType layer)
{
    Entity* entity = new Entity(entityName, layer, *this);
    m_entities.emplace_back(entity);
    return *entity;
}

std::vector<Entity*> EntityManager::GetEntities() const
{
    return m_entities;
}

std::vector<Entity*> EntityManager::GetEntitiesByLayer(LayerType layer) const
{
    std:: vector<Entity*> selectedEntities;
    for (auto& entity: m_entities)
    {
        if (entity->m_layer == layer)
        {
            selectedEntities.emplace_back(entity);
        }
    }
    return selectedEntities;
}

unsigned int EntityManager::GetEntityCount()
{
    return m_entities.size();
}
