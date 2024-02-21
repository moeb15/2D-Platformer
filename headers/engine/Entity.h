#pragma once
#include "Components.h"
#include "ObjectTypes.h"
#include <memory>
#include <string>
#include <tuple>


typedef std::tuple<
	CTransform,
	CLifespan,
	CBoundingBox,
	CInput,
	CAnimation,
	CGravity,
	CState,
	CShape,
	CDraggable,
	CSpeed,
	CLevel,
	CAI
> ComponentTuple;


class Entity {
	friend class EntityManager;
private:
	Entities::Type m_Tag = Entities::Default;
	bool m_Active = true;
	std::size_t m_ID = 0;
	ComponentTuple m_Components;

	Entity(const Entities::Type& tag,
		const std::size_t id);
public:

	
	void destroy();
	bool isActive() const;
	const Entities::Type& tag() const;
	const std::size_t id() const;

	template<typename T, typename... TArgs>
	void addComponent(TArgs&&... mArgs) {
		auto& component = getComponent<T>();
		component = T(std::forward<TArgs>(mArgs)...);
		component.has = true;
	}
	
	template<typename T>
	T& getComponent() {
		return std::get<T>(m_Components);
	}

	template<typename T>
	bool hasComponent() {
		return getComponent<T>().has;
	}
};
