#pragma once
#include "engine/Scene.h"
#include "engine/Entity.h"
#include "engine/EntityManager.h"
#include <filesystem>
#include <SFML/Graphics.hpp>

class EditorScene : public Scene {
private:
	EntityManager m_EntityManager;
	Vec2 m_MousePos;
	sf::CircleShape m_MouseShape;
	sf::View m_EditorView;
	std::string m_LevelName;
	sf::Sprite m_Background;

public:
	EditorScene(GameEngine*);

	virtual void update(float dt);
	virtual void sRender();
	virtual void sDoAction(const Action& action);
	void sDraggable();
	void sAnimation();
	void saveScene();
	void sceneEditor();

private:
	Vec2 windowToWorld(const Vec2& window) const;
	void loadAssets();
	void addGround();
	void addBrick();
	void addBox();
	void addEnemy();
	void init();
};