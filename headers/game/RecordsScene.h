#pragma once
#include "engine/Scene.h"
#include <vector>
#include <string>
#include <filesystem>
#include <unordered_map>
#include <SFML/Graphics.hpp>


class RecordsScene : public Scene {
private:
	sf::Sprite m_Background;
	sf::Text m_Title;
	sf::Text m_Subtext;
	std::vector<sf::Text> m_Records;
	sf::View m_RecordsView;
	std::unordered_map<std::string, 
		std::vector<float>> m_Levels;

public:
	RecordsScene(GameEngine* gameEngine);

	virtual void update(float dt);
	virtual void sDoAction(const Action&);
	virtual void sRender();

private:
	void init();
	void getRecords();
};