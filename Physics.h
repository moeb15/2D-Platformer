#pragma once
#include "Entity.h"
#include "Vec2.h"
#include <memory>


class Physics {
public:
	static Vec2 GetOverlap(std::shared_ptr<Entity>, std::shared_ptr<Entity>);
	static Vec2 GetPreviousOverlap(std::shared_ptr<Entity>, std::shared_ptr<Entity>);

	Physics() = delete;
};