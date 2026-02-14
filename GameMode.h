#pragma once
#include <SFML/Graphics.hpp>
#include "EntityManager.h"

struct playerConfig {
	int V;
	float R;
	int S;
	int FR; int FG; int FB;
	int OR; int OG; int OB;
	int OT;
	int A;
};

struct enemyConfig {
	int VMAX, VMIN;
	int RMAX, RMIN;
	int FRMAX, FGMAX, FBMAX;
	int FRMIN, FGMIN, FBMIN;
	int ORMAX, OGMAX,  OBMAX;
	int ORMIN, OGMIN, OBMIN;
	int OT;
	int SMAX; int SMIN;
};

struct bulletConfig {
	int V;
	float R;
	int FR; int FG; int FB;
	int OR; int OG; int OB;
	int OT;
	int S;
	int l;
};

class GameMode
{


	sf::RenderWindow m_window;
	EntityManager m_entityManager;
	std::shared_ptr<Entity> m_player;
	bool m_paused = false;
	bool m_running;
	int m_score = 0;
	void init(std::string path);
	size_t currentFrame = 0, lastEnemySpawnFrame = 0;
	playerConfig PlayerConfig;
	enemyConfig EnemyConfig;
	bulletConfig BulletConfig;
	sf::Font m_font;
	sf::Text m_scoreText;

public:
	void run(std::string path);
	void sMovement();
	void sRender();
	void sEnemySpawner();
	void sCollision();
	void sLifespan();
	void sSpawnEnemy();
	bool sPause();
	void sInput();
	void sPawnBullet(Vec2 mousePos);
	void smallEnemySpawner(std::shared_ptr<Entity> parant);
};

