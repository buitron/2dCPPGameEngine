#ifndef ENTITYMANAGER_H
#define ENTITYMANAGER_H

#include <vector>

#include "./Component.h"
#include "./Constants.h"
#include "./Entity.h"

class Entity;

class EntityManager
{
    public:
        void ListAllEntities() const;
        void ClearData();
        void DestroyInactiveEntities(Entity* p_entity, int iterPosition);
        void Update(float deltaTime);
        void Render();
        bool HasNoEntities();
        CollisionType CheckCollisions() const;
        Entity& AddEntity(std::string entityName, LayerType layer);
        std::vector<Entity*> GetEntities() const;
        std::vector<Entity*> GetEntitiesByLayer(LayerType layer) const;
        unsigned int GetEntityCount();
    private:
        std::vector<Entity*> m_entities;
};


#endif
