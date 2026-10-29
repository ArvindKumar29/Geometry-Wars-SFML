#include "GameMode.h"
#include<iostream>
#include <fstream>
#include <cstdlib>
#include <ctime>
#include <cmath>
#include <SFML/System/Time.hpp>

sf::Clock Clock;

void GameMode::init(std::string path) {
	
	std::ifstream fin(path);
	if (!fin.is_open()) {
		std::cout << "ERROR!!!\n Could not open Config.txt!!!";
	}
	std::string type;
	int level;

	while (fin >> type) {
		if (type == "Window") {
			fin >> WindowConfig.width >> WindowConfig.height >> WindowConfig.framerateLimit;
		}
		else if (type == "Font") {
			fin >> FontConfig.fontPath >> FontConfig.fr >> FontConfig.fg >> FontConfig.fb;
		}
		else if (type == "Player") {
			fin >> PlayerConfig.V >> PlayerConfig.R >> PlayerConfig.S >> PlayerConfig.FR >> PlayerConfig.FG >> PlayerConfig.FB >>
				PlayerConfig.OR >> PlayerConfig.OG >> PlayerConfig.OB >> PlayerConfig.OT >> PlayerConfig.A;
		}
		else if (type == "Enemy") {
			fin >> EnemyConfig.VMAX >> EnemyConfig.VMIN >> EnemyConfig.RMAX >> EnemyConfig.RMIN >> EnemyConfig.FRMAX >> EnemyConfig.FGMAX >> EnemyConfig.FBMAX >>
				EnemyConfig.FRMIN >> EnemyConfig.FGMIN >> EnemyConfig.FBMIN >> EnemyConfig.ORMAX >> EnemyConfig.OGMAX >> EnemyConfig.OBMAX >>
				EnemyConfig.ORMIN >> EnemyConfig.OGMIN >> EnemyConfig.OBMIN >> EnemyConfig.OT >> EnemyConfig.SMAX >> EnemyConfig.SMIN;
		}
		else if (type == "Bullet") {
			fin >> BulletConfig.V >> BulletConfig.R >> BulletConfig.FR >> BulletConfig.FG >> BulletConfig.FB >>
				BulletConfig.OR >> BulletConfig.OG >> BulletConfig.OB >> BulletConfig.OT >> BulletConfig.S >> BulletConfig.l;
		}
		else if (type == "CurrentLevel") {
			fin >> level;
		}
	}
	if (level == 1)			currentLevel = LevelSpecs::loadLevelFromFile("Level1.txt");
	else if (level == 2)	currentLevel = LevelSpecs::loadLevelFromFile("Level2.txt");
	else if (level == 3)	currentLevel = LevelSpecs::loadLevelFromFile("Level3.txt");
	else std::cout << "Invalid level specified.\n";

	m_window.create(sf::VideoMode(WindowConfig.width, WindowConfig.height), "SFML Geometry Wars!!!");
	m_window.setFramerateLimit(WindowConfig.framerateLimit);
	m_player = m_entityManager.addEntity("player");
	m_player->CTransform = std::make_shared<Component::CTransform>(Vec2(WindowConfig.width/2, WindowConfig.height/2), Vec2(PlayerConfig.S, PlayerConfig.S));
	m_player->CShape = std::make_shared<Component::CShape>(PlayerConfig.V, PlayerConfig.R, sf::Color(PlayerConfig.FR, PlayerConfig.FG, PlayerConfig.FB), 
						sf::Color(PlayerConfig.OR, PlayerConfig.OG, PlayerConfig.OB), m_player->CTransform->pos, PlayerConfig.OT);
	m_player->CInput = std::make_shared<Component::CInput>();
	
	if (!m_font.loadFromFile(FontConfig.fontPath)) {
		std::cout << "ERROR: Could not load font from file:::::";
	}
	m_UIText.setFont(m_font);
	m_UIText.setString("SCORE: " + std::to_string(m_score) + " HEALTH: " + std::to_string(m_playerHealth));
	m_UIText.setFillColor(sf::Color(FontConfig.fr, FontConfig.fg, FontConfig.fb));
	//m_UIText.setPosition((WindowConfig.width - m_UIText.getGlobalBounds().width) * 0.5f, (WindowConfig.height - m_UIText.getGlobalBounds().height) * 0.5f);
	m_UIText.setPosition(0, 0);
}

void GameMode::run(std::string path) {
	init(path);
	while (m_window.isOpen()) {
		float dt = Clock.restart().asSeconds();
		m_entityManager.update();
		if (!m_paused) {
			sMovement(dt);
			sCollision(dt);
			sEnemySpawner(dt);
			sLifespan(dt);
			//m_gameState = GameState::Running;
			currentFrame++;
		}
		sUI();
		sInput(dt);
		sRender(m_window, dt);
	}
}

void GameMode::sRender(sf::RenderWindow& window, float dt) {
	window.clear();
	for (auto e : m_entityManager.getEntities()) {
		float rot = e->CShape->shape.getRotation();
		e->CShape->shape.setRotation((rot + 5.0f) * dt);
		window.draw(e->CShape->shape);
	}
	window.draw(m_UIText);
	window.display();
}

void GameMode::sMovement(float dt) {
	// MOVEMENT OF BULETS AND ENEMY USING TRANSFORM
	for (auto e : m_entityManager.getEntities()) {
		if (e->getTag() == "enemy") {

			// --- 1. STATE & TIMER LOGIC (Sirf Sochna) ---
			if (currentLevel.enemyRush) {
				if (!e->enemyInRush) {
					// Cooldown phase: wait kar rahe hain
					e->lastEnemyRushTime += dt;

					// Jaise hi cooldown khatam ho
					if (e->lastEnemyRushTime >= currentLevel.enemyRushCooldownTimer) {
						e->enemyInRush = true;
						e->lastEnemyRushTime = 0.0f; // Cooldown wapas 0 karo
						e->enemyRushTimer = 0.0f;    // Dash timer start karo

						// Dash ki direction sirf ek baar calculate karke save kar lo
						Vec2 dir = m_player->CTransform->pos - e->CTransform->pos;
						float dist = sqrt(dir.x * dir.x + dir.y * dir.y);
						if (dist > 0.0001f) {
							// Yahan normalization (dir.x / dist) bohot zaroori hai!
							e->rushVelocity.x = (dir.x / dist) * currentLevel.enemyRushSpeed;
							e->rushVelocity.y = (dir.y / dist) * currentLevel.enemyRushSpeed;
						}
						std::cout << "Enemy ID: " << e->getID() << " DASH STARTED!" << std::endl;
					}
				}
				else {
					// Dashing phase: dash chal raha hai
					e->enemyRushTimer += dt;

					// Dash ka time khatam hone par
					if (e->enemyRushTimer >= currentLevel.enemyRushTimer) {
						e->enemyInRush = false; // Dash rok do
						e->enemyRushTimer = 0.0f;
					}
				}
			}

			// --- 2. MOVEMENT APPLICATION (Asliyat me chalna) ---
			if (e->CTransform && e->CShape) {
				if (e->enemyInRush) {
					// Jo velocity save ki thi, usi velocity se dash karo (with dt)
					e->CTransform->pos.x += e->rushVelocity.x * dt;
					e->CTransform->pos.y += e->rushVelocity.y * dt;
				}
				else {
					// Normal aaram se chalna (Yahan 'dt' lagana zaroori hai)
					e->CTransform->pos.x += e->CTransform->vel.x * dt;
					e->CTransform->pos.y += e->CTransform->vel.y * dt;
				}

				// Final position shape par apply karo
				e->CShape->shape.setPosition(e->CTransform->pos.x, e->CTransform->pos.y);
			}
		}
		else if (e->getTag() == "small enemy") {
			if (e->CTransform && e->CShape) {
				// Yahan bhi dt lagana bhool gaye the aap
				e->CTransform->pos.x += e->CTransform->vel.x * dt;
				e->CTransform->pos.y += e->CTransform->vel.y * dt;
				e->CShape->shape.setPosition(e->CTransform->pos.x, e->CTransform->pos.y);
			}
		}
		else if (e->getTag() == "small enemy") {
			if (e->CTransform && e->CShape) {
				e->CTransform->pos.x += e->CTransform->vel.x;
				e->CTransform->pos.y += e->CTransform->vel.y;
				e->CShape->shape.setPosition(e->CTransform->pos.x, e->CTransform->pos.y);
			}
		}
		else if (e->getTag() == "bullet") {

			if (e->CTransform) {
				e->CTransform->pos.x += e->CTransform->vel.x * dt;
				e->CTransform->pos.y += e->CTransform->vel.y * dt;
				e->CShape->shape.setPosition(e->CTransform->pos.x, e->CTransform->pos.y);
			}
		}
	}
	// MOVEMENT FOR PLAYER USING INPUT
	if (currentLevel.canDashPlayer && m_player->CInput->alt) {
		// Player dash here
		if (m_player->CInput->left)
			m_player->CTransform->pos.x -= currentLevel.dashSpeedPlayer * dt;
		else if (m_player->CInput->right)
			m_player->CTransform->pos.x += currentLevel.dashSpeedPlayer * dt;
		else if (m_player->CInput->up)
			m_player->CTransform->pos.y -= currentLevel.dashSpeedPlayer * dt;
		else if (m_player->CInput->down)
			m_player->CTransform->pos.y += currentLevel.dashSpeedPlayer * dt;
	}
	if (m_player->CInput->up) {
		m_player->CTransform->pos.y -= currentLevel.movementSpeed * dt;
	}
	if (m_player->CInput->down) {
		m_player->CTransform->pos.y += currentLevel.movementSpeed * dt;
	}
	if (m_player->CInput->left) {
		m_player->CTransform->pos.x -= currentLevel.movementSpeed * dt;
	}
	if (m_player->CInput->right) {
		m_player->CTransform->pos.x += currentLevel.movementSpeed * dt;
	}
	m_player->CShape->shape.setPosition(m_player->CTransform->pos.x, m_player->CTransform->pos.y);
}

void GameMode::sCollision(float dt) {
	// COLLISION OF ENEMY TO WALLS
	for (auto e : m_entityManager.getEntities("enemy")) {
		// left wall
		if (e->CShape->shape.getPosition().x <= 40) {
			e->CTransform->vel.x *= -1;
		}
		// right wall
		if (e->CShape->shape.getPosition().x >= 1920) {
			e->CTransform->vel.x *= -1;
		}
		//Up and Down Walls

		// up wall
		if (e->CShape->shape.getPosition().y <= 40) {
			e->CTransform->vel.y *= -1;
		}
		// down wall
		if (e->CShape->shape.getPosition().y >= 1080) {
			e->CTransform->vel.y *= -1;
		}
	}

	// COLLISION FOR PLAYER TO WALLS!!!
	// left wall
	if (m_player->CShape->shape.getPosition().x <= 40) {
		m_player->CShape->shape.setPosition(40, m_player->CShape->shape.getPosition().y);
	}
	// right wall
	if (m_player->CShape->shape.getPosition().x >= 1880) {
		m_player->CShape->shape.setPosition(1880, m_player->CShape->shape.getPosition().y);
	}
	// Up and Down Walls

	// up wall
	if (m_player->CShape->shape.getPosition().y <= 40) {
		m_player->CShape->shape.setPosition(m_player->CShape->shape.getPosition().x, 40);
	}
	// down wall
	if (m_player->CShape->shape.getPosition().y >= 1040) {
		m_player->CShape->shape.setPosition(m_player->CShape->shape.getPosition().x, 1040);
	}

	// BULLET TO ENEMY
	for (auto b : m_entityManager.getEntities("bullet")) {
		if (!bossActive) {
			for (auto e : m_entityManager.getEntities("enemy")) {
				Vec2 p1 = e->CTransform->pos;
				Vec2 p2 = b->CTransform->pos;
				float radSQ = (e->CShape->shape.getRadius() + b->CShape->shape.getRadius()) * (e->CShape->shape.getRadius() + b->CShape->shape.getRadius());
				float distSQ = (p1.x - p2.x) * (p1.x - p2.x) + (p1.y - p2.y) * (p1.y - p2.y);
				if (distSQ <= radSQ) {
					// TO BE CHANGED FOR TAKING DAMAGE AND DIE ONCE HEALTH IS 0
					m_score += e->CShape->shape.getPointCount() + e->CShape->shape.getRadius();
					m_UIText.setString("SCORE: " + std::to_string(m_score) + " HEALTH: " + std::to_string(m_playerHealth));
					//m_healthText.setString("HEALTH: " + std::to_string(m_playerHealth));
					//m_window.draw(m_scoreText);
					//m_window.draw(m_healthText);
					e->destroy();
					smallEnemySpawner(e, dt);
					b->destroy();
				}
			}
		}
		else {
			for (auto e : m_entityManager.getEntities("boss")) {
				Vec2 p1 = e->CTransform->pos;
				Vec2 p2 = b->CTransform->pos;
				float radSQ = (e->CShape->shape.getRadius() + b->CShape->shape.getRadius()) * (e->CShape->shape.getRadius() + b->CShape->shape.getRadius());
				float distSQ = (p1.x - p2.x) * (p1.x - p2.x) + (p1.y - p2.y) * (p1.y - p2.y);
				if (distSQ <= radSQ) {
					// TO BE CHANGED FOR TAKING DAMAGE AND DIE ONCE HEALTH IS 0
					bossHealth -= 10;
					m_bossHealthText.setString("BOSS HEALTH: " + std::to_string(bossHealth));
					m_UIText.setString("SCORE: " + std::to_string(m_score) + " HEALTH: " + std::to_string(m_playerHealth) + " BOSS HEALTH: " + std::to_string(bossHealth));
					//m_window.draw(m_UIText);
					//m_window.draw(m_bossHealthText);
					if (bossHealth <= 0) {
						e->destroy();
						bossActive = false;
						m_paused = true;
						m_gameState = GameState::Victory;
					}
					b->destroy();
				}
			}
		}
	}

	// COLLISSION OF PLAYER TO ENEMY!!!
	if (!bossActive) {
		for (auto e : m_entityManager.getEntities("enemy")) {
			Vec2 p1 = m_player->CTransform->pos;
			Vec2 p2 = e->CTransform->pos;
			float radSQ = (e->CShape->shape.getRadius() + m_player->CShape->shape.getRadius()) * (e->CShape->shape.getRadius() + m_player->CShape->shape.getRadius());
			float distSQ = (p1.x - p2.x) * (p1.x - p2.x) + (p1.y - p2.y) * (p1.y - p2.y);
			if (distSQ <= radSQ) {
				// TO BE CHANGED FOR TAKING DAMAGE AND DIE ONCE HEALTH IS 0
				m_playerHealth -= 2;
				e->destroy();
				if (m_playerHealth <= 0) {
					m_player->destroy();
					m_paused = true;
					m_gameState = GameState::Dead;
				}
			}
		}
	}
	else {
		for (auto e : m_entityManager.getEntities("boss")) {
			Vec2 p1 = m_player->CTransform->pos;
			Vec2 p2 = e->CTransform->pos;
			float radSQ = (e->CShape->shape.getRadius() + m_player->CShape->shape.getRadius()) * (e->CShape->shape.getRadius() + m_player->CShape->shape.getRadius());
			float distSQ = (p1.x - p2.x) * (p1.x - p2.x) + (p1.y - p2.y) * (p1.y - p2.y);
			if (distSQ <= radSQ) {
				// TO BE CHANGED FOR TAKING DAMAGE AND DIE ONCE HEALTH IS 0
				m_playerHealth -= 5;
				if (m_playerHealth <= 0) {
					m_player->destroy();
					m_paused = true;
					m_gameState = GameState::Dead;
				}
			}
		}

	}
}


void GameMode::sInput(float dt) {
	sf::Event event;
	while (m_window.pollEvent(event)) {
		if (event.type == sf::Event::Closed) {
			m_window.close();
		}
		if (m_player->isAlive()) {
			if (event.type == sf::Event::KeyPressed) {
				switch (event.key.code)
				{
				case sf::Keyboard::W: m_player->CInput->up = true; break;
				case sf::Keyboard::A: m_player->CInput->left = true; break;;
				case sf::Keyboard::S: m_player->CInput->down = true; break;
				case sf::Keyboard::D: m_player->CInput->right = true; break;
				case sf::Keyboard::LAlt: m_player->CInput->alt = true; break;
				case sf::Keyboard::RAlt: m_player->CInput->alt = true; break;
				case sf::Keyboard::Space: m_player->CInput->SpecialAttack = true; break;
				case sf::Keyboard::P:
					if (m_gameState == GameState::Running || m_gameState == GameState::Paused) {
						m_paused = !m_paused;
						m_gameState = m_paused ? GameState::Paused : GameState::Running;
					}
					break;
				case sf::Keyboard::Escape: m_window.close();
				}
			}
			if (event.type == sf::Event::KeyReleased) {
				switch (event.key.code)
				{
				case sf::Keyboard::W: m_player->CInput->up = false; break;
				case sf::Keyboard::A: m_player->CInput->left = false; break;
				case sf::Keyboard::S: m_player->CInput->down = false; break;
				case sf::Keyboard::D: m_player->CInput->right = false; break;
				case sf::Keyboard::LAlt: m_player->CInput->alt = false; break;
				case sf::Keyboard::RAlt: m_player->CInput->alt = false; break;
				case sf::Keyboard::Space: m_player->CInput->SpecialAttack = false; break;
				}
			}
			if (event.type == sf::Event::MouseButtonPressed) {
				if (sf::Event::MouseLeft) {
					mousePos = Vec2(event.mouseButton.x, event.mouseButton.y);
					sPawnBullet( mousePos, dt);
				}
			}
			if (m_player->CInput->SpecialAttack && SpecialAttackCount > 0) {
				sSpawnSpecialAttack( mousePos, dt);
				SpecialAttackCount--;
			}
		}
	}
}

void GameMode::sSpawnEnemy(float dt) {
	int speed		= EnemyConfig.SMIN  + (rand() % (EnemyConfig.SMAX - EnemyConfig.SMIN + 1));
	int radius		= EnemyConfig.RMIN  + (rand() % (EnemyConfig.RMAX - EnemyConfig.RMIN + 1));
	int Vertices	= EnemyConfig.VMIN  + (rand() % (EnemyConfig.VMAX - EnemyConfig.VMIN + 1));
	int fillcolorR	= EnemyConfig.FRMIN + (rand() % (EnemyConfig.FRMAX - EnemyConfig.FRMIN + 1));
	int fillcolorG	= EnemyConfig.FGMIN + (rand() % (EnemyConfig.FGMAX - EnemyConfig.FGMIN + 1));
	int fillcolorB	= EnemyConfig.FBMIN + (rand() % (EnemyConfig.FBMAX - EnemyConfig.FBMIN + 1));
	int outcolorR	= EnemyConfig.ORMIN + (rand() % (EnemyConfig.ORMAX - EnemyConfig.ORMIN + 1));
	int outcolorG	= EnemyConfig.OGMIN + (rand() % (EnemyConfig.OGMAX - EnemyConfig.OGMIN + 1));
	int outcolorB	= EnemyConfig.OBMIN + (rand() % (EnemyConfig.OBMAX - EnemyConfig.OBMIN + 1));
	int posx = rand() % 1920;
	int posy = rand() % 1080;
	if (posx >= m_player->CTransform->pos.x - m_player->CShape->shape.getRadius() - 10 && posx <= m_player->CTransform->pos.x + m_player->CShape->shape.getRadius() + 10
		&& posy >= m_player->CTransform->pos.y - m_player->CShape->shape.getRadius() - 10 && posy <= m_player->CTransform->pos.y + m_player->CShape->shape.getRadius() + 10) {
		return;
	}
	std::shared_ptr<Entity> e = m_entityManager.addEntity("enemy");
	e->CTransform = std::make_shared<Component::CTransform>(Vec2(posx, posy), Vec2(speed, speed));
	e->CShape = std::make_shared<Component::CShape>(Vertices, radius, sf::Color(fillcolorR, fillcolorG, fillcolorB),
		sf::Color(outcolorR, outcolorG, outcolorB), e->CTransform->pos, EnemyConfig.OT);
	e->CCollision = std::make_shared<Component::CCollision>(radius);

}

void GameMode::sEnemySpawner(float dt) {
	if (currentFrame - lastEnemySpawnFrame >= 90 && m_score < currentLevel.targetScoreForBoss) {
		sSpawnEnemy( dt);
		lastEnemySpawnFrame = currentFrame;
	}
	else if (m_score >= currentLevel.targetScoreForBoss) {
		// Boss spawn logic here
		if (!bossActive)
		{
			int speed = EnemyConfig.SMAX * 1.5;
			int radius = EnemyConfig.RMAX * 2;
			int vertices = EnemyConfig.VMAX * 2;
			int fillColorR = 255;
			int fillColorG = 0;
			int fillColorB = 0;
			int outColorR = 255;
			int outColorG = 255;
			int outColorB = 255;
			int posX = 1920 - radius - 50;
			int posY = 540 - radius - 30;

			std::shared_ptr<Entity> boss = m_entityManager.addEntity("boss");
			boss->CTransform = std::make_shared<Component::CTransform>(Vec2(posX, posY), Vec2(speed, speed));
			boss->CShape = std::make_shared<Component::CShape>(vertices, radius, sf::Color(fillColorR, fillColorG, fillColorB),
				sf::Color(outColorR, outColorG, outColorB), boss->CTransform->pos, 2);
			boss->CCollision = std::make_shared<Component::CCollision>(radius);

			bossActive = true;
			m_UIText.setString("SCORE: " + std::to_string(m_score) + " HEALTH: " + std::to_string(m_playerHealth) + " BOSS HEALTH: " + std::to_string(bossHealth));

			for (auto e : m_entityManager.getEntities("enemy")) {
				e->destroy();
			}
		}
	}
}

void GameMode::smallEnemySpawner(std::shared_ptr<Entity> parent, float dt) {
	float pi = 3.14159f;
	int i = 0;
	while (i < parent->CShape->shape.getPointCount()) {
		auto se = m_entityManager.addEntity("small enemy");
		float angle = ((float)i / parent->CShape->shape.getPointCount()) * (2.0f * pi);
		se->CTransform = std::make_shared<Component::CTransform>(parent->CTransform->pos,
			Vec2(EnemyConfig.SMIN * std::cos(angle), EnemyConfig.SMIN * std::sin(angle)));
		se->CShape = std::make_shared<Component::CShape>(parent->CShape->shape.getPointCount(), parent->CShape->shape.getRadius() / 2,
			parent->CShape->shape.getFillColor(), parent->CShape->shape.getOutlineColor(), parent->CTransform->pos, 2);
		se->CLifespan = std::make_shared<Component::CLifespan>(30);
		i++;
	}
}

void GameMode::sPawnBullet(Vec2 mousePos, float dt) {
	std::shared_ptr<Entity> b = m_entityManager.addEntity("bullet");
	Vec2 dir = Vec2(mousePos.x - m_player->CTransform->pos.x, mousePos.y - m_player->CTransform->pos.y).normalize();
	Vec2 velocity = Vec2(currentLevel.bulletSpeed * dir.x, currentLevel.bulletSpeed * dir.y);
	b->CTransform = std::make_shared<Component::CTransform>(m_player->CTransform->pos, velocity);
	b->CShape = std::make_shared<Component::CShape>(BulletConfig.V, BulletConfig.R, sf::Color(BulletConfig.FR, BulletConfig.FG, BulletConfig.FB),
		sf::Color(BulletConfig.OB, BulletConfig.OG, BulletConfig.OB), b->CTransform->pos, 2);
	b->CCollision = std::make_shared<Component::CCollision>(BulletConfig.R);
	b->CLifespan = std::make_shared<Component::CLifespan>(BulletConfig.l);
}

void GameMode::sSpawnSpecialAttack(Vec2 mousePos, float dt) {
	for (int i = 0; i < 8; i++) {
		std::shared_ptr<Entity> b = m_entityManager.addEntity("bullet");
		float angle = (i / 8.0f) * (2.0f * 3.14159f);
		Vec2 dir = Vec2(std::cos(angle), std::sin(angle));
		Vec2 velocity = Vec2(currentLevel.bulletSpeed * dir.x, currentLevel.bulletSpeed * dir.y);
		b->CTransform = std::make_shared<Component::CTransform>(m_player->CTransform->pos, velocity);
		b->CShape = std::make_shared<Component::CShape>(BulletConfig.V, BulletConfig.R, sf::Color(BulletConfig.FR, BulletConfig.FG, BulletConfig.FB),
			sf::Color(BulletConfig.OB, BulletConfig.OG, BulletConfig.OB), b->CTransform->pos, 2);
		b->CCollision = std::make_shared<Component::CCollision>(BulletConfig.R);
		b->CLifespan = std::make_shared<Component::CLifespan>(BulletConfig.l);
	}
}

void GameMode::sLifespan(float dt) {
	for (auto b : m_entityManager.getEntities()) {
		if (b->getTag() == "bullet" || b->getTag() == "small enemy") {
			if (b->CLifespan->remaining == 0) {
				b->destroy();
			}
			b->CLifespan->remaining--;
			//int r = b->CShape->shape.getFillColor().r;
			//int g = b->CShape->shape.getFillColor().g;
			//int blue = b->CShape->shape.getFillColor().b;
			//int a = (b->CLifespan->remaining / 300) * 255;
			//sf::Color color(r, g, blue, a);
			//b->CShape->shape.setFillColor(color);
			//std::cout << "Lifespan for bullet ID: " << b->getID() << " remaining is: ";
		}

	}
}

bool GameMode::sPause() {
	return m_paused;
}

void GameMode::sUI() {
	switch(m_gameState) {
	case GameState::Running :
		m_UIText.setString("SCORE: " + std::to_string(m_score) + " HEALTH: " + std::to_string(m_playerHealth));
		break;
	case GameState::Paused :
		m_UIText.setString("Controls: WASD to move, Mouse to aim, Left Click to shoot, P to pause/start/resume, ESC to exit, Alt to dash, Space to special attack\nNewLine in the UI...\nHAVE FUN!");
		break;
	case GameState::Dead :
		m_UIText.setString("GAME OVER - ESC to exit");
		break;
	case GameState::Victory :
		m_UIText.setString("VICTORY! - ESC to exit");
		break;
	}
}

