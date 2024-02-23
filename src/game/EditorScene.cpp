#include "game/EditorScene.h"
#include "engine/GameEngine.h"
#include "engine/Physics.h"
#include <iostream>
#include <fstream>

EditorScene::EditorScene(GameEngine* engine) :
	Scene(engine),
	m_EditorView(m_GameEngine->getWindow().getDefaultView()) {

	m_MouseShape.setRadius(6.f);
	m_MouseShape.setFillColor(sf::Color(255, 0, 0));
	m_MouseShape.setOrigin(2, 2);

	init();
}

void EditorScene::init() {
	registerAction(sf::Keyboard::Escape, Actions::Quit);
	registerAction(sf::Keyboard::D, Actions::Right);
	registerAction(sf::Keyboard::A, Actions::Left);
	registerAction(sf::Keyboard::C, Actions::Copy);

	loadAssets();
	m_Background.setTexture(m_GameEngine->getAssets().getTexture(Textures::Background));
	m_Background.setTextureRect(sf::IntRect(0, 0, 128000, 720));
}

void EditorScene::loadAssets() {
	m_GameEngine->getAssets().addTexture(Textures::Ground, "graphics/dungeonGround.png");
	m_GameEngine->getAssets().addTexture(Textures::Climbable, "graphics/climbableWall.png");
	m_GameEngine->getAssets().addTexture(Textures::DoorExit, "graphics/dungeonDoorClosed.png");
	m_GameEngine->getAssets().addTexture(Textures::Enemy, "graphics/dungeonEnemy.png");
	m_GameEngine->getAssets().addTexture(Textures::Background, "graphics/dungeonBackground-version2.png");
	m_GameEngine->getAssets().getTexture(Textures::Background).setRepeated(true);

	m_GameEngine->getAssets().addAnimation(Animations::Ground);
	m_GameEngine->getAssets().addAnimation(Animations::Climbable);
	m_GameEngine->getAssets().addAnimation(Animations::Enemy);
	m_GameEngine->getAssets().addAnimation(Animations::DoorExit);
}

void EditorScene::saveScene() {
	auto windowSize = m_GameEngine->getWindow().getSize();
	std::ofstream newLvl("levels/custom/" + m_LevelName + ".txt");
	std::string line;
	for (auto& e : m_EntityManager.getEntities()) {
		Vec2& pos = e->getComponent<CTransform>().pos;
		switch (e->tag()) {
		case Entities::Floor:
			line = "Tile Ground " + std::to_string((int)pos.x / 64)
				+ " " + std::to_string((int)windowSize.y / 64 - ((int)pos.y / 64 + 1)) + "\n";
			newLvl << line;
			break;

		case Entities::Climbable:
			line = "Tile Climbable " + std::to_string((int)pos.x / 64)
				+ " " + std::to_string((int)windowSize.y / 64 - ((int)pos.y / 64 + 1)) + "\n";
			newLvl << line;
			break;

		case Entities::Enemy:
			line = "AI Enemy " + std::to_string((int)pos.x / 64)
				+ " " + std::to_string((int)windowSize.y / 128 - ((int)pos.y / 128 + 1)) + "\n";
			newLvl << line;
			break;

		case Entities::Door:
			line = "Door Exit " + std::to_string((int)pos.x / 64)
				+ " " + std::to_string((int)windowSize.y / 128 - ((int)pos.y / 128 + 1)) + "\n";
			newLvl << line;
			break;
		}
	}
}

void EditorScene::sceneEditor() {
	ImGui::Begin("Level Editor");
	ImGui::SetWindowPos(ImVec2(0, 0));
	ImGui::SetWindowSize(ImVec2(
		m_GameEngine->getWindow().getSize().x / 4,
		m_GameEngine->getWindow().getSize().y/2));
	ImGui::Text("Add Entity");
	if (ImGui::Button("Ground Tile")) {
		addGround();
	}
	if (ImGui::Button("Climbable Tile")) {
		addClimable();
	}
	if (ImGui::Button("Enemy Entity")) {
		addEnemy();
	}
	if (ImGui::Button("Level-Exit Door")) {
		addDoor();
	}

	ImGui::Text("Level Title");
	ImGui::InputText("Title", &m_LevelName);
	if (ImGui::Button("Save")) {
		saveScene();
	}
	ImGui::End();
}

void EditorScene::addGround(const Vec2& pos) {
	auto viewCenter = m_EditorView.getCenter();
	auto m_WindowSize = m_GameEngine->getWindow().getSize();
	Vec2 posn;
	if (pos.x != 0 || pos.y != 0) {
		posn.x = pos.x / 64;
		posn.y = pos.y / 64;
	}
	else {
		posn = Vec2(viewCenter.x / 64, viewCenter.y / 64);
	}
	sf::Texture& texture = m_GameEngine->getAssets().getTexture(Textures::Ground);
	Vec2 size(texture.getSize().x, texture.getSize().y);

	std::shared_ptr<Entity> e = m_EntityManager.addEntity(Entities::Floor);
	e->addComponent<CTransform>();
	e->getComponent<CTransform>().pos.x = posn.x * size.x;
	e->getComponent<CTransform>().pos.y = m_WindowSize.y - (posn.y + 1) * size.y;
	e->getComponent<CTransform>().has = true;
	e->addComponent<CDraggable>();

	e->addComponent<CAnimation>(m_GameEngine->getAssets().getAnimation(Animations::Ground));
	e->getComponent<CAnimation>().animation.getSprite().setPosition(sf::Vector2f(
		e->getComponent<CTransform>().pos.x,
		e->getComponent<CTransform>().pos.y
	));
	e->getComponent<CAnimation>().animation.setRepeat(true);
}

void EditorScene::addClimable(const Vec2& pos) {
	auto viewCenter = m_EditorView.getCenter();
	auto m_WindowSize = m_GameEngine->getWindow().getSize();
	Vec2 posn;
	if (pos.x != 0 || pos.y != 0) {
		posn.x = pos.x / 64;
		posn.y = pos.y / 64;
	}
	else {
		posn = Vec2(viewCenter.x / 64, viewCenter.y / 64);
	}
	sf::Texture& texture = m_GameEngine->getAssets().getTexture(Textures::Climbable);
	Vec2 size(texture.getSize().x, texture.getSize().y);

	std::shared_ptr<Entity> e = m_EntityManager.addEntity(Entities::Climbable);
	e->addComponent<CTransform>();
	e->getComponent<CTransform>().pos.x = posn.x * size.x;
	e->getComponent<CTransform>().pos.y = m_WindowSize.y - (posn.y + 1) * size.y;
	e->getComponent<CTransform>().has = true;
	e->addComponent<CDraggable>();

	e->addComponent<CAnimation>(m_GameEngine->getAssets().getAnimation(Animations::Climbable));
	e->getComponent<CAnimation>().animation.getSprite().setPosition(sf::Vector2f(
		e->getComponent<CTransform>().pos.x,
		e->getComponent<CTransform>().pos.y
	));
	e->getComponent<CAnimation>().animation.setRepeat(true);

}

void EditorScene::addEnemy(const Vec2& pos) {
	auto viewCenter = m_EditorView.getCenter();
	auto m_WindowSize = m_GameEngine->getWindow().getSize();
	Vec2 posn;
	if (pos.x != 0 || pos.y != 0) {
		posn.x = pos.x / 64;
		posn.y = pos.y / 64;
	}
	else {
		posn = Vec2(viewCenter.x / 64, viewCenter.y / 64);
	}
	sf::Texture& texture = m_GameEngine->getAssets().getTexture(Textures::Enemy);
	Vec2 size(texture.getSize().x, texture.getSize().y);

	std::shared_ptr<Entity> e = m_EntityManager.addEntity(Entities::Enemy);
	e->addComponent<CTransform>();
	e->getComponent<CTransform>().pos.x = posn.x * 64;
	e->getComponent<CTransform>().pos.y = m_WindowSize.y - (posn.y + 1) * 64;
	e->getComponent<CTransform>().has = true;
	e->addComponent<CDraggable>();

	e->addComponent<CAnimation>(m_GameEngine->getAssets().getAnimation(Animations::Enemy));
	e->getComponent<CAnimation>().animation.getSprite().setPosition(sf::Vector2f(
		e->getComponent<CTransform>().pos.x,
		e->getComponent<CTransform>().pos.y
	));
	e->getComponent<CAnimation>().animation.setRepeat(true);
}

void EditorScene::addDoor(const Vec2& pos) {
	auto viewCenter = m_EditorView.getCenter();
	auto m_WindowSize = m_GameEngine->getWindow().getSize();
	Vec2 posn;
	if (pos.x != 0 || pos.y != 0) {
		posn.x = pos.x / 64;
		posn.y = pos.y / 64;
	}
	else {
		posn = Vec2(viewCenter.x / 64, viewCenter.y / 64);
	}
	sf::Texture& texture = m_GameEngine->getAssets().getTexture(Textures::DoorExit);
	Vec2 size(texture.getSize().x, texture.getSize().y);

	std::shared_ptr<Entity> e = m_EntityManager.addEntity(Entities::Door);
	e->addComponent<CTransform>();
	e->getComponent<CTransform>().pos.x = posn.x * 64;
	e->getComponent<CTransform>().pos.y = m_WindowSize.y - (posn.y + 1) * 64;
	e->getComponent<CTransform>().has = true;
	e->addComponent<CDraggable>();

	e->addComponent<CAnimation>(m_GameEngine->getAssets().getAnimation(Animations::DoorExit));
	e->getComponent<CAnimation>().animation.getSprite().setPosition(sf::Vector2f(
		e->getComponent<CTransform>().pos.x,
		e->getComponent<CTransform>().pos.y
	));
	e->getComponent<CAnimation>().animation.setRepeat(true);
}


void EditorScene::update(float dt){
	ImGui::SFML::Update(m_GameEngine->getWindow(), sf::seconds(dt));
	m_EntityManager.update();

	sAnimation();
	sDraggable();
}


void EditorScene::sAnimation() {
	for (auto& e : m_EntityManager.getEntities()) {
		if (e->hasComponent<CAnimation>()) {
			e->getComponent<CAnimation>().animation.update();
		}
	}
}

void EditorScene::sDoAction(const Action& action){
	if (action.getType() == Actions::Start) {
		if (action.getName() == Actions::Quit) {
			m_GameEngine->changeScene(Scenes::Main, nullptr);
		}
		if (action.getName() == Actions::Left) {
			auto windowSize = m_GameEngine->getWindow().getSize();
			auto viewCenter = m_EditorView.getCenter();
			if (viewCenter.x > windowSize.x / 2.f) {
				viewCenter.x -= 64;
			}
			else {
				viewCenter.x = windowSize.x / 2.f;
			}
			m_EditorView.setCenter(viewCenter);
		}
		if (action.getName() == Actions::Right) {
			auto viewCenter = m_EditorView.getCenter();
			viewCenter.x += 64;
			m_EditorView.setCenter(viewCenter);
		}
		if (action.getName() == Actions::MouseLeft) {
			for (auto& e : m_EntityManager.getEntities()) {
				auto mPos = windowToWorld(action.getPos());
				if (e->hasComponent<CDraggable>() && Physics::IsInside(mPos, e)) {
					if (e->hasComponent<CDraggable>()) {
						e->getComponent<CDraggable>().dragging =
							!e->getComponent<CDraggable>().dragging;
					}
				}
			}
		}
		if (action.getName() == Actions::MouseRight) {
			for (auto& e : m_EntityManager.getEntities()) {
				auto mPos = windowToWorld(action.getPos());
				if (Physics::IsInside(mPos, e)) {
					e->destroy();
				}
			}
		}
		if (action.getName() == Actions::Copy) {
			for (auto& e : m_EntityManager.getEntities()) {
				auto mPos = windowToWorld(action.getPos());
				if (e->getComponent<CDraggable>().dragging) {
					std::cout << "Copied" << std::endl;
					Vec2 pos = Vec2(
						e->getComponent<CTransform>().pos.x + 128,
						m_GameEngine->getWindow().getSize().y - e->getComponent<CTransform>().pos.y
					);
					switch (e->tag()) {
					case Entities::Floor: addGround(pos); break;
					case Entities::Enemy: addEnemy(pos); break;
					case Entities::Climbable: addClimable(pos); break;
					}
				}
			}
		}
	}

	if (action.getName() == Actions::MouseMoved) {
		auto mPos = action.getPos();
		m_MousePos = windowToWorld(mPos);
	}
}

void EditorScene::sDraggable(){
	for (auto& e : m_EntityManager.getEntities()) {
		if (e->hasComponent<CDraggable>()) {
			if (e->getComponent<CDraggable>().dragging) {
				e->getComponent<CTransform>().pos = m_MousePos - Vec2(32, 32);
			}
		}
	}
}

void EditorScene::sRender(){
	sf::RenderWindow& window = m_GameEngine->getWindow();
	window.setView(m_EditorView);
	ImGui::SFML::SetCurrentWindow(window);
	sceneEditor();
	window.clear(sf::Color(135, 206, 255));
	window.draw(m_Background);

	for (auto& e : m_EntityManager.getEntities()) {
		if (e->hasComponent<CTransform>() &&
			e->hasComponent<CAnimation>()) {
			auto& transform = e->getComponent<CTransform>();
			auto& animation = e->getComponent<CAnimation>().animation;
			animation.getSprite().setPosition(transform.pos.x,
				transform.pos.y);
			animation.getSprite().setRotation(transform.angle);
			window.draw(animation.getSprite());
		}
	}

	m_MouseShape.setPosition(m_MousePos.x, m_MousePos.y);
	window.draw(m_MouseShape);

	ImGui::SFML::Render(window);
	window.display();
}

Vec2 EditorScene::windowToWorld(const Vec2& window) const {

	sf::View view = m_GameEngine->getWindow().getView();
	float wx = view.getCenter().x - m_GameEngine->getWindow().getSize().x / 2.f;
	float wy = view.getCenter().y - m_GameEngine->getWindow().getSize().y / 2.f;

	return Vec2(window.x + wx, window.y + wy);
}
