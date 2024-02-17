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

public:
	EditorScene(GameEngine*);

	virtual void update(float dt);
	virtual void sRender();
	virtual void sDoAction(const Action& action);
	void sDraggable();
	void saveScene();
	void sceneEditor();
};