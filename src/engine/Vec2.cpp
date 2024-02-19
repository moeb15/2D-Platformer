#include "engine/Vec2.h"
#include <cmath>


bool operator==(const Vec2& v, const Vec2& u) {
	if (v.x == u.x && v.y == u.y) {
		return true;
	}
	return false;
}

bool operator!=(const Vec2& v, const Vec2& u) {
	return !(v == u);
}

Vec2 operator+(const Vec2& v, const Vec2& u)  {
	Vec2 res;
	res.x = v.x + u.x;
	res.y = v.y + u.y;
	return res;
}

Vec2 operator-(const Vec2& v, const Vec2& u) {
	Vec2 res;
	res.x = v.x - u.x;
	res.y = v.y - u.y;
	return res;
}

float operator*(const Vec2& v, const Vec2& u) {
	return v.x * u.x + v.y * u.y;
}

float operator/(const Vec2& v, const Vec2& u) {
	return v.x * u.y - u.x * v.y;
}

Vec2 operator*(const Vec2& v, float f) {
	return Vec2(v.x * f, v.y * f);
}

Vec2 operator/(const Vec2& v, float f) {
	if (f == 0) {
		return Vec2();
	}
	return Vec2(v.x / f, v.y / f);
}

void normalize(Vec2& v) {
	float lenSq = v.x * v.x + v.y * v.y;
	if (lenSq == 0) {
		return;
	}	
	float len = sqrtf(lenSq);
	v.x /= len;
	v.y /= len;
}


float lenSq(Vec2& v) {
	return v.x * v.x + v.y * v.y;
}

float len(Vec2& v) {
	float lenSq = v.x * v.x + v.y * v.y;
	return sqrtf(lenSq);
}

float dist(Vec2& v, Vec2& u) {
	Vec2 r = v - u;
	return len(r);
}