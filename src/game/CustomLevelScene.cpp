#include "game/CustomLevelScene.h"
#include "game/GameScene.h"
#include "engine/GameEngine.h"
#include "game/EditorScene.h"

CustomLevelScene::CustomLevelScene(GameEngine* gameEngine) :
	Scene(gameEngine),
	m_Options(),
	m_OptionIndex(),
	m_Title(),
	m_Subtext(),
	m_LevelSelectView(m_GameEngine->getWindow().getDefaultView()) {

	init();
}

void CustomLevelScene::init() {
	m_Background.setSize(sf::Vector2f(
		(float)m_GameEngine->getWindow().getSize().x,
		(float)m_GameEngine->getWindow().getSize().y));
	m_Background.setFillColor(sf::Color(50, 50, 155));

	sf::Text customLevels;
	sf::Text levelEditor;

	m_Title.setFont(m_GameEngine->getAssets().getFont(Fonts::Main));
	m_Subtext.setFont(m_GameEngine->getAssets().getFont(Fonts::Main));
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

	registerAction(sf::Keyboard::W, Actions::Up);
	registerAction(sf::Keyboard::S, Actions::Down);
	registerAction(sf::Keyboard::Escape, Actions::Quit);
	registerAction(sf::Keyboard::Return, Actions::Select);

	getLevels();
}

void CustomLevelScene::getLevels() {
	std::string path = "levels/custom/";
	std::string lvl;
	for (const auto& file : std::filesystem::directory_iterator(path)) {
		lvl = file.path().string().substr(path.length());
		lvl.pop_back();
		lvl.pop_back();
		lvl.pop_back();
		lvl.pop_back();
		m_Levels.push_back(lvl);
	}
	
	for (int i = 0; i < m_Levels.size(); i++) {
		sf::Text customLvl;
		customLvl.setCharacterSize(50);
		customLvl.setFont(m_GameEngine->getAssets().getFont(Fonts::Main));
		customLvl.setString(m_Levels[i]);
		customLvl.setPosition(25, 100 + i * 50);
		m_Options.push_back(customLvl);
	}
}

void CustomLevelScene::update(float dt) {
	for (auto& elem : m_Options) {
		elem.setFillColor(sf::Color::White);
	}
	m_Options[m_OptionIndex].setFillColor(sf::Color(255, 50, 50));
}

void CustomLevelScene::sDoAction(const Action& action) {
	if (action.getType() == Actions::Start) {
		if (action.getName() == Actions::Up) {
			m_OptionIndex = (m_OptionIndex - 1) % m_Options.size();
		}
		if (action.getName() == Actions::Down) {
			m_OptionIndex = (m_OptionIndex + 1) % m_Options.size();
		}
		if (action.getName() == Actions::Select) {
			std::shared_ptr<GameScene> gameScene(
				new GameScene(m_GameEngine,
					"levels/custom/" + m_Levels[m_OptionIndex] + ".txt")
			);

			m_GameEngine->changeScene(Scenes::Game,
				std::static_pointer_cast<Scene>(gameScene));
		}
		if (action.getName() == Actions::Quit) {
			m_GameEngine->changeScene(Scenes::Main, nullptr);
		}
	}
}

void CustomLevelScene::sRender() {
	sf::RenderWindow& window = m_GameEngine->getWindow();
	window.setView(m_LevelSelectView);
	window.clear();
	window.draw(m_Background);
	window.draw(m_Title);
	for (auto& elem : m_Options) {
		window.draw(elem);
	}
	window.draw(m_Subtext);
	window.display();
}