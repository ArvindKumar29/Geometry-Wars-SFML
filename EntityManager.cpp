#include "EntityManager.h"
#include <algorithm>

EntityManager::EntityManager() { void init(); }
void EntityManager::init(){}

// Updating game
void EntityManager::update() {
	// Removing dead entities from game
    m_entities.erase(std::remove_if(m_entities.begin(), m_entities.end(), [](std::shared_ptr<Entity>& e) {return !e->isAlive(); }), m_entities.end());
	
	// Adding new Entities to game
	for (auto& e : toAdd) {
		m_entities.push_back(e);
		m_entityMap[e->getTag()].push_back(e);
	}
	toAdd.clear();
}

// Preparing entities to add in the next frame
std::shared_ptr<Entity> EntityManager::addEntity(std::string tag) {
	auto e = std::make_shared<Entity>(tag, m_totalEntities++) ;
	toAdd.push_back(e);
	return e;
}

// getting the vector containing all the entities
std::vector<std::shared_ptr<Entity>> EntityManager::getEntities(std::string tag) {
	std::vector<std::shared_ptr<Entity>> vec;
	for (auto& e : m_entities) {
		if (e->getTag() == tag) {
			vec.push_back(e);
		}
	}
	return vec;
}

std::vector<std::shared_ptr<Entity>> EntityManager::getEntities() {
	return m_entities;
}

int EntityManager::getTotalEntities() {
	return m_totalEntities;
}