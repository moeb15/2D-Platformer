#include "engine/Scene.h"


std::map<sf::Keyboard::Key, Actions::Name>& Scene::getActionMap() {
	return m_ActionMap;
}

void Scene::registerAction(sf::Keyboard::Key key, Actions::Name action) {
	m_ActionMap[key] = action;
}

void Scene::doAction(Action action) {
	sDoAction(action);
}