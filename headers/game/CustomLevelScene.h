#pragma once
#include "engine/Scene.h"
#include <vector>
#include <string>
#include <filesystem>
#include <SFML/Graphics.hpp>


class CustomLevelScene : public Scene {
private:
	sf::RectangleShape m_Background;
	sf::Text m_Title;
	sf::Text m_Subtext;
	std::vector<sf::Text> m_Options;
	std::size_t m_OptionIndex;
	sf::View m_LevelSelectView;
	std::vector<std::string> m_Levels;

public:
	CustomLevelScene(GameEngine* gameEngine);

	virtual void update(float dt);
	virtual void sDoAction(const Action&);
	virtual void sRender();

private:
	void init();
	void getLevels();
};