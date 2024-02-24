#include "engine/GameEngine.h"
#include "game/MenuScene.h"
#include <memory>


GameEngine::GameEngine():
	m_Running(true),
	m_CurrentScene(Scenes::Main){

	init();
}

void GameEngine::init() {
	m_Window.create(sf::VideoMode(1280, 720), "MegaWorld", sf::Style::Close);
	m_Window.setFramerateLimit(60);
	m_Assets.addFont(Fonts::Main, "fonts/ByteBounce.ttf");
	m_Assets.addFont(Fonts::Edit, "fonts/Roboto-Bold.ttf");
	m_Assets.addSoundBuffer(Sounds::Main, "sounds/background_music.wav");
	m_BackgroundMusic.setBuffer(m_Assets.getSoundBuffer(Sounds::Main));
	m_BackgroundMusic.setVolume(10);
	m_BackgroundMusic.setLoop(true);

	std::shared_ptr<MenuScene> menuScene(
		new MenuScene(this));
	m_SceneMap.insert(std::make_pair(Scenes::Main,
		std::static_pointer_cast<Scene>(menuScene)));
}

void GameEngine::run() {
	ImGui::SFML::Init(m_Window);
	sf::Time dt;
	sf::Clock clock;
	m_BackgroundMusic.play();
	while (m_Running) {
		dt = clock.restart();
		sUserInput();
		update(dt.asSeconds());
		currentScene()->sRender();
	}
	ImGui::SFML::Shutdown();
}

void GameEngine::update(float dt){
	currentScene()->update(dt);
}

void GameEngine::quit() {
	m_Window.close();
	m_Running = false;
}

void GameEngine::changeScene(Scenes::Type scene, std::shared_ptr<Scene> s) {
	m_CurrentScene = scene;
	auto found = m_SceneMap.find(m_CurrentScene);
	if (found == m_SceneMap.end()) {
		m_SceneMap[m_CurrentScene] = s;
	}
	else if(s != nullptr) {
		m_SceneMap[m_CurrentScene] = s;
	}
}

Scene* GameEngine::currentScene() {
	return m_SceneMap[m_CurrentScene].get();
}

sf::RenderWindow& GameEngine::getWindow() {
	return m_Window;
}

Assets& GameEngine::getAssets() {
	return m_Assets;
}

void GameEngine::sUserInput(){
	sf::Event event;
	while (m_Window.pollEvent(event)) {
		ImGui::SFML::ProcessEvent(event);
		if (event.type == sf::Event::Closed) {
			quit();
		}
		if (event.type == sf::Event::KeyPressed || event.type == sf::Event::KeyReleased) {
			if (currentScene()->getActionMap().find(event.key.code) == currentScene()->getActionMap().end()) {
				continue;
			}
			Actions::Name actionName = currentScene()->getActionMap().find(event.key.code)->second;
			Actions::Type actionType = event.type == sf::Event::KeyPressed ? Actions::Start : Actions::Stop;
			currentScene()->doAction(Action(actionName, actionType));
		}
		sf::Vector2i mousePos = sf::Mouse::getPosition(getWindow());
		Vec2 m_Pos(mousePos.x, mousePos.y);

		if (event.type == sf::Event::MouseButtonPressed) {
			switch (event.mouseButton.button) {
			case sf::Mouse::Left:
				currentScene()->doAction(Action(Actions::MouseLeft, Actions::Start, m_Pos));
				break;

			case sf::Mouse::Right:
				currentScene()->doAction(Action(Actions::MouseRight, Actions::Start, m_Pos));
				break;

			default:
				break;
			}
		}

		if (event.type == sf::Event::MouseButtonReleased) {
			switch (event.mouseButton.button) {
			case sf::Mouse::Left:
				currentScene()->doAction(Action(Actions::MouseLeft, Actions::Stop, m_Pos));
				break;

			case sf::Mouse::Right:
				currentScene()->doAction(Action(Actions::MouseRight, Actions::Stop, m_Pos));
				break;

			default:
				break;
			}
		}

		if (event.type == sf::Event::MouseMoved) {
			currentScene()->doAction(Action(Actions::MouseMoved, Actions::Start,
				Vec2(event.mouseMove.x, event.mouseMove.y)));
		}
	}
}