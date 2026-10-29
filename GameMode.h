#pragma once
#include <SFML/Graphics.hpp>
#include "EntityManager.h"
//#include "Systems.h"
#include "LevelSpecs.h"

struct windowConfig {
	int width;
	int height;
	int framerateLimit;
};

struct fontConfig {
	std::string fontPath;
	int fr, fg, fb;
};

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
	int ORMAX, OGMAX, OBMAX;
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

enum class GameState {
	Running,
	Dead,
	Paused,
	Victory
};

class GameMode
{
	EntityManager m_entityManager;
	void init(std::string path);
	bool m_running;
	sf::Font m_font;
	sf::RenderWindow m_window;
	Vec2 mousePos;
	int m_score;
	sf::Text m_scoreText, m_healthText, m_bossHealthText, m_UIText;
	bool bossActive;
	int bossHealth, m_playerHealth;
	int SpecialAttackCount;
	bool m_paused;
	windowConfig WindowConfig;
	fontConfig FontConfig;
	playerConfig PlayerConfig;
	enemyConfig EnemyConfig;
	bulletConfig BulletConfig;
	LevelSpecs::Level currentLevel;
	std::shared_ptr<Entity> m_player;
	size_t currentFrame, lastEnemySpawnFrame;
	GameState m_gameState;

public:
	void sMovement(float dt);
	void sRender(sf::RenderWindow& window, float dt);
	void sEnemySpawner(float dt);
	void sCollision(float dt);
	void sLifespan(float dt);
	void sSpawnEnemy(float dt);
	bool sPause();
	void sInput(float dt);
	void sPawnBullet(Vec2 mousePos, float dt);
	void sSpawnSpecialAttack(Vec2 mousePos, float dt);
	void smallEnemySpawner(std::shared_ptr<Entity> parent, float dt);
	void sUI();
	void run(std::string path);
};

