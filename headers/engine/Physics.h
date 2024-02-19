#pragma once
#include "Entity.h"
#include "Vec2.h"
#include <memory>


class Physics {
public:
	struct Intersect {
		Intersect(Vec2 pos, bool intersect):
			point(pos),
			isIntersect(intersect){}

		bool isIntersect = false;
		Vec2 point;
	};

	static Vec2 GetOverlap(std::shared_ptr<Entity>, std::shared_ptr<Entity>);
	static Vec2 GetPreviousOverlap(std::shared_ptr<Entity>, std::shared_ptr<Entity>);
	static bool IsInside(Vec2 pos, std::shared_ptr<Entity> e);
	static Intersect LineIntersection(Vec2& a, Vec2& b, Vec2& c, Vec2& d);
	static Intersect EntityIntersection(Vec2& a, Vec2& b, std::shared_ptr<Entity>& e);

	Physics() = delete;
};