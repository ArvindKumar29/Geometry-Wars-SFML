#pragma once
#include "Entity.h"
#include <vector>

class EntityManager
{
	std::vector<std::shared_ptr<Entity>> m_entities;
	std::vector<std::shared_ptr<Entity>> toAdd;
	std::map<std::string, std::vector<std::shared_ptr<Entity>>> m_entityMap;
	int m_totalEntities = 0;
	void init();

public:
	EntityManager();
	void update();
	std::shared_ptr<Entity> addEntity(std::string tag);
	std::vector<std::shared_ptr<Entity>> getEntities(std::string tag);
	std::vector<std::shared_ptr<Entity>> getEntities();
	int getTotalEntities();

};

