#include "engine/EntityManager.h"



EntityManager::EntityManager() {
	init();
}

void EntityManager::init() {
	m_Entities.clear();
	m_EntityMap.clear();
	m_ToAdd.clear();
	m_TotalEntites = 0;
}

void EntityManager::removeDeadEntities(EntityVec& v) {
	v.erase(
		std::remove_if(v.begin(), v.end(),
			[&](std::shared_ptr<Entity> obj) {return obj->isActive() == false; }),
		v.end()
	);
}

std::shared_ptr<Entity> EntityManager::addEntity(const Entities::Type& tag) {
	std::shared_ptr<Entity> e(new Entity(tag, m_TotalEntites++));
	m_ToAdd.push_back(e);
	return e;
}

EntityManager::EntityVec& EntityManager::getEntities() {
	return m_Entities;
}

EntityManager::EntityVec& EntityManager::getEntities(const Entities::Type& tag) {
	return m_EntityMap[tag];
}

void EntityManager::update() {
	for (auto& e : m_ToAdd) {
		m_Entities.push_back(e);
		m_EntityMap[e->tag()].push_back(e);
	}
	m_ToAdd.clear();
	
	removeDeadEntities(m_Entities);
	for (auto& kvPair : m_EntityMap) {
		removeDeadEntities(kvPair.second);
	}
}