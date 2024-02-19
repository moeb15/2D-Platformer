#include "game/LevelSelectScene.h"
#include "game/GameScene.h"
#include "engine/GameEngine.h"
#include "engine/Physics.h"
#include <fstream>
#include <sstream>


LevelSelectScene::LevelSelectScene(GameEngine* engine, const std::string& path) :
	Scene(engine),
	m_LevelSelectView(m_GameEngine->getWindow().getDefaultView()){

	init(path);
}

void LevelSelectScene::init(const std::string& path) {
	m_EntityManager = EntityManager();

	registerAction(sf::Keyboard::A, Actions::Left);
	registerAction(sf::Keyboard::D, Actions::Right);
	registerAction(sf::Keyboard::Escape, Actions::Quit);
	registerAction(sf::Keyboard::Space, Actions::Interact);


	loadAssets();
	loadLevel(path);
	spawnPlayer();
}

void LevelSelectScene::loadAssets() {
	m_GameEngine->getAssets().addTexture(Textures::Menu, "graphics/menuScreen.png");
	m_GameEngine->getAssets().addTexture(Textures::Gravel, "graphics/gravelCobbleStone.png");
	m_GameEngine->getAssets().addTexture(Textures::DoorEnter, "graphics/dungeonDoor.png");
	
	m_GameEngine->getAssets().addTexture(Textures::Idle, "graphics/megamanGohan.png");
	m_GameEngine->getAssets().addTexture(Textures::IdleLeft, "graphics/megamanGohanLeft.png");
	m_GameEngine->getAssets().addTexture(Textures::Run, "graphics/megamanGohanRun.png");
	m_GameEngine->getAssets().addTexture(Textures::RunLeft, "graphics/megamanGohanRunLeft.png");

	m_GameEngine->getAssets().addAnimation(Animations::Idle);
	m_GameEngine->getAssets().addAnimation(Animations::IdleLeft);
	m_GameEngine->getAssets().addAnimation(Animations::Run);
	m_GameEngine->getAssets().addAnimation(Animations::RunLeft);
	m_GameEngine->getAssets().addAnimation(Animations::Gravel);
	m_GameEngine->getAssets().addAnimation(Animations::DoorEnter);
}

void LevelSelectScene::loadLevel(const std::string& levelPath) {
	m_Background.setTexture(m_GameEngine->getAssets().getTexture(Textures::Menu));

	std::fstream file;
	file.open(levelPath);
	std::stringstream contents;
	contents << file.rdbuf();
	file.close();

	std::string line;
	std::vector<std::string> fileEntities;
	std::string curText;

	while (std::getline(contents, line)) {
		std::stringstream ss(line);
		while (std::getline(ss, curText, ' ')) {
			fileEntities.push_back(curText);
			//std::cout << curText << std::endl;
		}
		if (fileEntities[0] == "Tile") {
			addGravel(fileEntities);
		}
		else if (fileEntities[0] == "Door") {
			addDoor(fileEntities);
		}

		fileEntities.clear();
	}
}

void LevelSelectScene::spawnPlayer() {
	sf::Vector2u m_WindowSize = m_GameEngine->getWindow().getSize();
	Vec2 pos(0, 0);
	sf::Texture& texture = m_GameEngine->getAssets().getTexture(Textures::Idle);
	Vec2 size(texture.getSize().x, texture.getSize().y);

	m_Player = m_EntityManager.addEntity(Entities::Player);
	m_Player->addComponent<CTransform>();
	m_Player->getComponent<CTransform>().pos.x = pos.x * size.x;
	m_Player->getComponent<CTransform>().pos.y = m_WindowSize.y - (pos.y + 1) * size.y;

	m_Player->addComponent<CBoundingBox>();
	m_Player->getComponent<CBoundingBox>().size = Vec2(64, 80);
	m_Player->getComponent<CBoundingBox>().has = true;

	m_Player->addComponent<CAnimation>(m_GameEngine->getAssets().getAnimation(Animations::Idle));
	m_Player->getComponent<CAnimation>().animation.getSprite().setPosition(sf::Vector2f(
		m_Player->getComponent<CTransform>().pos.x,
		m_Player->getComponent<CTransform>().pos.y
	));
	m_Player->getComponent<CAnimation>().animation.setRepeat(true);

	m_Player->addComponent<CInput>();
	m_Player->addComponent<CGravity>();
	m_Player->addComponent<CState>();
	m_Player->getComponent<CState>().state = States::Air;
}

void LevelSelectScene::addGravel(std::vector<std::string>& fileEntities) {
	sf::Vector2u m_WindowSize = m_GameEngine->getWindow().getSize();

	Vec2 posn(std::stoi(fileEntities[2]), std::stoi(fileEntities[3]));
	sf::Texture& texture = m_GameEngine->getAssets().getTexture(Textures::Gravel);
	Vec2 size(texture.getSize().x, texture.getSize().y);

	auto e = m_EntityManager.addEntity(Entities::Floor);
	e->addComponent<CTransform>();
	e->getComponent<CTransform>().pos.x = posn.x * size.x;
	e->getComponent<CTransform>().pos.y = m_WindowSize.y - (posn.y + 1) * size.y;
	e->getComponent<CTransform>().has = true;

	e->addComponent<CAnimation>(m_GameEngine->getAssets().getAnimation(Animations::Gravel));
	e->getComponent<CAnimation>().animation.getSprite().setPosition(sf::Vector2f(
		e->getComponent<CTransform>().pos.x,
		e->getComponent<CTransform>().pos.y
	));
	e->getComponent<CAnimation>().animation.setRepeat(true);


	e->addComponent<CBoundingBox>();
	e->getComponent<CBoundingBox>().size.x = size.x;
	e->getComponent<CBoundingBox>().size.y = size.y;
}

void LevelSelectScene::addDoor(std::vector<std::string>& fileEntities) {
	sf::Vector2u m_WindowSize = m_GameEngine->getWindow().getSize();
	Vec2 posn(std::stoi(fileEntities[2]), std::stoi(fileEntities[3]));
	sf::Texture& texture = m_GameEngine->getAssets().getTexture(Textures::DoorEnter);
	Vec2 size(texture.getSize().x, texture.getSize().y);

	auto e = m_EntityManager.addEntity(Entities::Door);
	e->addComponent<CTransform>();
	e->getComponent<CTransform>().pos.x = posn.x * 64;
	e->getComponent<CTransform>().pos.y = m_WindowSize.y - (posn.y + 3) * 64;

	e->addComponent<CLevel>();
	e->getComponent<CLevel>().path = fileEntities[1] + ".txt";

	e->addComponent<CAnimation>(m_GameEngine->getAssets().getAnimation(Animations::DoorEnter));
	e->getComponent<CAnimation>().animation.getSprite().setPosition(sf::Vector2f(
		e->getComponent<CTransform>().pos.x,
		e->getComponent<CTransform>().pos.y
	));
	e->getComponent<CAnimation>().animation.setRepeat(true);

	e->addComponent<CBoundingBox>();
	e->getComponent<CBoundingBox>().size.x = size.x;
	e->getComponent<CBoundingBox>().size.y = size.y;
}


void LevelSelectScene::update(float dt) {
	m_EntityManager.update();

	sMovement(dt);
	sAnimation();
	sCollision();
	sRender();
}

void LevelSelectScene::enterLevel() {
	for (auto& door : m_EntityManager.getEntities(Entities::Door)) {
		Vec2 playerPos = m_Player->getComponent<CTransform>().pos;
		if (Physics::IsInside(playerPos, door)) {
			std::shared_ptr<GameScene> gameLevel(
				new GameScene(m_GameEngine, 
					"levels/" + door->getComponent<CLevel>().path));
			m_GameEngine->changeScene(Scenes::Game,
				std::static_pointer_cast<Scene>(gameLevel));
			break;
		}
	}
}

void LevelSelectScene::sDoAction(const Action& action) {
	if (action.getType() == Actions::Start) {
		if (action.getName() == Actions::Quit) {
			m_GameEngine->changeScene(Scenes::Main, nullptr);
		}
		if (action.getName() == Actions::Right) {
			m_Player->getComponent<CInput>().right = true;
		}
		if (action.getName() == Actions::Left) {
			m_Player->getComponent<CInput>().left = true;
		}
		if (action.getName() == Actions::Interact) {
			enterLevel();
		}
	}
	else if (action.getType() == Actions::Stop) {
		if (action.getName() == Actions::Right) {
			m_Player->getComponent<CInput>().right = false;
		}
		if (action.getName() == Actions::Left) {
			m_Player->getComponent<CInput>().left = false;
		}
	}
}

void LevelSelectScene::sAnimation() {

	if (m_Player->getComponent<CState>().state == States::Ground) {
		if (m_Player->getComponent<CInput>().right) {
			if (m_Player->getComponent<CAnimation>().animation.getType() != Animations::Run) {
				m_Player->addComponent<CAnimation>(m_GameEngine->getAssets().getAnimation(Animations::Run));
				m_Player->getComponent<CAnimation>().animation.setRepeat(true);
			}
		}
		else if (m_Player->getComponent<CInput>().left) {
			if (m_Player->getComponent<CAnimation>().animation.getType() != Animations::RunLeft) {
				m_Player->addComponent<CAnimation>(m_GameEngine->getAssets().getAnimation(Animations::RunLeft));
				m_Player->getComponent<CAnimation>().animation.setRepeat(true);
			}
		}
		else {
			if (m_Player->getComponent<CAnimation>().animation.getType() == Animations::Run ) {
				m_Player->addComponent<CAnimation>(m_GameEngine->getAssets().getAnimation(Animations::Idle));
			}
			else if (m_Player->getComponent<CAnimation>().animation.getType() == Animations::RunLeft) {
				m_Player->addComponent<CAnimation>(m_GameEngine->getAssets().getAnimation(Animations::IdleLeft));
			}
		}
	}

	for (auto& e : m_EntityManager.getEntities()) {
		if (e->hasComponent<CAnimation>()) {
			e->getComponent<CAnimation>().animation.update();
		}
	}
}

void LevelSelectScene::sMovement(float dt) {
	Vec2 playerVel;
	if (m_Player->getComponent<CInput>().right) {
		if (m_Player->getComponent<CTransform>().velocity.x < 400) {
			playerVel.x = m_Player->getComponent<CTransform>().velocity.x + 100;
		}
		else {
			playerVel.x = 400;
		}
	}

	if (m_Player->getComponent<CInput>().left) {
		if (m_Player->getComponent<CTransform>().velocity.x > -400) {
			playerVel.x = m_Player->getComponent<CTransform>().velocity.x - 100;
		}
		else {
			playerVel.x = -400;
		}
	}

	m_Player->getComponent<CTransform>().velocity = playerVel;

	if (m_Player->getComponent<CTransform>().pos.x < 0) {
		m_Player->getComponent<CTransform>().pos.x = 0;
	}
	if (m_Player->getComponent<CTransform>().pos.x >
		m_GameEngine->getWindow().getSize().x - m_Player->getComponent<CBoundingBox>().size.x) {
		m_Player->getComponent<CTransform>().pos.x = m_GameEngine->getWindow().getSize().x 
			- m_Player->getComponent<CBoundingBox>().size.x;
	}

	if (m_Player->getComponent<CTransform>().pos.y > m_GameEngine->getWindow().getSize().y) {
		m_Player->getComponent<CTransform>().pos = m_Player->getComponent<CBoundingBox>().size;
	}

	for (auto& e : m_EntityManager.getEntities()) {
		e->getComponent<CTransform>().prevPos = e->getComponent<CTransform>().pos;
		if (e->hasComponent<CGravity>()) {
			e->getComponent<CTransform>().velocity.y += e->getComponent<CGravity>().gravity * dt * 0.5f;
		}
		e->getComponent<CTransform>().pos = e->getComponent<CTransform>().pos +
			e->getComponent<CTransform>().velocity * dt;
	}
}

void LevelSelectScene::sCollision() {
	for (auto& e : m_EntityManager.getEntities()) {
		if (e->tag() == Entities::Player) {
			continue;
		}
		if (e->tag() == Entities::Floor) {
			Vec2 overlap = Physics::GetOverlap(m_Player, e);
			Vec2 prevOverlap = Physics::GetPreviousOverlap(m_Player, e);
			if (overlap.y > 0.0f && overlap.x > 0.0f) {
				if (prevOverlap.x > 0 && m_Player->getComponent<CTransform>().prevPos.y <
					e->getComponent<CTransform>().prevPos.y) {
					m_Player->getComponent<CTransform>().pos.y -= overlap.y;
					m_Player->getComponent<CTransform>().velocity.y = 0;
					m_Player->getComponent<CState>().state = States::Ground;
				}
				if (prevOverlap.x > 0 && m_Player->getComponent<CTransform>().prevPos.y >
					e->getComponent<CTransform>().prevPos.y) {
					m_Player->getComponent<CTransform>().pos.y += overlap.y;
					m_Player->getComponent<CTransform>().velocity.y = 0;
					m_Player->getComponent<CState>().state = States::Air;
				}
				if (prevOverlap.y > 0 && m_Player->getComponent<CTransform>().prevPos.x >
					e->getComponent<CTransform>().prevPos.x) {
					m_Player->getComponent<CTransform>().pos.x += overlap.x;
				}
				if (prevOverlap.y > 0 && m_Player->getComponent<CTransform>().prevPos.x <
					e->getComponent<CTransform>().prevPos.x) {
					m_Player->getComponent<CTransform>().pos.x -= overlap.x;
				}
			}
		}
	}
}

void LevelSelectScene::sRender() {
	sf::RenderWindow& window = m_GameEngine->getWindow();

	window.setView(m_LevelSelectView);
	window.clear();
	window.draw(m_Background);
	for (auto& e : m_EntityManager.getEntities()) {
		if (e->hasComponent<CAnimation>()) {
			auto& transform = e->getComponent<CTransform>();
			auto& animation = e->getComponent<CAnimation>().animation;
			animation.getSprite().setPosition(transform.pos.x, transform.pos.y);
			window.draw(animation.getSprite());
		}
	}

	window.display();
}