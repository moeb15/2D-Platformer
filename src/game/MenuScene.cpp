#include "game/MenuScene.h"
#include "game/GameScene.h"
#include "engine/GameEngine.h"
#include "game/EditorScene.h"

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
	m_Background.setSize(sf::Vector2f(
		(float)m_GameEngine->getWindow().getSize().x,
		(float)m_GameEngine->getWindow().getSize().y));
	m_Background.setFillColor(sf::Color(50, 50, 155));

	sf::Text level1;
	sf::Text level2;
	sf::Text level3;
	sf::Text customLevels;
	sf::Text levelEditor;

	m_Title.setFont(m_GameEngine->getAssets().getFont(Fonts::Main));
	m_Subtext.setFont(m_GameEngine->getAssets().getFont(Fonts::Main));
	level1.setFont(m_GameEngine->getAssets().getFont(Fonts::Main));
	level2.setFont(m_GameEngine->getAssets().getFont(Fonts::Main));
	level3.setFont(m_GameEngine->getAssets().getFont(Fonts::Main));
	customLevels.setFont(m_GameEngine->getAssets().getFont(Fonts::Main));
	levelEditor.setFont(m_GameEngine->getAssets().getFont(Fonts::Main));

	m_Title.setCharacterSize(80);
	m_Title.setString("MEGA-MARIO WORLD");
	m_Title.setPosition(25, 0);

	m_Subtext.setCharacterSize(50);
	m_Subtext.setString("W : UP \tS : DOWN \tEnter : Select \tEsc : QUIT");
	

	m_Subtext.setPosition(sf::Vector2f(
		25,
		m_GameEngine->getWindow().getSize().y * 9 / 10
	));

	level1.setCharacterSize(50);
	level2.setCharacterSize(50);
	level3.setCharacterSize(50);
	customLevels.setCharacterSize(50);
	levelEditor.setCharacterSize(50);

	level1.setString("LEVEL 1");
	level2.setString("LEVEL 2");
	level3.setString("LEVEL 3");
	customLevels.setString("CUSTOM LEVELS");
	levelEditor.setString("LEVEL EDITOR");

	level1.setPosition(25, 100);
	level2.setPosition(25, 150);
	level3.setPosition(25, 200);
	customLevels.setPosition(25, 250);
	levelEditor.setPosition(25, 300);

	m_Options.push_back(level1);
	m_Options.push_back(level2);
	m_Options.push_back(level3);
	m_Options.push_back(customLevels);
	m_Options.push_back(levelEditor);

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
			if (m_OptionIndex >= 0 && m_OptionIndex <= 2) {
				std::shared_ptr<GameScene> gameScene(
					new GameScene(m_GameEngine, m_LevelPaths[m_OptionIndex])
				);

				m_GameEngine->changeScene(Scenes::Game,
					std::static_pointer_cast<Scene>(gameScene));
			}
			if (m_OptionIndex == 4) {
				std::shared_ptr<EditorScene> lvlEditor(
					new EditorScene(m_GameEngine));

				m_GameEngine->changeScene(Scenes::Editor,
					std::static_pointer_cast<Scene>(lvlEditor));
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