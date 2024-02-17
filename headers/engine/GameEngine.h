#pragma once
#include "imgui.h"
#include "imgui-SFML.h"
#include <SFML/Graphics.hpp>
#include "Scene.h"
#include "ObjectTypes.h"
#include "EntityManager.h"
#include "Assets.h"
#include <map>


class GameEngine {
private:
	sf::RenderWindow m_Window;
	std::map<Scenes::Type, Scene::ScenePtr> m_SceneMap;
	Assets m_Assets;
	Scenes::Type m_CurrentScene;
	bool m_Running;

public:
	GameEngine();
	
	void init();
	void run();
	void update(float);
	Scene* currentScene();
	void quit();
	void changeScene(Scenes::Type scene, std::shared_ptr<Scene> s);
	Assets& getAssets();
	sf::RenderWindow& getWindow();

	void sUserInput();
};