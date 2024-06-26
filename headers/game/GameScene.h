#pragma once
#include "engine/Scene.h"
#include "engine/Entity.h"
#include "engine/EntityManager.h"
#include <memory>


class GameScene : public Scene {
private:
	std::string m_Level;
	std::shared_ptr<Entity> m_Player;
	EntityManager m_EntityManager;
	sf::View m_GameView;
	Vec2 m_GridSize;
	bool m_DrawBox = false;
	Vec2 m_mPos;
	sf::CircleShape m_mShape;
	sf::Sprite m_Background;
	std::string m_LevelTitle;
	sf::Time m_GameTime;
	sf::Text m_TimeText;
	sf::Sound m_KiBlast;
	sf::Sound m_Jumping;

public:
	GameScene(GameEngine* gameEngine, const std::string& levelPath);

	virtual void update(float dt);

	Vec2 windowToWorld(const Vec2& window) const;

	//sytems
	void sDraggable();
	void sAnimation();
	void sMovement(float dt);
	void sCollision();
	void sAI();
	void sLifespan(float dt);
	virtual void sDoAction(const Action& action);
	virtual void sRender();
	void spawnBullet();

private:
	void init(const std::string& levelPath);
	void loadAssets();
	void loadLevel(const std::string& levelPath);
	void addTile(std::vector<std::string>& v, const std::string& type);
	void addEnemy(std::vector<std::string>& v);
	void addDoor(std::vector<std::string>& v);
	void spawnPlayer();
	void exitLevel();

};