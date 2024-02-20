#include "game/MenuScene.h"
#include "game/GameScene.h"
#include "engine/GameEngine.h"
#include "game/EditorScene.h"
#include "game/CustomLevelScene.h"
#include "game/LevelSelectScene.h"
#include "game/RecordsScene.h"

MenuScene::MenuScene(GameEngine* gameEngine):
	Scene(gameEngine),
	m_Options(),
	m_OptionIndex(),
	m_Title(),
	m_Subtext(),
	m_MenuView(m_GameEngine->getWindow().getDefaultView()){
	
	init();
}

void MenuScene::init() {
	/*m_Background.setSize(sf::Vector2f(
		(float)m_GameEngine->getWindow().getSize().x,
		(float)m_GameEngine->getWindow().getSize().y));
	m_Background.setFillColor(sf::Color(50, 50, 155));*/
	m_GameEngine->getAssets().addTexture(Textures::Menu, "graphics/menuScreen.png");
	m_Background.setTexture(m_GameEngine->getAssets().getTexture(Textures::Menu));

	sf::Text play;
	//sf::Text level2;
	sf::Text records;
	sf::Text customLevels;
	sf::Text levelEditor;

	m_Title.setFont(m_GameEngine->getAssets().getFont(Fonts::Main));
	m_Subtext.setFont(m_GameEngine->getAssets().getFont(Fonts::Main));
	play.setFont(m_GameEngine->getAssets().getFont(Fonts::Main));
	//level2.setFont(m_GameEngine->getAssets().getFont(Fonts::Main));
	records.setFont(m_GameEngine->getAssets().getFont(Fonts::Main));
	customLevels.setFont(m_GameEngine->getAssets().getFont(Fonts::Main));
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

	play.setCharacterSize(50);
	//level2.setCharacterSize(50);
	records.setCharacterSize(50);
	customLevels.setCharacterSize(50);
	levelEditor.setCharacterSize(50);

	play.setString("PLAY");
	//level2.setString("LEVEL 2");
	customLevels.setString("CUSTOM LEVELS");
	levelEditor.setString("LEVEL EDITOR");
	records.setString("RECORDS");

	play.setPosition(25, 100);
	//level2.setPosition(25, 150);
	records.setPosition(25, 250);
	customLevels.setPosition(25, 150);
	levelEditor.setPosition(25, 200);

	m_Options.push_back(play);
	//m_Options.push_back(level2);
	m_Options.push_back(customLevels);
	m_Options.push_back(levelEditor);
	m_Options.push_back(records);

	registerAction(sf::Keyboard::W, Actions::Up);
	registerAction(sf::Keyboard::S, Actions::Down);
	registerAction(sf::Keyboard::Escape, Actions::Quit);
	registerAction(sf::Keyboard::Return, Actions::Select);
	
}

void MenuScene::update(float dt){
	for (auto& elem : m_Options) {
		elem.setFillColor(sf::Color::White);
	}
	m_Options[m_OptionIndex].setFillColor(sf::Color(255, 50, 50));
}

void MenuScene::sDoAction(const Action& action){
	if (action.getType() == Actions::Start) {
		if (action.getName() == Actions::Up) {
			m_OptionIndex = (m_OptionIndex - 1) % m_Options.size();
		}
		if (action.getName() == Actions::Down) {
			m_OptionIndex = (m_OptionIndex + 1) % m_Options.size();
		}
		if (action.getName() == Actions::Select) {
			if (m_OptionIndex >= 0 && m_OptionIndex == 0) {
				std::shared_ptr<LevelSelectScene> gameScene(
					new LevelSelectScene(m_GameEngine, "levels/rooms/levelSelect.txt")
				);

				m_GameEngine->changeScene(Scenes::LevelSelect,
					std::static_pointer_cast<Scene>(gameScene));
			}
			if (m_OptionIndex == 1) {
				std::shared_ptr<CustomLevelScene> customLvls(
					new CustomLevelScene(m_GameEngine));

				m_GameEngine->changeScene(Scenes::CustomLevels,
					std::static_pointer_cast<Scene>(customLvls));
			}
			if (m_OptionIndex == 2) {
				std::shared_ptr<EditorScene> lvlEditor(
					new EditorScene(m_GameEngine));

				m_GameEngine->changeScene(Scenes::Editor,
					std::static_pointer_cast<Scene>(lvlEditor));
			}
			if (m_OptionIndex == 3) {
				std::shared_ptr<RecordsScene> rec(
					new RecordsScene(m_GameEngine));

				m_GameEngine->changeScene(Scenes::Records,
					std::static_pointer_cast<Scene>(rec));
			}
		}
		if (action.getName() == Actions::Quit) {
			m_GameEngine->quit();
		}
	}
}

void MenuScene::sRender() {
	sf::RenderWindow& window = m_GameEngine->getWindow();
	window.setView(m_MenuView);
	window.clear();
	window.draw(m_Background);
	window.draw(m_Title);
	for (auto& elem : m_Options) {
		window.draw(elem);
	}
	window.draw(m_Subtext);
	window.display();
}