#pragma once
#include "EntityManager.h"
#include "Action.h"
#include <SFML/Graphics.hpp>
#include <map>
#include <memory>

class GameEngine;

class Scene {
public:
	typedef std::shared_ptr<Scene> ScenePtr;

protected:
	GameEngine* m_GameEngine;
	int m_CurrentFrame;
	std::map<sf::Keyboard::Key, Actions::Name> m_ActionMap;
	bool m_Paused;
	bool m_HasEnded;

public:
	Scene(GameEngine* gameEngine) :
		m_GameEngine(gameEngine),
		m_ActionMap(),
		m_Paused(),
		m_HasEnded(),
		m_CurrentFrame() {
	}

	virtual void update(float dt) = 0;
	virtual void sDoAction(const Action&) = 0;
	virtual void sRender() = 0;

	//void simulate();
	std::map<sf::Keyboard::Key, Actions::Name>& getActionMap();
	void doAction(Action);
	void registerAction(sf::Keyboard::Key, Actions::Name); 

};