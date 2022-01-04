#include "ProjectileHandler.h"
#include "Game.h"

//Initialize static instance
ProjectileHandler* ProjectileHandler::s_pInstance = 0;

//Load player projectile
void ProjectileHandler::loadPlayerProjectile(std::string textureID, int x, int y, int width, int height, int numFrames, Vector2D heading, std::vector<TileLayer*>* pCollisionLayers) {
	//Create new player projectile
	PlayerProjectile* pPlayerProjectile = new PlayerProjectile();

	//Load player projectile
	pPlayerProjectile->load(std::unique_ptr<LoaderParams>(new LoaderParams(x, y, width, height, textureID, numFrames)), heading);
	//Set collision layers
	pPlayerProjectile->setCollisionLayers(pCollisionLayers);

	//Add projectile to projectile vector
	m_playerProjectiles.push_back(pPlayerProjectile);
}

//Load enemy projectile
void ProjectileHandler::loadEnemyProjectile(std::string textureID, int x, int y, int width, int height, int numFrames, Vector2D heading, std::vector<TileLayer*>* pCollisionLayers) {
	//Create new enemy projectile
	EnemyProjectile* pEnemyProjectile = new EnemyProjectile();

	//Load enemy projectile
	pEnemyProjectile->load(std::unique_ptr<LoaderParams>(new LoaderParams(x, y, width, height, textureID, numFrames)), heading);
	//Set collision layers
	pEnemyProjectile->setCollisionLayers(pCollisionLayers);

	//Add enemy projectile to projectile vector
	m_enemyProjectiles.push_back(pEnemyProjectile);
}

//Draw projectiles
void ProjectileHandler::draw() {
	//Draw player projectiles
	for (int i = 0; i < m_playerProjectiles.size(); i++) {
		m_playerProjectiles[i]->draw();
	}

	//Draw enemy projectiles
	for (int j = 0; j < m_enemyProjectiles.size(); j++) {
		m_enemyProjectiles[j]->draw();
	}
}

//Update projectiles
void ProjectileHandler::update() {
	//For each player projectile
	for (std::vector<PlayerProjectile*>::iterator p_it = m_playerProjectiles.begin(); p_it != m_playerProjectiles.end();) {
		//If projectile is out of map or dead
		if ((*p_it)->getPosition().m_x < 0 || (*p_it)->getPosition().m_x < TheCamera::Instance()->getPosition().m_x
			|| (*p_it)->getPosition().m_x > TheCamera::Instance()->getPosition().m_x + TheGame::Instance()->getGameWidth()
			|| (*p_it)->getPosition().m_y < 0 || (*p_it)->getPosition().m_y > TheGame::Instance()->getGameHeight() + TheCamera::Instance()->getPosition().m_y
			|| (*p_it)->dead())
		{
			//Delete projectile
			delete* p_it;

			//Erase projectile from vector
			p_it = m_playerProjectiles.erase(p_it);
		}
		//Projectile is not out of map or dead
		else {
			//Update projectile
			(*p_it)->update();

			//Increment to next projectile
			++p_it;
		}
	}

	//For each enemy projectile
	for (std::vector<EnemyProjectile*>::iterator e_it = m_enemyProjectiles.begin(); e_it != m_enemyProjectiles.end();) {
		//If projectile is out of map or dead
		if ((*e_it)->getPosition().m_x < 0 || (*e_it)->getPosition().m_x > TheGame::Instance()->getGameWidth()
			|| (*e_it)->getPosition().m_y < 0 || (*e_it)->getPosition().m_y > TheGame::Instance()->getGameHeight()
			|| (*e_it)->dead())
		{
			//Delete projectile
			delete* e_it;

			//Erase projectile from vector
			e_it = m_enemyProjectiles.erase(e_it);
		}
		//Projectile is not out of map or dead
		else {
			//Update projectile
			(*e_it)->update();

			//Increment to next projectile
			++e_it;
		}
	}
}

//Clear projectiles
void ProjectileHandler::clearProjectiles() {
	//If player projectiles is not empty
	if (!m_playerProjectiles.empty()) {
		//For each player projectile
		for (std::vector<PlayerProjectile*>::iterator p_it = m_playerProjectiles.begin(); p_it != m_playerProjectiles.end(); ++p_it) {
			//Delete projectile
			delete* p_it;
			std::cout << "Deleting Projectile\n";
		}
	}
	
	//If enemy projectiles is not empty
	if (!m_enemyProjectiles.empty()) {
		//For each enemy projectile
		for (std::vector<EnemyProjectile*>::iterator e_it = m_enemyProjectiles.begin(); e_it != m_enemyProjectiles.end(); ++e_it) {
			//Delete projectile
			delete* e_it;
		}
	}

	//Clear projectiles vectors
	m_playerProjectiles.clear();
	m_enemyProjectiles.clear();
}