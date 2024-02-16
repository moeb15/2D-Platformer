#include "engine/Physics.h"



Vec2 Physics::GetOverlap(std::shared_ptr<Entity> e1, std::shared_ptr<Entity> e2) {
	if (e1->hasComponent<CBoundingBox>() && e1->hasComponent<CBoundingBox>() &&
		e2->hasComponent<CTransform>() && e2->hasComponent<CTransform>()) {

		Vec2& size1 = e1->getComponent<CBoundingBox>().size;
		Vec2& size2 = e2->getComponent<CBoundingBox>().size;

		Vec2& pos1 = e1->getComponent<CTransform>().pos;
		Vec2& pos2 = e2->getComponent<CTransform>().pos;

		float dx = fabsf(pos2.x - pos1.x);
		float dy = fabsf(pos2.y - pos1.y);

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

		float dx = fabsf(pos2.x - pos1.x);
		float dy = fabsf(pos2.y - pos1.y);

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
	//std::cout << size.x << " " << size.y << std::endl;

	float dx = fabsf(transform.pos.x - pos.x + size.x / 2);
	float dy = fabsf(transform.pos.y - pos.y + size.y / 2);

	return (dx <= size.x / 2) && (dy <= size.y / 2);
}