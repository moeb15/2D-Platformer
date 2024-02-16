#pragma once
#include <SFML/Graphics.hpp>
#include "Vec2.h"
#include "ObjectTypes.h"

class Animation {
private:
	std::size_t m_NumFrames;
	std::size_t m_CurrentFrame;
	std::size_t m_GameFrame;
	sf::Sprite m_Sprite;
	int m_Speed;
	Vec2 m_Size;
	Animations::Type m_Type;
	bool m_Repeat;

public:
	Animation() {};
	Animation(std::size_t num, sf::Texture& t, int s, Vec2 size, Animations::Type type);

	void update();
	bool hasEnded();
	Animations::Type& getType();
	Vec2& getSize();
	sf::Sprite& getSprite();
	void setRepeat(bool flag);

};