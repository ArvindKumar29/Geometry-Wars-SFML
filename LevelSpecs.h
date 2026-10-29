#pragma once
#include <string>

class LevelSpecs
{
public:
	struct Level {

		//Level
		int randomizer = 1;

		//Small enemy
		float enemySpawnRate = 90; // frames

		bool  fireBullet = false;
		float enemyBulletSpeed = 5.0f;
		int   enemyBulletDamage = 10;

		bool  enemyRush = false;
		float enemyRushTimer = 2.0f;
		float enemySpeed = 2.0f;
		float enemyRushSpeed = 5.0f;
		float enemyRushCooldownTimer = 2.0f;

		int   smallEnemyDamage = 2;
		float enemyBulletLifespan = 60; // frames
		int enemyHealth = 50;

		// Player
		float movementSpeed = 5.0f;
		float bulletSpeed = 10.0f;
		int	  bulletDamage = 10;
		float bulletLifespan = 40; // frames
		int playerHealth = 0.0f;

		bool canDashPlayer = false;
		float dashSpeedPlayer = 0.0f;
		float dashCooldownPlayer = 0.0f;

		bool hasSpecialAttackPlayer = false;
		float specialAttackCooldownTimerPlayer = 0.0f;
		int specialAttackDamagePlayer = 0.0f;


		// Boss
		float targetScoreForBoss = 100;
		int BossHealth = 1000;

		bool canDashBoss = false;
		float dashSpeedBoss = 0.0f;
		float dashCooldownBoss = 0.0f;
		int dashDamageBoss = 0.0f;

		bool hasSpecialAttackBoss = false;
		float specialAttackCooldownTimerBoss = 0.0f;
		int specialAttackDamageBoss = 0.0f;

	};

	static Level loadLevelFromFile(const std::string& filename);
};