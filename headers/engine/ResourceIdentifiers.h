#pragma once
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>

namespace Textures {
	enum ID{
		Idle,
		IdleLeft,
		Run,
		RunLeft,
		Jump,
		JumpLeft,
		Shoot,
		ShootLeft,
		Brick,
		Ground,
		Explosion,
		QuestionBox,
		Enemy,
		Background,
		Menu,
		Blast,
		BlastLeft,
	};
}

namespace Fonts {
	enum ID{
		Main,
		Edit,
	};
}

namespace Sounds {
	enum ID{
		Main,
	};
}

template<typename Resource, typename Identifier>
class ResourceHolder;

typedef ResourceHolder<sf::Texture, Textures::ID> TextureHolder;
typedef ResourceHolder<sf::Font, Fonts::ID> FontHolder;
typedef ResourceHolder<sf::SoundBuffer, Sounds::ID> SoundHolder;