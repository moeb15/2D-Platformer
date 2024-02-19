#pragma once
#include "engine/Scene.h"
#include <vector>
#include <string>
#include <SFML/Graphics.hpp>


class MenuScene : public Scene {
private:
	sf::Sprite m_Background;
	sf::Text m_Title;
	sf::Text m_Subtext;
	std::vector<sf::Text> m_Options;
	std::size_t m_OptionIndex;
	sf::View m_MenuView;

	std::string m_LevelPaths[3] = {
		"C:/Projects/IsometricShooter/levels/level1.txt",
		"C:/Projects/IsometricShooter/levels/level2.txt",
		"C:/Projects/IsometricShooter/levels/level3.txt"
	};

public:
	MenuScene(GameEngine* gameEngine);

	virtual void update(float dt);
	virtual void sDoAction(const Action&);
	virtual void sRender();

private:
	void init();
};