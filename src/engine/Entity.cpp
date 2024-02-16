#include "engine/Entity.h"


Entity::Entity(const Entities::Type& tag,
	const std::size_t id) :
	m_Tag(tag),
	m_ID(id),
	m_Active(true) {
	
/*std::get<CTransform>(m_Components) = CTransform();
	std::get<CBoundingBox>(m_Components) = CBoundingBox();
	std::get<CInput>(m_Components) = CInput();
	std::get<CAnimation>(m_Components) = CAnimation();
	std::get<CLifespan>(m_Components) = CLifespan();
	std::get<CGravity>(m_Components) = CGravity();*/
}


void Entity::destroy() {
	m_Active = false;
}

bool Entity::isActive() const {
	return m_Active;
}

const Entities::Type& Entity::tag() const {
	return m_Tag;
}

const std::size_t Entity::id() const{
	return m_ID;
}