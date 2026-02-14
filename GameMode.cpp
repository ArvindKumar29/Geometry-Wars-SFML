#include "GameMode.h"
#include<iostream>
#include <fstream>
#include <cstdlib>
#include <ctime>

void GameMode::init(std::string path) {
	std::ifstream fin(path);
	if (!fin.is_open()) {
		std::cout << "ERROR!!!\n Could not open Config.txt!!!";
	}
	std::string type;
	int w, h, fl,fr, fg, fb;
	std::string font;
	
	while (fin >> type) {
		if (type == "Window") {
			fin >> w >> h >> fl;
		}
		else if (type == "Font") {
			fin >> font >> fr >> fg >> fb;
		}
		else if (type == "Player") {
			fin >> PlayerConfig.V >> PlayerConfig.R >> PlayerConfig.S >>PlayerConfig.FR >> PlayerConfig.FG >> PlayerConfig.FB >> 
				PlayerConfig.OR >> PlayerConfig.OG >> PlayerConfig.OB >> PlayerConfig.OT >> PlayerConfig.A;
		}
		else if (type == "Enemy") {
			fin >> EnemyConfig.VMAX >> EnemyConfig.VMIN >> EnemyConfig.RMAX >> EnemyConfig.RMIN >> EnemyConfig.FRMAX >> EnemyConfig.FGMAX >> EnemyConfig.FBMAX >>
				EnemyConfig.FRMIN >> EnemyConfig.FGMIN >> EnemyConfig.FBMIN>> EnemyConfig.ORMAX >> EnemyConfig.OGMAX >> EnemyConfig.OBMAX >> 
				EnemyConfig.ORMIN >> EnemyConfig.OGMIN >> EnemyConfig.OBMIN>> EnemyConfig.OT >> EnemyConfig.SMAX >> EnemyConfig.SMIN;
		}
		else if (type == "Bullet") {
			fin >> BulletConfig.V >> BulletConfig.R >> BulletConfig.FR >> BulletConfig.FG >> BulletConfig.FB >>
				BulletConfig.OR >> BulletConfig.OG >> BulletConfig.OB >> BulletConfig.OT >> BulletConfig.S >> BulletConfig.l;
		}
	}	

	m_window.create(sf::VideoMode(w, h), "SFML Geometry Wars!!!");
	m_window.setFramerateLimit(fl);
	m_player = m_entityManager.addEntity("player");
	m_player->CTransform = std::make_shared<Component::CTransform>(Vec2(w/2, h/2), Vec2(PlayerConfig.S, PlayerConfig.S));
	m_player->CShape = std::make_shared<Component::CShape>(PlayerConfig.V, PlayerConfig.R, sf::Color(PlayerConfig.FR, PlayerConfig.FG, PlayerConfig.FB), 
						sf::Color(PlayerConfig.OR, PlayerConfig.OG, PlayerConfig.OB), m_player->CTransform->pos, PlayerConfig.OT);
	m_player->CInput = std::make_shared<Component::CInput>();
	
	if (!m_font.loadFromFile(font)) {
		std::cout << "ERROR: Could not load font from file:::::";
	}
	m_scoreText.setFont(m_font);
	m_scoreText.setString("SCORE: " + std::to_string(m_score));
	m_scoreText.setFillColor(sf::Color(fr, fg, fb));
	m_scoreText.setPosition(0.0f, 0.0f);
}

void GameMode::run(std::string path) {
	init(path);
	while (m_window.isOpen()) {
		m_entityManager.update();
		if (!m_paused) {
			sMovement();
			sCollision();
			sEnemySpawner();
			sLifespan();
			currentFrame++;
		}
		sInput();
		sRender();
	}
}
void GameMode::sRender() {
	m_window.clear();
	for (auto e : m_entityManager.getEntities()) {
		m_window.draw(e->CShape->shape);
	}
	m_window.draw(m_scoreText);
	m_window.display();
}

void GameMode::sMovement() {
	// MOVEMENT OF BULETS AND ENEMY USING TRANSFORM
	for (auto e : m_entityManager.getEntities()) {
		if (e->getTag() == "enemy" || e->getTag() == "bullet") {
			if (e->CTransform && e->CShape) {
				e->CTransform->pos.x += e->CTransform->vel.x;
				e->CTransform->pos.y += e->CTransform->vel.y;
				e->CShape->shape.setPosition(e->CTransform->pos.x, e->CTransform->pos.y);
			}
		}
	}

	// MOVEMENT FOR PLAYER USING INPUT
	if (m_player->CInput->up) {
		m_player->CTransform->pos.y -= m_player->CTransform->vel.y;
		m_player->CShape->shape.setPosition(m_player->CTransform->pos.x, m_player->CTransform->pos.y);
	}
	if (m_player->CInput->down) {
		m_player->CTransform->pos.y += m_player->CTransform->vel.y;
		m_player->CShape->shape.setPosition(m_player->CTransform->pos.x, m_player->CTransform->pos.y);
	}
	if (m_player->CInput->left) {
		m_player->CTransform->pos.x -= m_player->CTransform->vel.x;
		m_player->CShape->shape.setPosition(m_player->CTransform->pos.x, m_player->CTransform->pos.y);
	}
	if (m_player->CInput->right) {
		m_player->CTransform->pos.x += m_player->CTransform->vel.x;
		m_player->CShape->shape.setPosition(m_player->CTransform->pos.x, m_player->CTransform->pos.y);
	}
}

void GameMode::sCollision() {
	// COLLISSION OF ENEMY TO WALLS
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
		m_player->CShape->shape.setPosition(1880, m_player->CShape->shape.getPosition().x);
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
		for (auto e : m_entityManager.getEntities("enemy")) {
			Vec2 p1 = e->CTransform->pos;
			Vec2 p2 = b->CTransform->pos;
			float radSQ = (e->CShape->shape.getRadius() + b->CShape->shape.getRadius()) * (e->CShape->shape.getRadius() + b->CShape->shape.getRadius());
			float distSQ = (p1.x - p2.x) * (p1.x - p2.x) + (p1.y - p2.y) * (p1.y - p2.y);
				if (distSQ <= radSQ) {
					m_score += e->CShape->shape.getPointCount() + e->CShape->shape.getRadius();
					m_scoreText.setString("SCORE: " + std::to_string(m_score));
					e->destroy();
					smallEnemySpawner(e);
					b->destroy();
			}
		}
	}

	// COLLISSION OF PLAYER TO ENEMY!!!
	for (auto e : m_entityManager.getEntities("enemy")) {
		Vec2 p1 = m_player->CTransform->pos;
		Vec2 p2 = e->CTransform->pos;
		float radSQ = (e->CShape->shape.getRadius() + m_player->CShape->shape.getRadius()) * (e->CShape->shape.getRadius() + m_player->CShape->shape.getRadius());
		float distSQ = (p1.x - p2.x) * (p1.x - p2.x) + (p1.y - p2.y) * (p1.y - p2.y);
		if (distSQ <= radSQ) {
			m_player->destroy();
			e->destroy();
			m_paused = true;
		}
	}
}
	

void GameMode::sInput() {
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
				case sf::Keyboard::P: m_paused = !m_paused; break;
				}
			}
			if (event.type == sf::Event::KeyReleased) {
				switch (event.key.code)
				{
				case sf::Keyboard::W: m_player->CInput->up = false; break;
				case sf::Keyboard::A: m_player->CInput->left = false; break;;
				case sf::Keyboard::S: m_player->CInput->down = false; break;
				case sf::Keyboard::D: m_player->CInput->right = false; break;
				}
			}
			if (event.type == sf::Event::MouseButtonPressed) {
				if (sf::Event::MouseLeft) {
					Vec2 mousePos = Vec2(event.mouseButton.x, event.mouseButton.y);
					sPawnBullet(mousePos);
				}
			}
		}

	}
}

void GameMode::sSpawnEnemy() {
	int speed = EnemyConfig.SMIN + (rand() % (EnemyConfig.SMAX - EnemyConfig.SMIN + 1));
	int radius = EnemyConfig.RMIN + (rand() % (EnemyConfig.RMAX - EnemyConfig.RMIN + 1));
	int Vertices = EnemyConfig.VMIN + (rand() % (EnemyConfig.VMAX - EnemyConfig.VMIN + 1));
	int fillcolorR = EnemyConfig.FRMIN + (rand() % (EnemyConfig.FRMAX - EnemyConfig.FRMIN + 1));
	int fillcolorG = EnemyConfig.FGMIN + (rand() % (EnemyConfig.FGMAX - EnemyConfig.FGMIN + 1));
	int fillcolorB = EnemyConfig.FBMIN + (rand() % (EnemyConfig.FBMAX - EnemyConfig.FBMIN + 1));
	int outcolorR = EnemyConfig.ORMIN + (rand() % (EnemyConfig.ORMAX - EnemyConfig.ORMIN + 1));
	int outcolorG = EnemyConfig.OGMIN + (rand() % (EnemyConfig.OGMAX - EnemyConfig.OGMIN + 1));
	int outcolorB = EnemyConfig.OBMIN + (rand() % (EnemyConfig.OBMAX - EnemyConfig.OBMIN + 1));
	int posx = rand() % 1920;
	int posy = rand() % 1080;
	if (posx >= m_player->CTransform->pos.x - m_player->CShape->shape.getRadius() && posx <= m_player->CTransform->pos.x + m_player->CShape->shape.getRadius()
		&& posy >= m_player->CTransform->pos.y - m_player->CShape->shape.getRadius() && posy <= m_player->CTransform->pos.y + m_player->CShape->shape.getRadius()) {
		return;
	}
	std::shared_ptr<Entity> e = m_entityManager.addEntity("enemy");
	e->CTransform = std::make_shared<Component::CTransform>(Vec2(posx, posy), Vec2(speed, speed));
	e->CShape = std::make_shared<Component::CShape>(Vertices, radius, sf::Color(fillcolorR, fillcolorG, fillcolorB), 
				sf::Color(outcolorR, outcolorG, outcolorB), e->CTransform->pos, EnemyConfig.OT);
	e->CCollision = std::make_shared<Component::CCollision>(radius);
	
}

void GameMode::sEnemySpawner() {
	if (currentFrame - lastEnemySpawnFrame >= 90) {
		sSpawnEnemy();
		lastEnemySpawnFrame = currentFrame;
	}
}

void GameMode::smallEnemySpawner(std::shared_ptr<Entity> parant) {
	int i = 0;
	while (i < 3) {
		auto se = m_entityManager.addEntity("small enemy");
		se->CTransform = std::make_shared<Component::CTransform>(parant->CTransform->pos, parant->CTransform->vel);
		se->CShape = std::make_shared<Component::CShape>(parant->CShape->shape.getPointCount(), parant->CShape->shape.getRadius() / 2, sf::Color::Yellow, sf::Color::Blue,parant->CTransform->pos ,2);
		se->CLifespan = std::make_shared<Component::CLifespan>(60);
		i++;
	}
}

void GameMode::sPawnBullet(Vec2 mousePos) {
	std::shared_ptr<Entity> b = m_entityManager.addEntity("bullet");
	Vec2 dir = Vec2(mousePos.x - m_player->CTransform->pos.x, mousePos.y - m_player->CTransform->pos.y).normalize();
	Vec2 velocity = Vec2(BulletConfig.S * dir.x, BulletConfig.S * dir.y);
	b->CTransform = std::make_shared<Component::CTransform>(m_player->CTransform->pos, velocity);
	b->CShape = std::make_shared<Component::CShape>(BulletConfig.V, BulletConfig.R, sf::Color(BulletConfig.FR, BulletConfig.FG, BulletConfig.FB), 
					sf::Color(BulletConfig.OB, BulletConfig.OG, BulletConfig.OB), b->CTransform->pos, 2);
	b->CCollision = std::make_shared<Component::CCollision>(BulletConfig.R);
	b->CLifespan = std::make_shared<Component::CLifespan>(BulletConfig.l);
}

void GameMode::sLifespan() {
	for (auto b : m_entityManager.getEntities()) {
		if (b->getTag() == "bullet" || b->getTag() == "small enemy") {
			if (b->CLifespan->remaining == 0) {
				b->destroy();
			}
			b->CLifespan->remaining--;
			//std::cout << "Lifespan for bullet ID: " << b->getID() << " remaining is: ";
		}
		
	}
}

bool GameMode::sPause() {
	return m_paused;
}
