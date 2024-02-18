#include "engine/Assets.h"
#include "engine/Datatables.h"


namespace {
	const std::vector<AnimationData> Table = initializeAnimationData();
}

Textures::ID toTextureID(Animations::Type type) {
	switch (type) {
	case Animations::Idle:
		return Textures::Idle;

	case Animations::IdleLeft:
		return Textures::IdleLeft;

	case Animations::Run:
		return Textures::Run;

	case Animations::RunLeft:
		return Textures::RunLeft;

	case Animations::Jump:
		return Textures::Jump;

	case Animations::Shoot:
		return Textures::Shoot;

	case Animations::Tile:
		return Textures::Brick;

	case Animations::Ground:
		return Textures::Ground;

	case Animations::QuestionBox:
		return Textures::QuestionBox;

	case Animations::Explosion:
		return Textures::Explosion;

	case Animations::Enemy:
		return Textures::Enemy;
	}
}

Assets::Assets(){}

void Assets::addAnimation(Animations::Type type) {
	Textures::ID textureID = toTextureID(type);
	sf::Texture& texture = getTexture(textureID);
	std::size_t frameCount = Table[type].frameCount;
	int duration = Table[type].duration;

	Vec2 frameSize;
	frameSize.x = texture.getSize().x / frameCount;
	frameSize.y = (int)texture.getSize().y;

	std::unique_ptr<Animation> newAnimation(
		new Animation(frameCount, texture, duration, frameSize, type)
	);
	m_Animations.insert(std::make_pair(type, std::move(newAnimation)));
}

void Assets::addTexture(Textures::ID id, const std::string& file) {
	m_Textures.add(id, file);
}

void Assets::addFont(Fonts::ID id, const std::string& file) {
	m_Fonts.add(id, file);
}

void Assets::addSound(Sounds::ID id, const std::string& file) {
	m_Sounds.add(id, file);
}

sf::Texture& Assets::getTexture(Textures::ID id) {
	return m_Textures.get(id);
}

sf::Font& Assets::getFont(Fonts::ID id) {
	return m_Fonts.get(id);
}

sf::Sound& Assets::getSound(Sounds::ID id) {
	sf::Sound sound;
	sound.setBuffer(m_Sounds.get(id));
	return sound;
}

Animation& Assets::getAnimation(Animations::Type type) {
	auto found = m_Animations.find(type);
	if (found == m_Animations.end()) {
		throw std::exception("Assets::getAnimation, animation not found");
	}

	return *found->second;
}