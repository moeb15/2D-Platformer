#pragma once
#include <SFML/Graphics.hpp>
#include "Vec2.h"
#include "Animation.h"


class CDraggable {
public:
	bool dragging = false;
	bool has = false;

	CDraggable(){}
};

class CTransform {
public:
	Vec2 pos;
	Vec2 prevPos;
	Vec2 velocity;
	Vec2 scale;
	bool has = false;
	float angle = 0.0f;

	CTransform(){}
	CTransform(const Vec2& p, Vec2& pp, const Vec2& v,
		const Vec2& s,const float& a) : 
		pos(p), velocity(v),
		scale(s), angle(a), prevPos(pp), has(true) {}
};

class CShape {
public:
	sf::CircleShape circle;

	CShape():
	circle(){}
};

class CAnimation {
public:
	Animation animation;
	bool has = false;

	CAnimation(){}
	CAnimation(const Animation& a) :
		animation(a) {}
	/*CAnimation(std::size_t num, sf::Texture& t, int s, Vec2 size, Animations::Type type) :
		animation(Animnum, t, s, size, type),
		has(true) {}*/
};

class CBoundingBox {
public:
	Vec2 size;
	bool has = false;
	
	CBoundingBox(){}
	CBoundingBox(const Vec2& s):
		size(s),
		has(true) {}
};

class CLifespan {
public:
	int remaining = 0;
	int total = 0;
	bool has = false;

	CLifespan(){}
	CLifespan(int r, int t) :
		remaining(r), total(t),
		has(true) {}
};

class CInput {
public:
	bool up = false;
	bool down = false;
	bool left = false;
	bool right = false;
	bool shoot = false;
	bool has = false;

	CInput() {}
	CInput(bool):
		has(true) {}
};

class CGravity {
public:
	float gravity = 2000.f;
	bool has = false;

	CGravity(){}
};

class CState {
public:
	States::Type state = States::Ground;
	bool has = false;

	CState(){}
};

class CSpeed {
public:
	float maxSpeed;
	bool has = false;

};

class CLevel {
public:
	std::string path;
	bool has = false;

};