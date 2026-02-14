#pragma once
#include "Component.h"
#include <string>
class Entity
{
	friend class EntityManager;
	std::string m_tag;
	bool m_Alive;
	size_t m_id;
public:
	std::shared_ptr<Component::CShape> CShape;
	std::shared_ptr<Component::CTransform> CTransform;
	std::shared_ptr<Component::CCollision> CCollision;
	std::shared_ptr<Component::CInput> CInput;
	std::shared_ptr<Component::CLifespan> CLifespan;
	std::shared_ptr <Component::CHealth> CHealth;

	Entity(std::string tag, size_t id){
		m_tag = tag;
		m_id = id;
		m_Alive = true;
	}

	void destroy();
	bool isAlive();
	std::string& getTag();
	size_t& getID();
};

