#include "ObjectLayer.h"
#include "Vector2D.h"
#include "ProjectileHandler.h"
#include "Camera.h"
#include "GameObject.h"
#include "Level.h"
#include "Game.h"

//Destructor
ObjectLayer::~ObjectLayer() {
	//For each game object in game objects vector
	for (std::vector<GameObject*>::iterator it = m_gameObjects.begin(); it != m_gameObjects.end(); ++it) {
		//Delete pointer
		delete (*it);
	}

	//Clear game objects vector
	m_gameObjects.clear();
}

//Draw game objects
void ObjectLayer::render() {
	//For each game object
	for (int i = 0; i < m_gameObjects.size(); i++) {
		//Draw using base class
		m_gameObjects[i]->draw();
	}
}

//Update game objects
void ObjectLayer::update(Level* pLevel) {
	//Check collision between player and game objects
	m_collisionManager.checkPlayerEnemyCollision(pLevel->getPlayer(), (const std::vector<GameObject*>&)m_gameObjects);
	//Check collision between projectiles and game objects
	m_collisionManager.checkProjectileObjectCollision(pLevel->getPlayer(), (const std::vector<GameObject*>&)m_gameObjects, (const std::vector<PlayerProjectile*>&)TheProjectileHandler::Instance()->getPlayerProjectiles(), (const std::vector<EnemyProjectile*>&)TheProjectileHandler::Instance()->getEnemyProjectiles());

	//If game objects vector is not empty
	if (!m_gameObjects.empty()) {
		//For each game object
		for (std::vector<GameObject*>::iterator it = m_gameObjects.begin(); it != m_gameObjects.end();) {
			//If game object Xpos is <= (camera Xpos + game width) : If game object is to left of right end of screen
			if ((*it)->getPosition().getX() <= TheCamera::Instance()->getPosition().m_x + TheGame::Instance()->getGameWidth()) {
				//Update game object
				(*it)->setUpdating(true);
				(*it)->setStartedUpdating(true);
				(*it)->update();
			}
			//Game object is to right of right end of screen
			else {
				//If game object is not player and object has not started updating already
				if ((*it)->type() != std::string("Player") && !(*it)->startedUpdating()) {
					//Dont update game object
					(*it)->setUpdating(false);
				}
				//Game object is player
				else {
					//Update game object
					(*it)->update();
				}
			}

			//If game object is dead or to left of game or above game or below bottom of screen
			if ((*it)->dead() || (*it)->getPosition().m_x < 0 || (*it)->getPosition().m_y < 0 || (*it)->getPosition().m_y > TheGame::Instance()->getGameHeight()) {
				//If object type is player
				if ((*it)->type() == pLevel->getPlayer()->type()) {
					//Output
					std::cout << "Player out of bounds, killing player\n";

					//Kill player
					pLevel->getPlayer()->collision();
				}
				//Object is not player
				else {
					//Output
					std::cout << "Deleting: " << (*it)->type() << std::endl;

					//Delete pointer
					delete* it;

					//Clear game object from vector and set new iterator
					it = m_gameObjects.erase(it);
				}
			}
			//Game object is in play
			else {
				//Go to next game object
				++it;
			}
		}
	}
}

//Set player access for game objects - Allows objects to access player
void ObjectLayer::loadPlayerAccess(GameObject* pPlayer) {
	//For each object
	for (int i = 0; i < m_gameObjects.size(); i++) {
		//Set player access
		m_gameObjects[i]->setPlayer(pPlayer);
	}
}