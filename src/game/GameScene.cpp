#include "game/GameScene.h"
#include "engine/GameEngine.h"
#include "game/MenuScene.h"
#include "game/LevelSelectScene.h"
#include "engine/Physics.h"
#include <iostream>
#include <fstream>
#include <sstream>

GameScene::GameScene(GameEngine* gameEngine, const std::string& levelPath) :
	Scene(gameEngine),
	m_Level(levelPath),
	m_GameView(m_GameEngine->getWindow().getDefaultView()),
	m_GridSize(64, 64){
	
	init(levelPath);
}


void GameScene::init(const std::string& levelPath) {
	std::size_t found = levelPath.find_last_of("/\\");
	m_LevelTitle = levelPath.substr(found + 1);
	// removing .txt from level title
	for (int i = 0; i < 4; i++) {
		m_LevelTitle.pop_back();
	}

	m_GameTime = sf::Time::Zero;

	m_TimeText.setString(std::to_string(m_GameTime.asSeconds()));
	m_TimeText.setFillColor(sf::Color::White);
	m_TimeText.setFont(m_GameEngine->getAssets().getFont(Fonts::Main));
	m_TimeText.setCharacterSize(50);
	sf::FloatRect textRect = m_TimeText.getLocalBounds();
	

	m_TimeText.setOrigin(
		textRect.left + textRect.width / 2.f,
		textRect.top + textRect.height / 2.f
		);

	m_TimeText.setPosition(
		m_GameView.getCenter().x,
		25
	);

	m_EntityManager = EntityManager();

	registerAction(sf::Keyboard::W, Actions::Jump);
	registerAction(sf::Keyboard::A, Actions::Left);
	registerAction(sf::Keyboard::D, Actions::Right);
	registerAction(sf::Keyboard::J, Actions::Shoot);
	registerAction(sf::Keyboard::K, Actions::Dash);
	registerAction(sf::Keyboard::L, Actions::AirDash);
	//registerAction(sf::Keyboard::Space, Actions::Interact);
	registerAction(sf::Keyboard::Escape, Actions::Quit);
	registerAction(sf::Keyboard::B, Actions::ToggleBox);
	registerAction(sf::Keyboard::P, Actions::Pause);

	loadAssets();
	spawnPlayer();
	loadLevel(levelPath);
}

void GameScene::loadAssets() {
	//textures
	m_GameEngine->getAssets().addTexture(Textures::Ground, "graphics/dungeonGround.png");
	m_GameEngine->getAssets().addTexture(Textures::DoorExit, "graphics/dungeonDoorClosed.png");
	m_GameEngine->getAssets().addTexture(Textures::Climbable, "graphics/climbableWall.png");
	m_GameEngine->getAssets().addTexture(Textures::Explosion, "graphics/kiBlastExplode.png");
	m_GameEngine->getAssets().addTexture(Textures::Enemy, "graphics/dungeonEnemy.png");
	m_GameEngine->getAssets().addTexture(Textures::Background, "graphics/dungeonBackground-version2.png");
	m_GameEngine->getAssets().getTexture(Textures::Background).setRepeated(true);

	m_GameEngine->getAssets().addTexture(Textures::Idle, "graphics/megamanGohan.png");
	m_GameEngine->getAssets().addTexture(Textures::IdleLeft, "graphics/megamanGohanLeft.png");
	m_GameEngine->getAssets().addTexture(Textures::Run, "graphics/megamanGohanRun.png");
	m_GameEngine->getAssets().addTexture(Textures::RunLeft, "graphics/megamanGohanRunLeft.png");
	m_GameEngine->getAssets().addTexture(Textures::Jump, "graphics/megamanGohanJump.png");
	m_GameEngine->getAssets().addTexture(Textures::JumpLeft, "graphics/megamanGohanJumpLeft.png");
	m_GameEngine->getAssets().addTexture(Textures::Shoot, "graphics/megamanGohanAttack.png");
	m_GameEngine->getAssets().addTexture(Textures::ShootLeft, "graphics/megamanGohanAttackLeft.png");
	m_GameEngine->getAssets().addTexture(Textures::Blast, "graphics/kiBlast-Sheet.png");
	m_GameEngine->getAssets().addTexture(Textures::BlastLeft, "graphics/kiBlast-SheetLeft.png");
	m_GameEngine->getAssets().addTexture(Textures::Climb, "graphics/megamanGohanClimb.png");
	m_GameEngine->getAssets().addTexture(Textures::ClimbLeft, "graphics/megamanGohanClimbLeft.png");
	m_GameEngine->getAssets().addTexture(Textures::AirDash, "graphics/megamanGohanDash.png");
	m_GameEngine->getAssets().addTexture(Textures::AirDashLeft, "graphics/megamanGohanDashLeft.png");

	//sounds
	m_GameEngine->getAssets().addSoundBuffer(Sounds::Blast, "sounds/kiBlast.wav");
	m_GameEngine->getAssets().addSoundBuffer(Sounds::Jump, "sounds/jump.wav");
	m_KiBlast.setBuffer(m_GameEngine->getAssets().getSoundBuffer(Sounds::Blast));
	m_Jumping.setBuffer(m_GameEngine->getAssets().getSoundBuffer(Sounds::Jump));

	m_KiBlast.setVolume(50);

	//animations
	m_GameEngine->getAssets().addAnimation(Animations::Idle);
	m_GameEngine->getAssets().addAnimation(Animations::IdleLeft);
	m_GameEngine->getAssets().addAnimation(Animations::Run);
	m_GameEngine->getAssets().addAnimation(Animations::RunLeft);
	m_GameEngine->getAssets().addAnimation(Animations::Jump);
	m_GameEngine->getAssets().addAnimation(Animations::JumpLeft);
	m_GameEngine->getAssets().addAnimation(Animations::Shoot);
	m_GameEngine->getAssets().addAnimation(Animations::ShootLeft);
	m_GameEngine->getAssets().addAnimation(Animations::Blast);
	m_GameEngine->getAssets().addAnimation(Animations::BlastLeft);
	m_GameEngine->getAssets().addAnimation(Animations::Climb);
	m_GameEngine->getAssets().addAnimation(Animations::ClimbLeft);
	m_GameEngine->getAssets().addAnimation(Animations::AirDash);
	m_GameEngine->getAssets().addAnimation(Animations::AirDashLeft);
	m_GameEngine->getAssets().addAnimation(Animations::Ground);
	m_GameEngine->getAssets().addAnimation(Animations::Explosion);
	m_GameEngine->getAssets().addAnimation(Animations::Enemy);
	m_GameEngine->getAssets().addAnimation(Animations::DoorExit);
	m_GameEngine->getAssets().addAnimation(Animations::Climbable);
}


void GameScene::loadLevel(const std::string& levelPath){
	m_Background.setTexture(m_GameEngine->getAssets().getTexture(Textures::Background));
	m_Background.setTextureRect(sf::IntRect(0, 0, 128000, 72000));

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
		else if (fileEntities[0] == "AI") {
			addEnemy(fileEntities);
		}
		else if (fileEntities[0] == "Door") {
			addDoor(fileEntities);
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
	m_Player->getComponent<CBoundingBox>().size = Vec2(64,80);
	m_Player->getComponent<CBoundingBox>().has = true;

	m_Player->addComponent<CDash>();
	m_Player->addComponent<CSpeed>();
	m_Player->getComponent<CSpeed>().maxSpeed = 400;

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

void GameScene::addEnemy(std::vector<std::string>& fileEntities) {
	sf::Vector2u m_WindowSize = m_GameEngine->getWindow().getSize();
	Vec2 posn(std::stoi(fileEntities[2]), std::stoi(fileEntities[3]));
	sf::Texture& texture = m_GameEngine->getAssets().getTexture(Textures::Enemy);
	Vec2 size(texture.getSize().x, texture.getSize().y);

	auto e = m_EntityManager.addEntity(Entities::Enemy);
	e->addComponent<CTransform>();
	e->getComponent<CTransform>().pos.x = posn.x * 64;
	e->getComponent<CTransform>().pos.y = m_WindowSize.y - (posn.y + 3) * 64;

	e->addComponent<CAI>();
	e->addComponent<CGravity>();
	e->addComponent<CState>();
	e->getComponent<CState>().state = States::Air;

	e->addComponent<CAnimation>(m_GameEngine->getAssets().getAnimation(Animations::Enemy));
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

void GameScene::addDoor(std::vector<std::string>& fileEntities) {
	sf::Vector2u m_WindowSize = m_GameEngine->getWindow().getSize();
	Vec2 posn(std::stoi(fileEntities[2]), std::stoi(fileEntities[3]));
	sf::Texture& texture = m_GameEngine->getAssets().getTexture(Textures::DoorExit);
	Vec2 size(texture.getSize().x, texture.getSize().y);

	auto e = m_EntityManager.addEntity(Entities::Door);
	e->addComponent<CTransform>();
	e->getComponent<CTransform>().pos.x = posn.x * 64;
	e->getComponent<CTransform>().pos.y = m_WindowSize.y - (posn.y + 3) * 64;

	e->addComponent<CAnimation>(m_GameEngine->getAssets().getAnimation(Animations::DoorExit));
	e->getComponent<CAnimation>().animation.getSprite().setPosition(sf::Vector2f(
		e->getComponent<CTransform>().pos.x,
		e->getComponent<CTransform>().pos.y
	));
	e->getComponent<CAnimation>().animation.setRepeat(true);

	e->addComponent<CBoundingBox>();
	e->getComponent<CBoundingBox>().size.x = size.x;
	e->getComponent<CBoundingBox>().size.y = size.y;
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
	else if (type == "Climbable") {
		Vec2 posn(std::stoi(fileEntities[2]), std::stoi(fileEntities[3]));
		sf::Texture& texture = m_GameEngine->getAssets().getTexture(Textures::Climbable);
		Vec2 size(texture.getSize().x, texture.getSize().y);

		auto e = m_EntityManager.addEntity(Entities::Climbable);
		e->addComponent<CTransform>();
		e->getComponent<CTransform>().pos.x = posn.x * size.x;
		e->getComponent<CTransform>().pos.y = m_WindowSize.y - (posn.y + 1) * size.y;
		e->getComponent<CTransform>().has = true;

		e->addComponent<CAnimation>(m_GameEngine->getAssets().getAnimation(Animations::Climbable));
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

void GameScene::exitLevel() {
	for (auto& door : m_EntityManager.getEntities(Entities::Door)) {
		Vec2 playerPos = m_Player->getComponent<CTransform>().pos;
		if (Physics::IsInside(playerPos, door)) {
			std::shared_ptr<LevelSelectScene> lvlSelect(
				new LevelSelectScene(m_GameEngine, "levels/rooms/levelSelect.txt"));
			m_GameEngine->changeScene(Scenes::LevelSelect, lvlSelect);

			try {
				std::ofstream recordsFile("records/records.txt", std::fstream::app);
				std::string line = m_LevelTitle + " " + std::to_string(m_GameTime.asSeconds()) + "\n";
				recordsFile << line;
				recordsFile.close();
			}
			catch (std::fstream::failure e) {
				std::cout << e.what() << std::endl;
				return;
			}
			break;
		}
	}
}

void GameScene::update(float dt){
	m_EntityManager.update();
	//std::cout << m_EntityManager.getEntities().size() << std::endl;
	
	m_GameTime += sf::seconds(dt);
	m_TimeText.setString(std::to_string(m_GameTime.asSeconds()));

	if (!m_Paused) {
		sLifespan(dt);
		sAI();
		sMovement(dt);
		sDraggable();
		sCollision();
		sAnimation();
		exitLevel();
	}
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
			if (m_Player->getComponent<CAnimation>().animation.getType() != Animations::RunLeft) {
				m_Player->addComponent<CAnimation>(m_GameEngine->getAssets().getAnimation(Animations::RunLeft));
				m_Player->getComponent<CAnimation>().animation.setRepeat(true);
			}
		}
		else if (m_Player->getComponent<CInput>().up) {
			if (m_Player->getComponent<CAnimation>().animation.getType() == Animations::Run ||
				m_Player->getComponent<CAnimation>().animation.getType() == Animations::Idle) {
				m_Player->addComponent<CAnimation>(m_GameEngine->getAssets().getAnimation(Animations::Jump));
			}
			else if (m_Player->getComponent<CAnimation>().animation.getType() == Animations::RunLeft ||
				m_Player->getComponent<CAnimation>().animation.getType() == Animations::IdleLeft) {
				m_Player->addComponent<CAnimation>(m_GameEngine->getAssets().getAnimation(Animations::JumpLeft));
			}
		}
		else if (m_Player->getComponent<CInput>().shoot) {
			if (m_Player->getComponent<CAnimation>().animation.getType() == Animations::Run ||
				m_Player->getComponent<CAnimation>().animation.getType() == Animations::Idle ||
				m_Player->getComponent<CAnimation>().animation.getType() == Animations::Jump) {
				m_Player->addComponent<CAnimation>(m_GameEngine->getAssets().getAnimation(Animations::Shoot));
			}
			else if (m_Player->getComponent<CAnimation>().animation.getType() == Animations::RunLeft ||
				m_Player->getComponent<CAnimation>().animation.getType() == Animations::IdleLeft ||
				m_Player->getComponent<CAnimation>().animation.getType() == Animations::JumpLeft ) {
				m_Player->addComponent<CAnimation>(m_GameEngine->getAssets().getAnimation(Animations::ShootLeft));
			}
			m_Player->getComponent<CAnimation>().animation.setRepeat(true);
		}
		else {
			if (m_Player->getComponent<CAnimation>().animation.getType() == Animations::Run ||
				m_Player->getComponent<CAnimation>().animation.getType() == Animations::Jump ||
				m_Player->getComponent<CAnimation>().animation.getType() == Animations::Shoot) {
				m_Player->addComponent<CAnimation>(m_GameEngine->getAssets().getAnimation(Animations::Idle));
			}
			else if (m_Player->getComponent<CAnimation>().animation.getType() == Animations::RunLeft ||
				m_Player->getComponent<CAnimation>().animation.getType() == Animations::JumpLeft ||
				m_Player->getComponent<CAnimation>().animation.getType() == Animations::ShootLeft) {
				m_Player->addComponent<CAnimation>(m_GameEngine->getAssets().getAnimation(Animations::IdleLeft));
			}
		}
	}
	else if (m_Player->getComponent<CState>().state == States::Air) {
		if (m_Player->getComponent<CAnimation>().animation.getType() == Animations::Run ||
			m_Player->getComponent<CAnimation>().animation.getType() == Animations::Idle ||
			m_Player->getComponent<CAnimation>().animation.getType() == Animations::Climb ||
			m_Player->getComponent<CAnimation>().animation.getType() == Animations::AirDash) {
			m_Player->addComponent<CAnimation>(m_GameEngine->getAssets().getAnimation(Animations::Jump));
		}
		else if (m_Player->getComponent<CAnimation>().animation.getType() == Animations::RunLeft ||
			m_Player->getComponent<CAnimation>().animation.getType() == Animations::IdleLeft ||
			m_Player->getComponent<CAnimation>().animation.getType() == Animations::ClimbLeft ||
			m_Player->getComponent<CAnimation>().animation.getType() == Animations::AirDashLeft) {
			m_Player->addComponent<CAnimation>(m_GameEngine->getAssets().getAnimation(Animations::JumpLeft));
		}
	}
	else if (m_Player->getComponent<CState>().state == States::AirDash) {
		if (m_Player->getComponent<CAnimation>().animation.getType() == Animations::Jump) {
			m_Player->addComponent<CAnimation>(m_GameEngine->getAssets().getAnimation(Animations::AirDash));
		}
		else if (m_Player->getComponent<CAnimation>().animation.getType() == Animations::JumpLeft) {
			m_Player->addComponent<CAnimation>(m_GameEngine->getAssets().getAnimation(Animations::AirDashLeft));
		}
	}
	else if (m_Player->getComponent<CState>().state == States::Climb) {
		// check whether the player wants to detach from the climbable entity
		if (m_Player->getComponent<CAnimation>().animation.getType() == Animations::ClimbLeft) {
			if (m_Player->getComponent<CInput>().left ||
				(m_Player->getComponent<CInput>().left &&
					m_Player->getComponent<CInput>().up)) {
				m_Player->getComponent<CState>().state = States::Air;
				// setting animation here caused bugs, only set animations in 1 place, just change state to 
				// control what the animation changes to
				//m_Player->addComponent<CAnimation>(m_GameEngine->getAssets().getAnimation(Animations::JumpLeft));
				//m_Player->getComponent<CAnimation>().animation.setRepeat(true);
			}
		}
		if (m_Player->getComponent<CAnimation>().animation.getType() == Animations::Climb) {
			if (m_Player->getComponent<CInput>().right ||
				(m_Player->getComponent<CInput>().right &&
					m_Player->getComponent<CInput>().up)) {
				m_Player->getComponent<CState>().state = States::Air;
				//m_Player->addComponent<CAnimation>(m_GameEngine->getAssets().getAnimation(Animations::Jump));
				//m_Player->getComponent<CAnimation>().animation.setRepeat(true);
			}
		}
		// check if the player is trying to latch onto the climbable entity
		if (m_Player->getComponent<CAnimation>().animation.getType() == Animations::Run || 
			m_Player->getComponent<CAnimation>().animation.getType() == Animations::Jump ||
			m_Player->getComponent<CAnimation>().animation.getType() == Animations::AirDash ||
			m_Player->getComponent<CTransform>().velocity.x > 0) {
			m_Player->addComponent<CAnimation>(m_GameEngine->getAssets().getAnimation(Animations::ClimbLeft));
		}
		if (m_Player->getComponent<CAnimation>().animation.getType() == Animations::RunLeft ||
			m_Player->getComponent<CAnimation>().animation.getType() == Animations::JumpLeft ||
			m_Player->getComponent<CAnimation>().animation.getType() == Animations::AirDashLeft ||
			m_Player->getComponent<CTransform>().velocity.x < 0) {
			m_Player->addComponent<CAnimation>(m_GameEngine->getAssets().getAnimation(Animations::Climb));
		}
	}

	for (auto& e : m_EntityManager.getEntities()) {
		if (e->hasComponent<CAnimation>()) {
			e->getComponent<CAnimation>().animation.update();
			if (e->getComponent<CAnimation>().animation.hasEnded() && e->tag() != Entities::Player) {
				e->destroy();
			}
			else if (e->getComponent<CAnimation>().animation.hasEnded() && e->tag() == Entities::Player) {
				if (m_Player->getComponent<CAnimation>().animation.getType() == Animations::AirDash ||
					m_Player->getComponent<CAnimation>().animation.getType() == Animations::AirDashLeft) {
					m_Player->getComponent<CState>().state = States::Air;
				}
			}
		}
	}
}

void GameScene::sDoAction(const Action& action){
	if (action.getType() == Actions::Start) {
		if (action.getName() == Actions::Quit) {
			std::shared_ptr<LevelSelectScene> lvlSelect(
				new LevelSelectScene(m_GameEngine, "levels/rooms/levelSelect.txt"));
			m_GameEngine->changeScene(Scenes::LevelSelect, lvlSelect);
		}
		if (action.getName() == Actions::Right) {
			m_Player->getComponent<CInput>().right = true;
		}
		if (action.getName() == Actions::Left) {
			m_Player->getComponent<CInput>().left = true;
		}
		if (action.getName() == Actions::Jump) {
			if (m_Player->getComponent<CState>().state != States::Air) {
				m_Player->getComponent<CState>().state = States::Air;
				m_Jumping.play();
				m_Player->getComponent<CInput>().up = true;
			}
			else {
				m_Player->getComponent<CInput>().up = false;
			}
		}
		if (action.getName() == Actions::Shoot 
			&& m_EntityManager.getEntities(Entities::Bullet).size() <= 3
			&& m_Player->getComponent<CState>().state != States::Climb) {
			m_Player->getComponent<CInput>().shoot = true;
			m_KiBlast.play();
			spawnBullet();
		}
		else {
			m_Player->getComponent<CInput>().shoot = false;
		}
		if (action.getName() == Actions::Dash) {
			if (m_Player->getComponent<CState>().state != States::Air) {
				m_Player->getComponent<CSpeed>().maxSpeed = 600;
			}
			else {
				m_Player->getComponent<CSpeed>().maxSpeed = 400;
			}
		}
		if (action.getName() == Actions::AirDash) {
			if (m_Player->getComponent<CState>().state == States::Air &&
				 !m_Player->getComponent<CDash>().hasDashed) {
				m_Jumping.play();
				m_Player->getComponent<CState>().state = States::AirDash;
				m_Player->getComponent<CDash>().hasDashed = true;
			}
		}
		if (action.getName() == Actions::Pause) {
			m_Paused = !m_Paused;
		}
		if (action.getName() == Actions::ToggleBox) {
			m_DrawBox = !m_DrawBox;
		}
		if (action.getName() == Actions::MouseLeft) {
			Vec2 m_Pos = windowToWorld(action.getPos());
			for (auto& e : m_EntityManager.getEntities()) {
				if (e->hasComponent<CDraggable>() && Physics::IsInside(m_Pos, e)) {
					e->getComponent<CDraggable>().dragging = !e->getComponent<CDraggable>().dragging;
				}
			}
		}
		if (action.getName() == Actions::MouseMoved) {
			Vec2 mpos = action.getPos();
			m_mPos = windowToWorld(mpos);
		}
		/*if (action.getName() == Actions::Interact) {
			exitLevel();
		}*/
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
		if (action.getName() == Actions::Shoot) {
			m_Player->getComponent<CInput>().shoot = false;
		}
		if (action.getName() == Actions::Dash) {
			m_Player->getComponent<CSpeed>().maxSpeed = 400;
		}
	/*	if (action.getName() == Actions::AirDash) {
			if (m_Player->getComponent<CState>().state == States::AirDash &&
				m_Player->getComponent<CAnimation>().animation.hasEnded()) {
				m_Player->getComponent<CState>().state = States::Air;
			}
		}*/
	}
}

void GameScene::sMovement(float dt){
	// player movement
	Vec2 playerVel;
	if (m_Player->getComponent<CState>().state == States::AirDash) {
		if (m_Player->getComponent<CAnimation>().animation.getType() == Animations::AirDash) {
			playerVel.x = 1200;
			playerVel.y = 0;
		}
		else if (m_Player->getComponent<CAnimation>().animation.getType() == Animations::AirDashLeft) {
			playerVel.x = -1200;
			playerVel.y = 0;
		}
	}
	else {
		if (m_Player->getComponent<CInput>().right) {
			if (m_Player->getComponent<CTransform>().velocity.x <
				m_Player->getComponent<CSpeed>().maxSpeed) {
				playerVel.x = m_Player->getComponent<CTransform>().velocity.x + 100;
			}
			else {
				playerVel.x = m_Player->getComponent<CSpeed>().maxSpeed;
			}
		}

		if (m_Player->getComponent<CInput>().left) {
			if (m_Player->getComponent<CTransform>().velocity.x >
				-m_Player->getComponent<CSpeed>().maxSpeed) {
				playerVel.x = m_Player->getComponent<CTransform>().velocity.x - 100;
			}
			else {
				playerVel.x = -m_Player->getComponent<CSpeed>().maxSpeed;
			}
		}

		if (m_Player->getComponent<CInput>().up &&
			m_Player->getComponent<CTransform>().velocity.y >= 0 &&
			m_Player->getComponent<CState>().state == States::Ground) {
			playerVel.y = -750;
			m_Player->getComponent<CState>().state = States::Air;
		}
		else if (m_Player->getComponent<CInput>().up &&
			m_Player->getComponent<CState>().state == States::Climb) {
			if (m_Player->getComponent<CAnimation>().animation.getType() == Animations::ClimbLeft) {
				playerVel.y = -450;
				playerVel.x = -600;
				m_Player->getComponent<CState>().state = States::Air;
			}
			else if (m_Player->getComponent<CAnimation>().animation.getType() == Animations::Climb) {
				playerVel.y = -450;
				playerVel.x = 600;
				m_Player->getComponent<CState>().state = States::Air;
			}
		}
		else {
			playerVel.y = m_Player->getComponent<CTransform>().velocity.y;
		}
	}

	m_Player->getComponent<CTransform>().velocity = playerVel;

	if (m_Player->getComponent<CTransform>().pos.x < 
		m_Player->getComponent<CBoundingBox>().size.x) {
		m_Player->getComponent<CTransform>().pos.x = m_Player->getComponent<CBoundingBox>().size.x;
	}

	if (m_Player->getComponent<CTransform>().pos.y > m_GameEngine->getWindow().getSize().y) {
		m_Player->getComponent<CTransform>().pos = m_Player->getComponent<CBoundingBox>().size;
	}

	// enemy movement
	for (auto& e : m_EntityManager.getEntities(Entities::Enemy)) {
		if (e->getComponent<CAI>().state == AIStates::Follow) {
			if (e->getComponent<CTransform>().pos.x <
				m_Player->getComponent<CTransform>().pos.x) {
				e->getComponent<CTransform>().velocity.x = 200;
			}
			else {
				e->getComponent<CTransform>().velocity.x = -200;
			}
		}
		else if (e->getComponent<CAI>().state == AIStates::Idle) {
			e->getComponent<CTransform>().velocity.x = 0;
		}
	}

	for (auto& e : m_EntityManager.getEntities()) {
		e->getComponent<CTransform>().prevPos = e->getComponent<CTransform>().pos;
		if (e->hasComponent<CGravity>()) {
			if (e->hasComponent<CState>() &&
				e->getComponent<CState>().state == States::Climb) {
				e->getComponent<CTransform>().velocity.y = 50;
			}
			else if (e->hasComponent<CState>() &&
				e->getComponent<CState>().state == States::AirDash) {
				e->getComponent<CTransform>().velocity.y = 0;
			}
			else {
				e->getComponent<CTransform>().velocity.y += e->getComponent<CGravity>().gravity * dt * 0.5f;
			}
		}
		e->getComponent<CTransform>().pos = e->getComponent<CTransform>().pos +
			e->getComponent<CTransform>().velocity * dt;
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

void GameScene::sAI() {
	for (auto& e : m_EntityManager.getEntities(Entities::Enemy)) {
		Vec2 eDist = e->getComponent<CTransform>().pos - m_Player->getComponent<CTransform>().pos;
		// if the player is far away, do nothing
		if (len(eDist) > 384.f) {
			e->getComponent<CTransform>().velocity.x = 0;
			continue;
		}
		for (auto& f : m_EntityManager.getEntities()) {
			// use m_Player
			if (f->tag() == Entities::Player) {
				continue;
			}
			// ignore other enemies
			if (f->tag() == Entities::Enemy) {
				continue;
			}
			// check if there is an entity between the player and the enemy
			Vec2 a = m_Player->getComponent<CTransform>().pos;
			Vec2 b = e->getComponent<CTransform>().pos;
			if (a.x > b.x) {
				Vec2 temp = a;
				a = b;
				b = temp;
			}
			Physics::Intersect entityIntersect = Physics::EntityIntersection(a, b, f);
			if (entityIntersect.isIntersect) {
				e->getComponent<CAI>().state = AIStates::Idle;
			}
			else {
				e->getComponent<CAI>().state = AIStates::Follow;
			}
		}
	}
}

void GameScene::sCollision(){
	for (auto& e : m_EntityManager.getEntities(Entities::Enemy)) {
		if (e->getComponent<CTransform>().pos.x < 64) {
			e->getComponent<CTransform>().velocity.x =
				-e->getComponent<CTransform>().velocity.x;
		}
		// collisions with floor entities
		for (auto& f : m_EntityManager.getEntities(Entities::Floor)) {
			Vec2 overlap = Physics::GetOverlap(e, f);
			Vec2 prevOverlap = Physics::GetPreviousOverlap(e, f);
			if (overlap.x > 0.0f && overlap.y > 0.0f) {
				if (prevOverlap.y > 0 && e->getComponent<CTransform>().prevPos.x >
					f->getComponent<CTransform>().prevPos.x) {
					e->getComponent<CTransform>().pos.x += overlap.x;
				}
				if (prevOverlap.y > 0 && e->getComponent<CTransform>().prevPos.x <
					f->getComponent<CTransform>().prevPos.x) {
					e->getComponent<CTransform>().pos.x -= overlap.x;
				}
				if (prevOverlap.x > 0) {
					e->getComponent<CTransform>().pos.y -= overlap.y;
					e->getComponent<CTransform>().velocity.y = 0;
					e->getComponent<CState>().state = States::Ground;
				}
			}
		}
		// collisions with climable entities
		for (auto& c : m_EntityManager.getEntities(Entities::Climbable)) {
			Vec2 overlap = Physics::GetOverlap(e, c);
			Vec2 prevOverlap = Physics::GetPreviousOverlap(e, c);
			if (overlap.x > 0.0f && overlap.y > 0.0f) {
				if (prevOverlap.y > 0 && e->getComponent<CTransform>().prevPos.x >
					c->getComponent<CTransform>().prevPos.x) {
					e->getComponent<CTransform>().pos.x += overlap.x;
				}
				if (prevOverlap.y > 0 && e->getComponent<CTransform>().prevPos.x <
					c->getComponent<CTransform>().prevPos.x) {
					e->getComponent<CTransform>().pos.x -= overlap.x;
				}
				if (prevOverlap.x > 0) {
					e->getComponent<CTransform>().pos.y -= overlap.y;
					e->getComponent<CTransform>().velocity.y = 0;
					e->getComponent<CState>().state = States::Ground;
				}
			}
		}
		// collisions with bullet entities
		for (auto& b : m_EntityManager.getEntities(Entities::Bullet)) {
			Vec2 overlap = Physics::GetOverlap(e, b);
			if (overlap.x > 0.0f && overlap.y > 0.0f) {
				e->destroy();
				b->destroy();
			}
		}
		// collision with player
		Vec2 overlap = Physics::GetOverlap(e, m_Player);
		if (overlap.x > 0.0f && overlap.y > 0.0f) {
			e->getComponent<CAI>().state = AIStates::Idle;
			m_Player->getComponent<CTransform>().pos = Vec2(1, 1);
			m_Player->getComponent<CState>().state = States::Air;
		}
	}

	// bullet collisions
	for (auto& b : m_EntityManager.getEntities(Entities::Bullet)) {
		// collision with floor entities
		for (auto& f : m_EntityManager.getEntities(Entities::Floor)) {
			Vec2 overlap = Physics::GetOverlap(b, f);
			if (overlap.x > 0.0f && overlap.y > 0.0f) {
				b->destroy();
				break;
			}
		}

		// collsion with climbable entities
		for (auto& c : m_EntityManager.getEntities(Entities::Climbable)) {
			Vec2 overlap = Physics::GetOverlap(b, c);
			if (overlap.x > 0.0f && overlap.y > 0.0f) {
				b->destroy();
				break;
			}
		}
	}

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
					m_Player->getComponent<CDash>().hasDashed = false;
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
		// climbable entities
		if (e->tag() == Entities::Climbable) {
			Vec2 overlap = Physics::GetOverlap(m_Player, e);
			Vec2 prevOverlap = Physics::GetPreviousOverlap(m_Player, e);
			if (overlap.y > 0.0f && overlap.x > 0.0f) {
				if (prevOverlap.x > 0 && m_Player->getComponent<CTransform>().prevPos.y <
					e->getComponent<CTransform>().prevPos.y) {
					m_Player->getComponent<CTransform>().pos.y -= overlap.y;
					m_Player->getComponent<CTransform>().velocity.y = 0;
					m_Player->getComponent<CState>().state = States::Ground;
					m_Player->getComponent<CDash>().hasDashed = false;
				}
				else if (prevOverlap.x > 0 && m_Player->getComponent<CTransform>().prevPos.y >
					e->getComponent<CTransform>().prevPos.y) {
					m_Player->getComponent<CTransform>().pos.y += overlap.y;
					m_Player->getComponent<CTransform>().velocity.y = 0;
					m_Player->getComponent<CState>().state = States::Air;
				}
				if (prevOverlap.y > 0 && m_Player->getComponent<CTransform>().prevPos.x >
					e->getComponent<CTransform>().prevPos.x) {
					m_Player->getComponent<CTransform>().pos.x += overlap.x;
					m_Player->getComponent<CState>().state = States::Climb;
					m_Player->getComponent<CDash>().hasDashed = false;
				}
				else if (prevOverlap.y > 0 && m_Player->getComponent<CTransform>().prevPos.x <
					e->getComponent<CTransform>().prevPos.x) {
					m_Player->getComponent<CTransform>().pos.x -= overlap.x;
					m_Player->getComponent<CState>().state = States::Climb;
					m_Player->getComponent<CDash>().hasDashed = false;
				}
			}
		}
	}
}

void GameScene::sLifespan(float dt) {
	for (auto& e : m_EntityManager.getEntities()) {
		if (e->hasComponent<CLifespan>()) {
			if(e->getComponent<CLifespan>().total <= 0.0f){
				e->destroy();
			}
			else {
				e->getComponent<CLifespan>().total -= dt;
			}
		}
	}
}

void GameScene::sRender(){
	sf::RenderWindow& window = m_GameEngine->getWindow();
	sf::Vector2u windowSize = window.getSize();
	
	auto& playerPos = m_Player->getComponent<CTransform>().pos;
	int centerX = std::max(windowSize.x / 2.f, playerPos.x);
	//int centerY = std::min(windowSize.y / 2.f, playerPos.y);
	
	m_GameView.setCenter(centerX, windowSize.y / 2.f);
	window.setView(m_GameView);

	window.clear(sf::Color(135, 206, 255));
	if (!m_DrawBox) {
		window.draw(m_Background);
	}

	// draw door first
	for (auto& e : m_EntityManager.getEntities(Entities::Door)) {
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

	for (auto& e : m_EntityManager.getEntities()) {
		auto& transform = e->getComponent<CTransform>();
		// skip door
		if (e->tag() == Entities::Door) {
			continue;
		}
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

	m_TimeText.setPosition(m_GameView.getCenter().x, 25);
	window.draw(m_TimeText);

//	m_mShape.setPosition(sf::Vector2f(m_mPos.x, m_mPos.y));
//	window.draw(m_mShape);

	window.display();
}

Vec2 GameScene::windowToWorld(const Vec2& window) const {

	sf::View view = m_GameEngine->getWindow().getView();
	float wx = view.getCenter().x - m_GameEngine->getWindow().getSize().x / 2.f;
	float wy = view.getCenter().y - m_GameEngine->getWindow().getSize().y / 2.f;

	return Vec2(window.x + wx, window.y + wy);
}

void GameScene::spawnBullet() {
	sf::Vector2u m_WindowSize = m_GameEngine->getWindow().getSize();
	sf::Texture& texture = m_GameEngine->getAssets().getTexture(Textures::Blast);
	Vec2 size(texture.getSize().x, texture.getSize().y);

	bool isLeft = m_Player->getComponent<CAnimation>().animation.getType() == Animations::ShootLeft ||
		m_Player->getComponent<CAnimation>().animation.getType() == Animations::IdleLeft ||
		m_Player->getComponent<CAnimation>().animation.getType() == Animations::JumpLeft ||
		m_Player->getComponent<CAnimation>().animation.getType() == Animations::RunLeft ;

	auto e = m_EntityManager.addEntity(Entities::Bullet);
	e->addComponent<CTransform>();
	e->getComponent<CTransform>().pos.x = isLeft ? m_Player->getComponent<CTransform>().pos.x :
		m_Player->getComponent<CTransform>().pos.x + 48;
	e->getComponent<CTransform>().pos.y = m_Player->getComponent<CTransform>().pos.y + 24;
	e->getComponent<CTransform>().velocity.x = isLeft ? -800.f : 800.f;

	
	e->addComponent<CAnimation>(isLeft ? m_GameEngine->getAssets().getAnimation(Animations::BlastLeft) :
		m_GameEngine->getAssets().getAnimation(Animations::Blast));
	e->getComponent<CAnimation>().animation.getSprite().setPosition(sf::Vector2f(
		e->getComponent<CTransform>().pos.x,
		e->getComponent<CTransform>().pos.y
	));
	e->getComponent<CAnimation>().animation.setRepeat(true);

	e->addComponent<CLifespan>();
	e->getComponent<CLifespan>().total = 50.f;
	e->addComponent<CBoundingBox>();
	e->getComponent<CBoundingBox>().size.x = size.y * 1.5f;
	e->getComponent<CBoundingBox>().size.y = size.y / 2.f;
}

