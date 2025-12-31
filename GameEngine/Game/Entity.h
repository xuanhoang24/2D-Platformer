#ifndef ENTITY_H
#define ENTITY_H

#include "../Core/StandardIncludes.h"
#include <unordered_map>
#include <typeindex>
#include <memory>

using EntityID = uint32_t;
constexpr EntityID INVALID_ENTITY = 0;

struct Component
{
    virtual ~Component() = default;
};

/**
 * Entity is a container holds components.
 * 
 * Component-based architecture:
 * - Entity = unique ID + collection of components
 * - Components = data only (position, health, sprite, etc.)
 * - Systems = logic that operates on entities with specific components
 */

class Entity
{
public:
    Entity();
    ~Entity();

    EntityID GetID() const { return m_id; }
    bool IsValid() const { return m_id != INVALID_ENTITY; }
    void SetActive(bool active) { m_isActive = active; }
    bool IsActive() const { return m_isActive; }

    // Add a component of type T to this entity
    template<typename T, typename... Args>
    T* AddComponent(Args&&... args)
    {
        auto comp = std::make_unique<T>(std::forward<Args>(args)...);
        T* ptr = comp.get();
        m_components[std::type_index(typeid(T))] = std::move(comp);
        return ptr;
    }

    // Get a component of type T, returns nullptr if not found
    template<typename T>
    T* GetComponent()
    {
        auto it = m_components.find(std::type_index(typeid(T)));
        return it != m_components.end() ? static_cast<T*>(it->second.get()) : nullptr;
    }

    // Check if entity has a component of type T
    template<typename T>
    bool HasComponent() const
    {
        return m_components.find(std::type_index(typeid(T))) != m_components.end();
    }

    // Remove a component of type T
    template<typename T>
    void RemoveComponent() { m_components.erase(std::type_index(typeid(T))); }

private:
    EntityID m_id;
    bool m_isActive;
    std::unordered_map<std::type_index, std::unique_ptr<Component>> m_components;
    static EntityID s_nextID;
};

#endif
