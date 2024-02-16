#pragma once
#include "engine/Scene.h"
#include "engine/Entity.h"
#include "engine/EntityManager.h"
#include <memory>


class GameScene : public Scene {
private:
	std::string m_Level;
	std::shared_ptr<Entity> m_Player;
	std::size_t m_GameFrame;
	std::size_t m_JumpFrame;
	std::size_t m_JumpTime;
	EntityManager m_EntityManager;
	sf::View m_GameView;
	Vec2 m_GridSize;
	bool m_DrawBox = false;
	Vec2 m_mPos;
	sf::CircleShape m_mShape;

public:
	GameScene(GameEngine* gameEngine, const std::string& levelPath);

	virtual void update();

	Vec2 windowToWorld(const Vec2& window) const;

	//sytems
	void sDraggable();
	void sAnimation();
	void sMovement();
	void sCollision();
	virtual void sDoAction(const Action& action);
	virtual void sRender();
	//void sEnemySpawner();
	void spawnBullet();

private:
	void init(const std::string& levelPath);
	void loadAssets();
	void loadLevel(const std::string& levelPath);
	void addTile(std::vector<std::string>& v, const std::string& type);
	void spawnPlayer();
	

};