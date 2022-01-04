#pragma once
#include "Projectile.h"
#include <iostream>
#include <vector>

//A class that allows us to handle projectiles
class ProjectileHandler {
public:

	//Ensures only one instance
	static ProjectileHandler* Instance() {
		if (s_pInstance == 0) {
			s_pInstance = new ProjectileHandler();
		}

		return s_pInstance;
	}

	//Management functions

	void loadPlayerProjectile(std::string textureID, int x, int y, int width, int height, int numFrames, Vector2D heading, std::vector<TileLayer*>* pCollisionLayers);
	void loadEnemyProjectile(std::string textureID, int x, int y, int width, int height, int numFrames, Vector2D heading, std::vector<TileLayer*>* pCollisionLayers);
	void draw();
	void update();
	void clearProjectiles();

	//Getters

	const std::vector<PlayerProjectile*> getPlayerProjectiles() { return m_playerProjectiles; }
	const std::vector<EnemyProjectile*> getEnemyProjectiles() { return m_enemyProjectiles; }

private:

	//Constructor/Destructor

	ProjectileHandler() {}
	~ProjectileHandler() {}

	//Instance
	static ProjectileHandler* s_pInstance;

	//Projectile storage

	std::vector<PlayerProjectile*> m_playerProjectiles;
	std::vector<EnemyProjectile*> m_enemyProjectiles;
};

typedef ProjectileHandler TheProjectileHandler;