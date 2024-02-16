#pragma once
#include "Entity.h"
#include "ObjectTypes.h"
#include <vector>
#include <map>

class EntityManager {
public:
	typedef std::vector<std::shared_ptr<Entity>> EntityVec;
	typedef std::map<Entities::Type, EntityVec> EntityMap;

private:
	EntityVec m_Entities;
	EntityMap m_EntityMap;
	EntityVec m_ToAdd;
	std::size_t m_TotalEntites = 0;
	
	void init();
public:
	EntityManager();
	void update();
	void removeDeadEntities(EntityVec& v);
	std::shared_ptr<Entity> addEntity(const Entities::Type& tag);
	EntityVec& getEntities();
	EntityVec& getEntities(const Entities::Type& tag);

};