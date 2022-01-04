#pragma once
#include "ProjectileHandler.h"
#include <iostream>
#include <vector>

class Player;
class GameObject;

//A class that allows us to check and handle collisions
class CollisionManager {
public:

	void checkPlayerEnemyCollision(Player* pPlayer, const std::vector<GameObject*>& objects);
	void checkProjectileObjectCollision(Player* pPlayer, const std::vector<GameObject*>& objects, const std::vector<PlayerProjectile*> playerProjectiles, const std::vector<EnemyProjectile*> enemyProjectiles);
};