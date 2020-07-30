#ifndef ENTITY_H
#define ENTITY_H

#include <map>
#include <string>
#include <vector>

#include "./Component.h"
#include "./Constants.h"
#include "./EntityManager.h"

class EntityManager;
class Component;

class Entity
{
    public:
        std::string m_name;
        std::vector <Component*> m_components;
        LayerType m_layer;
        bool m_bang;

        Entity(EntityManager& manager);
        Entity(std::string name, LayerType layer, EntityManager& manager);
        void DisplayContents() const;
        void Update(float deltaTime);
        void Render();
        void Destroy();
        bool IsActive() const;

        void AddSibling(Entity* sibling)
        {
            m_siblingMap[sibling->m_name] = sibling;
        }

        Entity* GetSibling(std::string siblingName)
        {
            return m_siblingMap[siblingName];
        }

        EntityManager& GetEntityManager()
        {
            return m_manager;
        }

        bool HasSibling(std::string siblingName) const
        {
            return !(m_siblingMap.find(siblingName) == m_siblingMap.end());
        }
        
        template <typename T, typename... TArgs>
        T& AddComponent(TArgs&&... args)
        {
            T* newComponent(new T(std::forward<TArgs>(args)...));

            newComponent->m_owner = this;
            m_components.emplace_back(newComponent);
            m_componentTypeMap[&typeid(*newComponent)] = newComponent;
            newComponent->Initialize();

            return * newComponent;
        }

        template <typename T>
        T* GetComponent()
        {
            return static_cast<T*>(m_componentTypeMap[&typeid(T)]);
        }

        template <typename T>
        bool HasComponent() const
        {
            return !(m_componentTypeMap.find(&typeid(T)) == m_componentTypeMap.end());
        }

    private:
        EntityManager& m_manager;
        bool m_isActive;
        std::map<const std::type_info*, Component*> m_componentTypeMap;
        std::map<const std::string, Entity*> m_siblingMap;
};

#endif
