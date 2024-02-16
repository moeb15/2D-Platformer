#include "GameScene.h"
#include "GameEngine.h"
#include "MenuScene.h"
#include "Physics.h"
#include <iostream>
#include <fstream>
#include <sstream>


bool IsInside(Vec2 pos, std::shared_ptr<Entity> e) {
	auto& transform = e->getComponent<CTransform>();
	auto& size = e->getComponent<CAnimation>().animation.getSize();
	//std::cout << size.x << " " << size.y << std::endl;

	float dx = fabsf(transform.pos.x - pos.x + size.x / 2);
	float dy = fabsf(transform.pos.y - pos.y + size.y / 2);
	
	return (dx <= size.x / 2) && (dy <= size.y / 2);
}

GameScene::GameScene(GameEngine* gameEngine, const std::string& levelPath) :
	Scene(gameEngine),
	m_Level(levelPath),
	m_GameFrame(0),
	m_JumpFrame(0),
	m_JumpTime(60),
	m_GameView(m_GameEngine->getWindow().getDefaultView()),
	m_GridSize(64, 64){

	m_mShape.setRadius(6.f);
	m_mShape.setFillColor(sf::Color(255, 0, 0));
	m_mShape.setOrigin(2, 2);

	init(levelPath);
}


void GameScene::init(const std::string& levelPath) {
	m_EntityManager = EntityManager();

	registerAction(sf::Keyboard::W, Actions::Jump);
	registerAction(sf::Keyboard::A, Actions::Left);
	registerAction(sf::Keyboard::D, Actions::Right);
	registerAction(sf::Keyboard::K, Actions::Shoot);
	registerAction(sf::Keyboard::Escape, Actions::Quit);
	registerAction(sf::Keyboard::B, Actions::ToggleBox);

	loadAssets();
	spawnPlayer();
	loadLevel(levelPath);
}

void GameScene::loadAssets() {
	m_GameEngine->getAssets().addTexture(Textures::Brick, "graphics/brickTexture.png");
	m_GameEngine->getAssets().addTexture(Textures::Ground, "graphics/groundTexture.png");
	m_GameEngine->getAssets().addTexture(Textures::QuestionBox, "graphics/questionBoxAnimation.png");
	m_GameEngine->getAssets().addTexture(Textures::Explosion, "graphics/shittyExplosion.png");

	m_GameEngine->getAssets().addTexture(Textures::Idle, "graphics/megamanIdle.png");
	m_GameEngine->getAssets().addTexture(Textures::Run, "graphics/megamanRun.png");
	m_GameEngine->getAssets().addTexture(Textures::Jump, "graphics/megamanJump.png");

	m_GameEngine->getAssets().addAnimation(Animations::Idle);
	m_GameEngine->getAssets().addAnimation(Animations::Run);
	m_GameEngine->getAssets().addAnimation(Animations::Jump);
	m_GameEngine->getAssets().addAnimation(Animations::Tile);
	m_GameEngine->getAssets().addAnimation(Animations::Ground);
	m_GameEngine->getAssets().addAnimation(Animations::QuestionBox);
	m_GameEngine->getAssets().addAnimation(Animations::Explosion);
}


void GameScene::loadLevel(const std::string& levelPath){
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
			addTile(fileEntities, fileEntities[1]);
		}

		fileEntities.clear();
 	}
}

void GameScene::spawnPlayer() {
	sf::Vector2u m_WindowSize = m_GameEngine->getWindow().getSize();
	Vec2 pos(1, 1);
	sf::Texture& texture = m_GameEngine->getAssets().getTexture(Textures::Idle);
	Vec2 size(texture.getSize().x, texture.getSize().y);

	m_Player = m_EntityManager.addEntity(Entities::Player);
	m_Player->addComponent<CTransform>();
	m_Player->getComponent<CTransform>().pos.x = pos.x * size.x;
	m_Player->getComponent<CTransform>().pos.y = m_WindowSize.y - (pos.y + 1) * size.y;

	m_Player->addComponent<CBoundingBox>();
	m_Player->getComponent<CBoundingBox>().size = size;
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
}

void GameScene::addTile(std::vector<std::string>& fileEntities, const std::string& type) {
	sf::Vector2u m_WindowSize = m_GameEngine->getWindow().getSize();

	if (type == "Ground") {
		Vec2 posn(std::stoi(fileEntities[2]), std::stoi(fileEntities[3]));
		sf::Texture& texture = m_GameEngine->getAssets().getTexture(Textures::Ground);
		Vec2 size(texture.getSize().x, texture.getSize().y);

		auto e = m_EntityManager.addEntity(Entities::Floor);
		e->addComponent<CTransform>();
		e->getComponent<CTransform>().pos.x = posn.x * size.x;
		e->getComponent<CTransform>().pos.y = m_WindowSize.y - (posn.y + 1) * size.y;
		e->getComponent<CTransform>().has = true;

		e->addComponent<CAnimation>(m_GameEngine->getAssets().getAnimation(Animations::Ground));
		e->getComponent<CAnimation>().animation.getSprite().setPosition(sf::Vector2f(
			e->getComponent<CTransform>().pos.x,
			e->getComponent<CTransform>().pos.y
		));
		e->getComponent<CAnimation>().animation.setRepeat(true);


		e->addComponent<CBoundingBox>();
		e->getComponent<CBoundingBox>().has = true;
		e->getComponent<CBoundingBox>().size.x = size.x;
		e->getComponent<CBoundingBox>().size.y = size.y;
	}
	else if (type == "Brick") {
		Vec2 posn(std::stoi(fileEntities[2]), std::stoi(fileEntities[3]));
		sf::Texture& texture = m_GameEngine->getAssets().getTexture(Textures::Brick);
		Vec2 size(texture.getSize().x, texture.getSize().y);

		auto e = m_EntityManager.addEntity(Entities::Tile);
		e->addComponent<CDraggable>();
		e->addComponent<CTransform>();
		e->getComponent<CTransform>().pos.x = posn.x * size.x;
		e->getComponent<CTransform>().pos.y = m_WindowSize.y - (posn.y + 1) * size.y;
		e->getComponent<CTransform>().has = true;

		e->addComponent<CAnimation>(m_GameEngine->getAssets().getAnimation(Animations::Tile));
		e->getComponent<CAnimation>().has = true;

		e->getComponent<CAnimation>().animation.getSprite().setPosition(sf::Vector2f(
			e->getComponent<CTransform>().pos.x,
			e->getComponent<CTransform>().pos.y
		));
		e->getComponent<CAnimation>().animation.setRepeat(true);


		e->addComponent<CBoundingBox>();
		e->getComponent<CBoundingBox>().has = true;
		e->getComponent<CBoundingBox>().size.x = size.x;
		e->getComponent<CBoundingBox>().size.y = size.y;

	}
	else if (type == "QuestionBox") {
		Vec2 posn(std::stoi(fileEntities[2]), std::stoi(fileEntities[3]));
		sf::Texture& texture = m_GameEngine->getAssets().getTexture(Textures::QuestionBox);
		Vec2 size(texture.getSize().x/4, texture.getSize().y);

		auto e = m_EntityManager.addEntity(Entities::QuestionBox);
		e->addComponent<CTransform>();
		e->getComponent<CTransform>().pos.x = posn.x * size.x;
		e->getComponent<CTransform>().pos.y = m_WindowSize.y - (posn.y + 1) * size.y;
		e->getComponent<CTransform>().has = true;

		e->addComponent<CAnimation>(m_GameEngine->getAssets().getAnimation(Animations::QuestionBox));
		e->getComponent<CAnimation>().animation.setRepeat(true);
		e->getComponent<CAnimation>().animation.getSprite().setPosition(sf::Vector2f(
			e->getComponent<CTransform>().pos.x,
			e->getComponent<CTransform>().pos.y
		));


		e->addComponent<CBoundingBox>();
		e->getComponent<CBoundingBox>().has = true;
		e->getComponent<CBoundingBox>().size.x = size.x;
		e->getComponent<CBoundingBox>().size.y = size.y;

	}
}

void GameScene::update(){
	m_GameFrame++;
	m_EntityManager.update();
	//std::cout << m_EntityManager.getEntities().size() << std::endl;
	
	sMovement();
	sDraggable();
	sCollision();
	sAnimation();
	sRender();
}

void GameScene::sAnimation() {
	
	if (m_Player->getComponent<CState>().state == States::Ground) {
		if (m_Player->getComponent<CInput>().right) {
			if (m_Player->getComponent<CAnimation>().animation.getType() != Animations::Run) {
				m_Player->addComponent<CAnimation>(m_GameEngine->getAssets().getAnimation(Animations::Run));
				m_Player->getComponent<CAnimation>().animation.setRepeat(true);
			}
		}
		else if (m_Player->getComponent<CInput>().left) {
			if (m_Player->getComponent<CAnimation>().animation.getType() != Animations::Run) {
				m_Player->addComponent<CAnimation>(m_GameEngine->getAssets().getAnimation(Animations::Run));
				m_Player->getComponent<CAnimation>().animation.setRepeat(true);
				m_Player->getComponent<CAnimation>().animation.getSprite().scale(-1, 1);
			}
		}
		else if (m_Player->getComponent<CInput>().up) {
			m_Player->addComponent<CAnimation>(m_GameEngine->getAssets().getAnimation(Animations::Jump));
			m_Player->getComponent<CAnimation>().animation.setRepeat(true);
		}
		else {
			m_Player->addComponent<CAnimation>(m_GameEngine->getAssets().getAnimation(Animations::Idle));
			m_Player->getComponent<CAnimation>().animation.setRepeat(true);
		}
	}
	if (m_Player->getComponent<CState>().state == States::Air) {
		m_Player->addComponent<CAnimation>(m_GameEngine->getAssets().getAnimation(Animations::Jump));
		m_JumpTime++;
	}

	for (auto& e : m_EntityManager.getEntities()) {
		if (e->hasComponent<CAnimation>()) {
			e->getComponent<CAnimation>().animation.update();
			if (e->getComponent<CAnimation>().animation.hasEnded()) {
				e->destroy();
			}
		}
	}
}

void GameScene::sDoAction(const Action& action){
	if (action.getType() == Actions::Start) {
		if (action.getName() == Actions::Quit) {
			std::shared_ptr<MenuScene> menuScene(
				new MenuScene(m_GameEngine));

			m_GameEngine->changeScene(Scenes::Main, menuScene);
		}
		if (action.getName() == Actions::Right) {
			m_Player->getComponent<CInput>().right = true;
		}
		if (action.getName() == Actions::Left) {
			m_Player->getComponent<CInput>().left = true;
		}
		if (action.getName() == Actions::Jump) {
			if (m_Player->getComponent<CState>().state != States::Air
					&& m_GameFrame - m_JumpFrame > 10) {
				m_Player->getComponent<CState>().state = States::Air;
				m_Player->getComponent<CInput>().up = true;
				m_JumpFrame = m_GameFrame;
				m_JumpTime = 60;
			}
		}
		if (action.getName() == Actions::ToggleBox) {
			m_DrawBox = !m_DrawBox;
		}
		if (action.getName() == Actions::MouseLeft) {
			Vec2 m_Pos = windowToWorld(action.getPos());
			for (auto& e : m_EntityManager.getEntities()) {
				if (e->hasComponent<CDraggable>() && IsInside(m_Pos, e)) {
					e->getComponent<CDraggable>().dragging = !e->getComponent<CDraggable>().dragging;
				}
			}
		}
		if (action.getName() == Actions::MouseMoved) {
			Vec2 mpos = action.getPos();
			m_mPos = windowToWorld(mpos);
		}
	}
	else if (action.getType() == Actions::Stop) {
		if (action.getName() == Actions::Right) {
			m_Player->getComponent<CInput>().right = false;
		}
		if (action.getName() == Actions::Left) {
			m_Player->getComponent<CInput>().left = false;
		}
		if (action.getName() == Actions::Jump) {
			m_Player->getComponent<CInput>().up = false;
		}
	}
}

void GameScene::sMovement(){
	Vec2 playerVel;
	if (m_Player->getComponent<CInput>().right) {
		if (m_Player->getComponent<CTransform>().velocity.x < 12) {
			playerVel.x = m_Player->getComponent<CTransform>().velocity.x + 3;
		}
		else {
			playerVel.x = 12;
		}
	}

	if (m_Player->getComponent<CInput>().left) {
		if (m_Player->getComponent<CTransform>().velocity.x > -12) {
			playerVel.x = m_Player->getComponent<CTransform>().velocity.x - 3;
		}
		else {
			playerVel.x = -12;
		}
	}

	if (m_Player->getComponent<CInput>().up &&
		m_Player->getComponent<CTransform>().velocity.y >= 0 &&
		m_Player->getComponent<CState>().state == States::Ground) {
		playerVel.y = -45;
		m_Player->getComponent<CState>().state = States::Air;
	}
	else {
		playerVel.y = m_Player->getComponent<CTransform>().velocity.y;
	}

	m_Player->getComponent<CTransform>().velocity = playerVel;

	if (m_Player->getComponent<CTransform>().pos.x < 
		m_Player->getComponent<CBoundingBox>().size.x) {
		m_Player->getComponent<CTransform>().pos.x = m_Player->getComponent<CBoundingBox>().size.x;
	}

	if (m_Player->getComponent<CTransform>().pos.y > m_GameEngine->getWindow().getSize().y) {
		m_Player->getComponent<CTransform>().pos = m_Player->getComponent<CBoundingBox>().size;
	}

	for (auto& e : m_EntityManager.getEntities()) {
		e->getComponent<CTransform>().prevPos = e->getComponent<CTransform>().pos;
		if (e->hasComponent<CGravity>()) {
			e->getComponent<CTransform>().velocity.y += e->getComponent<CGravity>().gravity *
				((float)m_JumpTime / 60.f) * ((float)m_JumpTime / 60.f) * 0.5f;
		}
		e->getComponent<CTransform>().pos = e->getComponent<CTransform>().pos +
			e->getComponent<CTransform>().velocity;
	}
}

void GameScene::sDraggable() {
	for (auto& e : m_EntityManager.getEntities()) {
		if (e->hasComponent<CDraggable>()) {
			if (e->getComponent<CDraggable>().dragging) {
				e->getComponent<CTransform>().pos = Vec2(m_mPos.x - 32, m_mPos.y - 32);
			}
		}
	}
}

void GameScene::sCollision(){
	for (auto& e : m_EntityManager.getEntities()) {
		if (e->tag() == Entities::Player) {
			continue;
		}
		if (e->tag() == Entities::Floor) {
			Vec2 overlap = Physics::GetOverlap(m_Player, e);
			Vec2 prevOverlap = Physics::GetPreviousOverlap(m_Player, e);
			if (overlap.y > 0.0f && overlap.x > 0) {
				if (prevOverlap.x > 0) {
					m_Player->getComponent<CTransform>().pos.y -= overlap.y;
					m_Player->getComponent<CTransform>().velocity.y = 0;
					m_Player->getComponent<CState>().state = States::Ground;
				}
				if (prevOverlap.y > 0) {
					m_Player->getComponent<CTransform>().pos.x -= overlap.x;
				}
			}
			else if(m_Player->getComponent<CTransform>().pos.y > 
				m_GameEngine->getWindow().getSize().y - m_Player->getComponent<CBoundingBox>().size.y) {
				m_JumpTime = 60;
				m_Player->getComponent<CState>().state = States::Air;
			}
		}
		if (e->tag() == Entities::Tile) {
			Vec2 overlap = Physics::GetOverlap(m_Player, e);
			Vec2 prevOverlap = Physics::GetPreviousOverlap(m_Player, e);
			if (overlap.x > 0.0f && overlap.y > 0.0f) {
				if (m_Player->getComponent<CTransform>().prevPos.y >
					e->getComponent<CTransform>().prevPos.y) {
					e->addComponent<CAnimation>(m_GameEngine->getAssets().getAnimation(Animations::Explosion));;
					e->addComponent<CBoundingBox>();
					continue;
				}
				if (prevOverlap.x > 0) {
					m_Player->getComponent<CTransform>().pos.y -= overlap.y;
					m_Player->getComponent<CTransform>().velocity.y = 0;
					m_JumpTime = 60;
					m_Player->getComponent<CState>().state = States::Ground;
				}
				else if (prevOverlap.y > 0) {
					m_Player->getComponent<CTransform>().pos.x -= overlap.x;
				}
			}
		}
		if (e->tag() == Entities::QuestionBox) {
			Vec2 overlap = Physics::GetOverlap(m_Player, e);
			Vec2 prevOverlap = Physics::GetPreviousOverlap(m_Player, e);
			if (overlap.x > 0.0f && overlap.y > 0.0f) {
				if (m_Player->getComponent<CTransform>().prevPos.y >
					e->getComponent<CTransform>().prevPos.y) {
					m_Player->getComponent<CTransform>().pos.y += overlap.y;
					m_Player->getComponent<CTransform>().velocity.y = 0;
					continue;
				}
				if (prevOverlap.x > 0) {
					m_Player->getComponent<CTransform>().pos.y -= overlap.y;
					m_Player->getComponent<CTransform>().velocity.y = 0;
					m_JumpTime = 60;
					m_Player->getComponent<CState>().state = States::Ground;
				}
				else if (prevOverlap.y > 0) {
					m_Player->getComponent<CTransform>().pos.x += overlap.x;
				}
			}
		}
	}
}

void GameScene::sRender(){
	sf::RenderWindow& window = m_GameEngine->getWindow();
	sf::Vector2u windowSize = window.getSize();
	
	auto& playerPos = m_Player->getComponent<CTransform>().pos;
	int centerX = std::max(windowSize.x / 2.f, playerPos.x);
	
	m_GameView.setCenter(centerX, windowSize.y / 2.f);
	window.setView(m_GameView);

	window.clear(sf::Color(135, 206, 255));
	for (auto& e : m_EntityManager.getEntities()) {
		auto& transform = e->getComponent<CTransform>();
		if (!m_DrawBox) {
			if (e->hasComponent<CAnimation>()) {
				auto& animation = e->getComponent<CAnimation>().animation;
				animation.getSprite().setRotation(transform.angle);
				animation.getSprite().setPosition(
					transform.pos.x,
					transform.pos.y);

				window.draw(animation.getSprite());
			}
		}
		else {
			if (e->hasComponent<CBoundingBox>()) {
				auto& size = e->getComponent<CBoundingBox>().size;
				sf::RectangleShape box;
				box.setSize(sf::Vector2f(size.x, size.y));
				box.setPosition(transform.pos.x, transform.pos.y);
				box.setFillColor(sf::Color(0, 0, 0, 0));
				box.setOutlineColor(sf::Color(0, 0, 0));
				box.setOutlineThickness(1.f);
				window.draw(box);
			}
		}
	}

	m_mShape.setPosition(sf::Vector2f(m_mPos.x, m_mPos.y));
	window.draw(m_mShape);

	window.display();
}

Vec2 GameScene::windowToWorld(const Vec2& window) const {

	sf::View view = m_GameEngine->getWindow().getView();
	float wx = view.getCenter().x - m_GameEngine->getWindow().getSize().x / 2.f;
	float wy = view.getCenter().y - m_GameEngine->getWindow().getSize().y / 2.f;

	return Vec2(window.x + wx, window.y + wy);
}

void GameScene::spawnBullet() {
}

