#include "Action.h"


Action::Action() :
	m_Name(Actions::NoneType),
	m_Type(Actions::None) {}

Action::Action(const Actions::Name& name, const Actions::Type& type) :
	m_Name(name),
	m_Type(type) {}

Action::Action(const Actions::Name& name, const Actions::Type& type, const Vec2& pos) :
	m_Name(name),
	m_Type(type),
	m_Pos{ pos } {}

const Actions::Type& Action::getType() const {
	return m_Type;
}

const Actions::Name& Action::getName() const {
	return m_Name;
}

const Vec2& Action::getPos() const {
	return m_Pos;
}