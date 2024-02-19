#include "engine/Physics.h"



Vec2 Physics::GetOverlap(std::shared_ptr<Entity> e1, std::shared_ptr<Entity> e2) {
	if (e1->hasComponent<CBoundingBox>() && e1->hasComponent<CBoundingBox>() &&
		e2->hasComponent<CTransform>() && e2->hasComponent<CTransform>()) {

		Vec2& size1 = e1->getComponent<CBoundingBox>().size;
		Vec2& size2 = e2->getComponent<CBoundingBox>().size;

		Vec2& pos1 = e1->getComponent<CTransform>().pos;
		Vec2& pos2 = e2->getComponent<CTransform>().pos;

		float dx = fabsf(pos2.x - pos1.x - size1.x / 2.f + size2.x / 2.f);
		float dy = fabsf(pos2.y - pos1.y - size1.y / 2.f + size2.y / 2.f);

		return Vec2(
			(size1.x + size2.x) / 2.f - dx,
			(size1.y + size2.y) / 2.f - dy
		);
	}
	return Vec2();
}

Vec2 Physics::GetPreviousOverlap(std::shared_ptr<Entity> e1, std::shared_ptr<Entity> e2) {
	if (e1->hasComponent<CTransform>() && e1->hasComponent<CBoundingBox>() &&
		e2->hasComponent<CTransform>() && e2->hasComponent<CBoundingBox>()) {

		Vec2& size1 = e1->getComponent<CBoundingBox>().size;
		Vec2& size2 = e2->getComponent<CBoundingBox>().size;

		Vec2& pos1 = e1->getComponent<CTransform>().prevPos;
		Vec2& pos2 = e2->getComponent<CTransform>().prevPos;

		float dx = fabsf(pos2.x - pos1.x - size1.x / 2.f + size2.x / 2.f);
		float dy = fabsf(pos2.y - pos1.y - size1.y / 2.f + size2.y / 2.f);

		return Vec2(
			(size1.x + size2.x) / 2.f - dx,
			(size1.y + size2.y) / 2.f - dy
		);
	}
	return Vec2();
}

bool Physics::IsInside(Vec2 pos, std::shared_ptr<Entity> e) {
	auto& transform = e->getComponent<CTransform>();
	auto& size = e->getComponent<CAnimation>().animation.getSize();

	float dx = fabsf(transform.pos.x - pos.x + size.x / 2);
	float dy = fabsf(transform.pos.y - pos.y + size.y / 2);

	return (dx <= size.x / 2) && (dy <= size.y / 2);
}

Physics::Intersect Physics::LineIntersection(Vec2& a, Vec2& b, Vec2& c, Vec2& d) {
	auto l1 = b - a;
	auto l2 = d - c;
	auto ca = c - a;

	float l1Xl2 = l1.x * l2.y - l2.x * l1.y;
	float caXl1 = ca.x * l1.y - l1.x * ca.y;
	float caXl2 = ca.x * l2.y - l2.x * ca.y;

	float t = caXl1 / l1Xl2;
	float u = caXl2 / l1Xl2;
	if ((t > 1.0f || t < 0.0f) || (u > 1.0f || u < 1.0f)) {
		return Intersect(Vec2(), false);
	}

	return Intersect(l1 * t, true);
}

Physics::Intersect Physics::EntityIntersection(Vec2& a, Vec2& b, std::shared_ptr<Entity>& e) {
	auto& transform = e->getComponent<CTransform>();
	auto& size = e->getComponent<CBoundingBox>().size;

	Vec2 e1 = transform.pos; // top left
	Vec2 e2 = transform.pos + Vec2(size.x, 0); // top right
	Vec2 e3 = transform.pos + Vec2(size.x, -size.y); // bottom right
	Vec2 e4 = transform.pos + Vec2(0, -size.y); // bottom left

	auto i1 = LineIntersection(a, b, e1, e2);
	auto i2 = LineIntersection(a, b, e2, e3);
	auto i3 = LineIntersection(a, b, e3, e4);
	auto i4 = LineIntersection(a, b, e4, e1);

	if (!i1.isIntersect && !i2.isIntersect &&
		!i3.isIntersect && !i4.isIntersect) {
		return Intersect(b, false);
	}


	// possible intersections, i1 i2, i1 i3, i1 i4,
	// i2 i3, i2 i4, i3, i4
#pragma region finding_shortest_path
	Vec2 shortestPath;
	if (i1.isIntersect && i2.isIntersect) {
		if (dist(i1.point, a) < dist(i2.point, a)) {
			shortestPath = i1.point;
		}
		else {
			shortestPath = i2.point;
		}
	}
	if (i1.isIntersect && i3.isIntersect) {
		if (dist(i1.point, a) < dist(i3.point, a)) {
			shortestPath = i1.point;
		}
		else {
			shortestPath = i3.point;
		}
	}
	if (i1.isIntersect && i4.isIntersect) {
		if (dist(i1.point, a) < dist(i4.point, a)) {
			shortestPath = i1.point;
		}
		else {
			shortestPath = i4.point;
		}
	}
	if (i2.isIntersect && i3.isIntersect) {
		if (dist(i2.point, a) < dist(i3.point, a)) {
			shortestPath = i2.point;
		}
		else {
			shortestPath = i3.point;
		}
	}
	if (i2.isIntersect && i4.isIntersect) {
		if (dist(i2.point, a) < dist(i4.point, a)) {
			shortestPath = i2.point;
		}
		else {
			shortestPath = i4.point;
		}
	}
	if (i3.isIntersect && i4.isIntersect) {
		if (dist(i3.point, a) < dist(i4.point, a)) {
			shortestPath = i3.point;
		}
		else {
			shortestPath = i4.point;
		}
	}
#pragma endregion finding_shortest_path

	return Intersect(shortestPath, true);
}