#pragma once

struct Vec2 {
	inline Vec2(float _x, float _y) : x(_x), y(_y) {}
	inline Vec2() :x(0.0f), y(0.0f) {}
	float x;
	float y;
};

bool operator==(const Vec2& v, const Vec2& u);
Vec2 operator+(const Vec2& v, const Vec2& u);
Vec2 operator-(const Vec2& v, const Vec2& u);
float operator*(const Vec2& v, const Vec2& u);
float operator/(const Vec2& v, const Vec2& u);
Vec2 operator*(const Vec2& v, float f);
Vec2 operator/(const Vec2& v, float f);

void normalize(Vec2& v);
float lenSq(Vec2& v);
float len(Vec2& v);
float dist(Vec2& v, Vec2& u);