#include "Collision.h"
#include "CollisionManager.h"
#include "Player.h"
#include "Enemy.h"

void CollisionManager::checkPlayerEnemyCollision(Player* pPlayer, const std::vector<GameObject*>& objects) {
	//Create the first rectangle hitbox
	SDL_Rect pRect1 = SDL_Rect();

	//Set rectangle dimensions to player dimensions
	pRect1.x = pPlayer->getPosition().getX();
	pRect1.y = pPlayer->getPosition().getY();
	pRect1.w = pPlayer->getWidth();
	pRect1.h = pPlayer->getHeight();
	
	//For each object in game objects vector
	for (int i = 0; i < objects.size(); i++) {
		//If object type is not enemy or is not a consumable object or is not updating
		if ((objects[i]->type() != std::string("Enemy") && objects[i]->type() != std::string("ConsumableObject")) || !objects[i]->updating()) {
			//Move on to next object
			continue;
		}

		//Create the second rectangle hitbox
		SDL_Rect pRect2 = SDL_Rect();

		//Set rectangle dimensions to enemy dimensions
		pRect2.x = objects[i]->getPosition().getX();
		pRect2.y = objects[i]->getPosition().getY();
		pRect2.w = objects[i]->getWidth();
		pRect2.h = objects[i]->getHeight();

		//If precise collision and player is hitting and object is enemy
		if (RectRectPrecise(pRect1, pRect2) && pPlayer->isHitting() && objects[i]->type() == std::string("Enemy")) {
			//If player is not dying and object is not dead or dying
			if (!pPlayer->dying() && !objects[i]->dead() && !objects[i]->dying()) {
				//If player is not flipped and player is to the left of enemy
				if (!pPlayer->isFlipped() && pRect1.x < pRect2.x) {
					//Handle enemy collision
					objects[i]->collision("right");
				}
				//If player is flipped and player is to the right of enemy
				else if (pPlayer->isFlipped() && pRect1.x > pRect2.x) {
					//Handle enemy collision
					objects[i]->collision("left");
				}
			}
		}

		//If collision
		if (RectRect(pRect1, pRect2)) {
			//Create fling variable
			std::string fling;

			//If object is a consumable object
			if (objects[i]->type() == std::string("ConsumableObject")) {
				//Handle consumable object collision
				objects[i]->collision();
			}

			//If enemy is not dead and not dying
			if (!objects[i]->dead() && !objects[i]->dying()) {
				//If player is above enemy - Enemy dies
				if (pRect1.y + pRect1.h - (pRect1.h / s_buffer) - 4 <= pRect2.y + (pRect2.h / s_buffer)) {
					//Bounce player
					pPlayer->setBouncing(true);

					//If player's center is to left of enemey's center
					if (((pRect1.x + pRect1.w) / 2) - 5 < (pRect2.x + pRect2.w) / 2) {
						//Set direction to right
						fling = "right";
					}
					//If player's center is to right of enemy's center
					else if ((pRect1.x + pRect1.w) / 2 > (pRect2.x + pRect2.w) / 2) {
						//Set direction to left
						fling = "left";
					}
					//Player's center is equal to enemy's center
					else {
						//Set direction to center
						fling = "center";
					}

					//Handle enemy collision
					objects[i]->collision(fling);
				}
				//If Player is hitting and not dying
				else if (pPlayer->isHitting() && !pPlayer->dying()) {
					//If player is not flipped and player is to the left of enemy
					if (!pPlayer->isFlipped() && pRect1.x < pRect2.x) {
						//Handle enemy collision
						//objects[i]->collision("right");
					}
					//If player is flipped and player is to the right of enemy
					else if (pPlayer->isFlipped() && pRect1.x > pRect2.x) {
						//Handle enemy collision
						//objects[i]->collision("left");
					}
					//Enemy collides with players back
					else {
						//Handle player collision
						pPlayer->collision();
					}
				}
				//Player is not above enemy and not hitting enemy - Player dies
				else {
					//Handle when player collides with enemy
					pPlayer->collision();
				}
			}
		}
	}
}

void CollisionManager::checkProjectileObjectCollision(Player* pPlayer, const std::vector<GameObject*>& objects, const std::vector<PlayerProjectile*>& playerProjectiles, const std::vector<EnemyProjectile*>& enemyProjectiles) {
	//Create rectangle hitbox
	SDL_Rect pRect1 = SDL_Rect();
	SDL_Rect pRect2 = SDL_Rect();

	//For each player projectile
	for (int i = 0; i < playerProjectiles.size(); i++) {
		//Set rectangle dimensions to projectile dimensions
		pRect1.x = playerProjectiles[i]->getPosition().m_x;
		pRect1.y = playerProjectiles[i]->getPosition().m_y;
		pRect1.w = playerProjectiles[i]->getWidth();
		pRect1.h = playerProjectiles[i]->getHeight();

		//For each object
		for (int j = 0; j < objects.size(); j++) {
			//If object is not an enemy and is not updating
			if (objects[j]->type() != std::string("Enemy") || !objects[j]->updating()) {
				//Move on to next object
				continue;
			}

			//Set rectangle dimensions to enemy dimensions
			pRect2.x = objects[j]->getPosition().m_x;
			pRect2.y = objects[j]->getPosition().m_y;
			pRect2.w = objects[j]->getWidth();
			pRect2.h = objects[j]->getHeight();

			//If collision
			if (RectRectPrecise(pRect1, pRect2)) {
				//If projectile is to left of object
				if (pRect1.x < pRect2.x) {
					//Handle right enemy collision
					objects[j]->collision("right");
					//Handle projectile collision
					playerProjectiles[i]->collision("none");
				}
				//If projectile is to right of object
				else {
					//Handle left enemy collision
					objects[j]->collision("left");
					//Handle projectile collision
					playerProjectiles[i]->collision("none");
				}
			}
		}
	}

	//Check enemy projectiles against player
}