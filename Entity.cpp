#include "Entity.h"

void Entity::destroy() {
	m_Alive = false;
};

bool Entity::isAlive() {
	return m_Alive;
};

std::string& Entity::getTag() {
	return m_tag;
};

size_t& Entity::getID() {
	return m_id;
};