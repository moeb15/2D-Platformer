#include "engine/Animation.h"
#include <iostream>


Animation::Animation(std::size_t num, sf::Texture& sprite,
	int s, Vec2 size, Animations::Type type) :
	m_NumFrames(num),
	m_CurrentFrame(0),
	m_GameFrame(0),
	m_Sprite(sprite),
	m_Speed(s),
	m_Type(type),
	m_Size(size),
	m_Repeat(false) {}


void Animation::update() {
	m_GameFrame++;
	if (m_Type == Animations::Run) {
		//std::cout << m_GameFrame << std::endl;
		//std::cout << m_CurrentFrame << std::endl;
	}
	if (m_CurrentFrame < m_NumFrames) {
		sf::IntRect curTexture(m_Size.x * m_CurrentFrame, 0, m_Size.x, m_Size.y);
		m_Sprite.setTextureRect(curTexture);
		if (m_Repeat) {
			m_CurrentFrame = (m_GameFrame/m_Speed) % m_NumFrames;
		}
		else {
			m_CurrentFrame = m_GameFrame/m_Speed;
		}
	}
}

void Animation::setRepeat(bool flag) {
	m_Repeat = flag;
}

bool Animation::hasEnded() {
	return m_CurrentFrame == m_NumFrames;
}

Animations::Type& Animation::getType() {
	return m_Type;
}

Vec2& Animation::getSize() {
	return m_Size;
}

sf::Sprite& Animation::getSprite() {
	return m_Sprite;
}