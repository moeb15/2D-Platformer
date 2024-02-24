#pragma once
#include "Animation.h"
#include "ResourceIdentifiers.h"
#include "ResourceHolder.h"
#include "ObjectTypes.h"
#include <map>
#include <memory>


class Assets {
private:
	TextureHolder m_Textures;
	FontHolder m_Fonts;
	SoundHolder m_Sounds;
	std::map<Animations::Type, std::unique_ptr<Animation>> m_Animations;

public:
	Assets();

	void addTexture(Textures::ID, const std::string&);
	void addSoundBuffer(Sounds::ID, const std::string&);
	void addFont(Fonts::ID, const std::string&);
	void addAnimation(Animations::Type);

	sf::Texture& getTexture(Textures::ID);
	sf::Font& getFont(Fonts::ID);
	sf::SoundBuffer& getSoundBuffer(Sounds::ID);
	Animation& getAnimation(Animations::Type);
};