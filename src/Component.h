#ifndef COMPONENT_H
#define COMPONENT_H

class Entity;

class Component
{
    public:
        Entity* m_owner;
        virtual ~Component() {}
        virtual void Initialize() {}
        virtual void Update(float deltaTime) {}
        virtual void Render() {}
};

#endif
