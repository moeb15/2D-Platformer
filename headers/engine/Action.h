#pragma once
#include "ObjectTypes.h"
#include "Vec2.h"

class Action {
private:
	Actions::Name m_Name;
	Actions::Type m_Type;
	Vec2 m_Pos;

public:
	Action();
	Action(const Actions::Name& name, const Actions::Type& type);
	Action(const Actions::Name& name, const Actions::Type& type, const Vec2& pos);

	const Actions::Type& getType() const;
	const Actions::Name& getName() const;
	const Vec2& getPos() const;
};