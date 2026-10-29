#include "LevelSpecs.h"
#include <fstream>
#include <iostream>

LevelSpecs::Level LevelSpecs::loadLevelFromFile(const std::string& filename)
{
	Level level;
	std::ifstream file(filename);

	if (!file.is_open()) {
		std::cout << "ERROR: Could not open level file: Returning with empty level!!!" << filename << std::endl;
		return level;
	}

	std::string value;
	int   rand, enemyBulletDamage, smallEnemyDamage, enemyHealth, bulletDamage, playerHealth, BossHealth, dashDamageBoss, specialAttackDamage, specialAttackDamageBoss;
	bool  fireBullet, enemyRush, canDashPlayer, hasSpecialAttackPlayer, canDashBoss, hasSpecialAttackBoss;
	float enemySpawnRate, enemyBulletSpeed, enemySpeed, enemyRushSpeed, enemyRushTimer, enemyRushCooldownTimer, enemyBulletLifespan, movementSpeed, bulletSpeed, bulletLifespan, dashSpeedPlayer, dashCooldownPlayer,
		specialAttackCooldownTimerPlayer, targetScoreForBoss, dashSpeedBoss, dashCooldownBoss, dashCooldownTimerBoss, specialAttackCooldownTimerBoss;


	while (file >> value)
	{
		if		(value == "Randomizer")							file >> rand;
		else if (value == "EnemySpawnRate")						file >> enemySpawnRate;
		else if (value == "FireBullet")							file >> fireBullet;
		else if (value == "EnemyBulletSpeed")					file >> enemyBulletSpeed;
		else if (value == "EnemyBulletDamage")					file >> enemyBulletDamage;
		else if (value == "EnemyRush")							file >> enemyRush;
		else if (value == "EnemyRushTimer")						file >> enemyRushTimer;
		else if (value == "EnemySpeed")							file >> enemySpeed;
		else if (value == "EnemyRushSpeed")						file >> enemyRushSpeed;
		else if (value == "SmallEnemyDamage")					file >> smallEnemyDamage;
		else if (value == "EnemyBulletLifespan")				file >> enemyBulletLifespan;
		else if (value == "EnemyHealth")						file >> enemyHealth;
		else if (value == "MovementSpeed")						file >> movementSpeed;
		else if (value == "BulletSpeed")						file >> bulletSpeed;
		else if (value == "BulletDamage")						file >> bulletDamage;
		else if (value == "BulletLifespan")						file >> bulletLifespan;
		else if (value == "PlayerHealth")						file >> playerHealth;
		else if (value == "CanDashPlayer")						file >> canDashPlayer;
		else if (value == "DashSpeedPlayer")					file >> dashSpeedPlayer;
		else if (value == "DashCooldownPlayer")					file >> dashCooldownPlayer;
		else if (value == "HasSpecialAttackPlayer")				file >> hasSpecialAttackPlayer;
		else if (value == "SpecialAttackCooldownTimerPlayer")	file >> specialAttackCooldownTimerPlayer;
		else if (value == "SpecialAttackDamagePlayer")			file >> specialAttackDamage;
		else if (value == "EnemyRushCooldownTimer")				file >> enemyRushCooldownTimer;
		else if (value == "TargetScoreForBoss")					file >> targetScoreForBoss;
	}

	level.randomizer						= rand;
	level.enemySpawnRate					= enemySpawnRate;
	level.fireBullet						= fireBullet;
	level.enemyBulletSpeed					= enemyBulletSpeed;
	level.enemyBulletDamage					= enemyBulletDamage;
	level.enemyRush							= enemyRush;
	level.enemyRushTimer					= enemyRushTimer;
	level.enemySpeed						= enemySpeed;
	level.enemyRushSpeed					= enemyRushSpeed;
	level.smallEnemyDamage					= smallEnemyDamage;
	level.enemyBulletLifespan				= enemyBulletLifespan;
	level.enemyHealth						= enemyHealth;
	level.movementSpeed						= movementSpeed;
	level.bulletSpeed						= bulletSpeed;
	level.bulletDamage						= bulletDamage;
	level.bulletLifespan					= bulletLifespan;
	level.playerHealth						= playerHealth;
	level.canDashPlayer						= canDashPlayer;
	level.dashSpeedPlayer					= dashSpeedPlayer;
	level.dashCooldownPlayer				= dashCooldownPlayer;
	level.hasSpecialAttackPlayer			= hasSpecialAttackPlayer;
	level.specialAttackCooldownTimerPlayer	= specialAttackCooldownTimerPlayer;
	level.specialAttackDamagePlayer			= specialAttackDamage;
	level.enemyRushCooldownTimer			= enemyRushCooldownTimer;
	level.targetScoreForBoss				= targetScoreForBoss;

	return level;
}