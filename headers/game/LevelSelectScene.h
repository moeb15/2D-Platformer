#pragma once
#include "engine/Scene.h"
#include <vector>
#include <string>
#include <SFML/Graphics.hpp>


class LevelSelectScene : public Scene {
private:
	EntityManager m_EntityManager;
	sf::Sprite m_Background;
	std::shared_ptr<Entity> m_Player;
	sf::View m_LevelSelectView;

public:
	LevelSelectScene(GameEngine* gameEngine, const std::string& levelPath);

	virtual void update(float dt);
	virtual void sDoAction(const Action&);
	virtual void sRender();
	void sMovement(float dt);
	void sAnimation();
	void sCollision();

private:
	void spawnPlayer();
	void addGravel(std::vector<std::string>& v);
	void addDoor(std::vector<std::string>& v);
	void enterLevel();
	void loadAssets();
	void loadLevel(const std::string& levelPath);
	void init(const std::string& levelPath);
};