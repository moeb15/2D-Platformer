#include "game/RecordsScene.h"
#include "engine/GameEngine.h"
#include "game/MenuScene.h"
#include <fstream>
#include <iostream>
#include <sstream>

RecordsScene::RecordsScene(GameEngine* gameEngine) :
	Scene(gameEngine),
	m_Records(),
	m_Title(),
	m_Subtext(),
	m_RecordsView(m_GameEngine->getWindow().getDefaultView()) {

	init();
}

void RecordsScene::init() {
	m_GameEngine->getAssets().addTexture(Textures::Menu, "graphics/menuScreen.png");
	m_Background.setTexture(m_GameEngine->getAssets().getTexture(Textures::Menu));

	sf::Text records;
	sf::Text levelEditor;

	m_Title.setFont(m_GameEngine->getAssets().getFont(Fonts::Main));
	m_Subtext.setFont(m_GameEngine->getAssets().getFont(Fonts::Main));
	records.setFont(m_GameEngine->getAssets().getFont(Fonts::Main));
	levelEditor.setFont(m_GameEngine->getAssets().getFont(Fonts::Main));

	m_Title.setCharacterSize(80);
	m_Title.setString("PLATFORMER");
	m_Title.setPosition(25, 0);

	m_Subtext.setCharacterSize(50);
	m_Subtext.setString("W : UP \tS : DOWN \tEnter : Select \tEsc : QUIT");


	m_Subtext.setPosition(sf::Vector2f(
		25,
		m_GameEngine->getWindow().getSize().y * 9 / 10
	));

	registerAction(sf::Keyboard::Escape, Actions::Quit);

	getRecords();
}

void RecordsScene::getRecords() {
	std::string path = "records/records.txt";
	std::string lvl;
	std::stringstream contents;
	std::ifstream records;
	try {
		records.open(path);
		contents << records.rdbuf();
		records.close();
	}
	catch (std::fstream::failure e) {
		std::cout << e.what() << std::endl;
		return;
	}

	while (std::getline(contents, lvl)) {
		std::string kv;
		std::stringstream ss(lvl);
		std::vector<std::string> temp;
		while (std::getline(ss, kv, ' ')) {
			temp.push_back(kv);
		}
		m_Levels[temp[0]].push_back(std::stof(temp[1]));
		temp.clear();
	}

	int i = 0;
	for (auto& kvPair : m_Levels) {
		std::sort(kvPair.second.begin(), kvPair.second.end());
		sf::Text customLvl;
		customLvl.setCharacterSize(50);
		customLvl.setFont(m_GameEngine->getAssets().getFont(Fonts::Main));
		customLvl.setString(kvPair.first + " " + std::to_string(kvPair.second[0]));
		customLvl.setPosition(25, 100 + i * 50);
		m_Records.push_back(customLvl);
		i++;
	}
}

void RecordsScene::update(float dt) {
	sRender();
}

void RecordsScene::sDoAction(const Action& action) {
	if (action.getType() == Actions::Start) {
		if (action.getName() == Actions::Quit) {
			m_GameEngine->changeScene(Scenes::Main, nullptr);
		}
	}
}

void RecordsScene::sRender() {
	sf::RenderWindow& window = m_GameEngine->getWindow();
	window.setView(m_RecordsView);
	window.clear();
	window.draw(m_Background);
	window.draw(m_Title);
	for (auto& elem : m_Records) {
		window.draw(elem);
	}
	window.draw(m_Subtext);
	window.display();
}