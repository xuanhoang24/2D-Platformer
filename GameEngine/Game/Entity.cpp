#include "Entity.h"

// Static counter for generating unique entity IDs
// Each new entity gets an incrementing ID (1, 2, 3, ...)
EntityID Entity::s_nextID = 1;

// Constructor: assigns unique ID and sets entity as active
Entity::Entity() : m_id(s_nextID++), m_isActive(true) {}

// Destructor: clears all components (unique_ptr handles deletion)
Entity::~Entity() { m_components.clear(); }
