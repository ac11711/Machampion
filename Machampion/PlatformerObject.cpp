#include "PlatformerObject.h"
#include "TextureManager.h"
#include "Game.h"
#include "TileLayer.h"
#include "Random.h"

//Constructor that initializes object
PlatformerObject::PlatformerObject() : GameObject(),
	m_bShooting(false),
	m_shootingTime(0),
	m_shootSpeed(0),
	m_bulletCounter(0),
	m_bulletVelocity(0),
	m_dyingTime(0),
	m_dyingCounter(0),
	m_jumpHeight(0),
	m_bPlayedDeathSound(false),
	m_bMoveRight(false),
	m_bMoveLeft(false),
	m_bMoveUp(false),
	m_bMoveDown(false),
	m_bHitting(false),
	m_bRunning(false),
	m_bSlowed(false),
	m_slowedTime(0),
	m_slowedCounter(0),
	m_bFalling(false),
	m_bJumping(false),
	m_bCanHit(false),
	m_bCanJump(false),
	m_bFlipped(false),
	m_bShiny(false),
	m_bBouncing(false),
	m_hittingTime(0),
	m_hitCounter(0),
	m_recoveryCounter(0),
	m_hitSpeed(0),
	m_bounceTime(0),
	m_bounceCounter(0),
	m_bThrusting(false),
	m_thrustTime(0),
	m_thrustCounter(0),
	m_lastSafePos(0, 0)
{}

//Load game object
void PlatformerObject::load(std::unique_ptr<LoaderParams> const& pParams) {
	//Define position of game object from loader params
	m_position = Vector2D(pParams->getX(), pParams->getY());

	//Define drawing variables of game object from loader params
	m_width = pParams->getWidth();
	m_height = pParams->getHeight();
	m_textureID = pParams->getTextureID();
	m_numFrames = pParams->getNumFrames();

	//If 10% chance is met set shiny
	if (Probability(7)) {
		m_bShiny = true;
	}
}

//Draw game object to screen
void PlatformerObject::draw() {
	//Draw game object using texture manager
	TextureManager::Instance()->drawFrame(m_textureID, (Uint32)m_position.getX(), (Uint32)m_position.getY(), m_width, m_height, m_currentRow, m_currentFrame, TheGame::Instance()->getRenderer(), m_angle, m_alpha);
}

//Update game object
void PlatformerObject::update() {
	//Apply velocity to game object's position
	m_position += m_velocity;

	//Animate the object
	m_currentFrame = int(((SDL_GetTicks() / (1000 / 3)) % m_numFrames));
}

//Thrust game object
void PlatformerObject::thrust(std::string fling, int x, int y) {
	//If fling is right
	if (m_fling == std::string("right")) {
		//If thrust count is less than half
		if (m_thrustCounter < m_thrustTime / 2) {
			m_velocity.m_x = x;
			m_velocity.m_y = -y;
		}
		//Thrust count is more than half
		else {
			m_velocity.m_y = y;
		}
	}
	//If fling is left
	else if (m_fling == std::string("left")) {
		//If thrust count is less than half
		if (m_thrustCounter < m_thrustTime / 2) {
			m_velocity.m_x = -x;
			m_velocity.m_y = -y;
		}
		//Thrust count is more than half
		else {
			m_velocity.m_y = y;
		}
	}
}

//Check if game object has collided with a tile
bool PlatformerObject::checkCollideTile(Vector2D newPos) {
	//If (new y position + height) of game object is >= height of game - 32 (tile height) : 
	if (newPos.m_y + m_height >= TheGame::Instance()->getGameHeight() - 32) {
		//No collision with tile
		return false;
	}
	//New Y pos + height is <= height of game - 32
	else {
		//For each layer in collision layers vector
		for (std::vector<TileLayer*>::iterator it = m_pCollisionLayers->begin(); it != m_pCollisionLayers->end(); ++it) {
			//Create tile layer pointer and set it to current tile layer
			TileLayer* pTileLayer = (*it);

			//Create a 2D tile vector to hold tiles in current tile layer
			std::vector<std::vector<int>> tiles = pTileLayer->getTileIDs();

			//Create a vector 2D to hold current position in tile layer
			Vector2D layerPos = pTileLayer->getPosition();

			//Initialize variables for tile
			int x, y, tileColumn, tileRow, tileID = 0;

			//Set x and y to current tile position
			x = layerPos.getX() / pTileLayer->getTileSize();
			y = layerPos.getY() / pTileLayer->getTileSize();

			Vector2D startPos = newPos;
			startPos.m_x += 15;
			startPos.m_y += 20;
			Vector2D endPos(newPos.m_x + (m_width - 15), newPos.m_y + (m_height - 4));

			for (int i = startPos.m_x; i < endPos.m_x; i++) {
				for (int j = startPos.m_y; j < endPos.m_y; j++) {
					tileColumn = i / pTileLayer->getTileSize();
					tileRow = j / pTileLayer->getTileSize();

					tileID = tiles[tileRow + y][tileColumn + x];

					if (tileID != 0) {
						//Collision with tile
						return true;
					}
				}
			}
		}

		//No collision with tile
		return false;
	}
}

//Check if game object has collided with a tile
bool PlatformerObject::checkProjectileCollideTile(Vector2D newPos) {
	//If (new y position + height) of game object is >= height of game - 32 (tile height) : 
	if (newPos.m_y + m_height >= TheGame::Instance()->getGameHeight() - 32) {
		//No collision with tile
		return false;
	}
	//New Y pos + height is <= height of game - 32
	else {
		//For each layer in collision layers vector
		for (std::vector<TileLayer*>::iterator it = m_pCollisionLayers->begin(); it != m_pCollisionLayers->end(); ++it) {
			//Create tile layer pointer and set it to current tile layer
			TileLayer* pTileLayer = (*it);

			//Create a 2D tile vector to hold tiles in current tile layer
			std::vector<std::vector<int>> tiles = pTileLayer->getTileIDs();

			//Create a vector 2D to hold current position in tile layer
			Vector2D layerPos = pTileLayer->getPosition();

			//Initialize variables for tile
			int x, y, tileColumn, tileRow, tileID = 0;

			//Set x and y to current tile position
			x = layerPos.getX() / pTileLayer->getTileSize();
			y = layerPos.getY() / pTileLayer->getTileSize();

			Vector2D startPos = newPos;
			startPos.m_x += 12;
			startPos.m_y += 10;
			Vector2D endPos(newPos.m_x + (m_width - 12), newPos.m_y + (m_height - 10));

			for (int i = startPos.m_x; i < endPos.m_x; i++) {
				for (int j = startPos.m_y; j < endPos.m_y; j++) {
					tileColumn = i / pTileLayer->getTileSize();
					tileRow = j / pTileLayer->getTileSize();

					tileID = tiles[tileRow + y][tileColumn + x];

					if (tileID != 0) {
						//Collision with tile
						return true;
					}
				}
			}
		}

		//No collision with tile
		return false;
	}
}

//Check if game object has collided with a fluid tile
bool PlatformerObject::checkFluidTile(Vector2D newPos) {
	//For each layer in collision layers vector
	for (std::vector<TileLayer*>::iterator it = m_pFluidLayers->begin(); it != m_pFluidLayers->end(); ++it) {
		//Create tile layer pointer and set it to current tile layer
		TileLayer* pTileLayer = (*it);

		//Create a 2D tile vector to hold tiles in current tile layer
		std::vector<std::vector<int>> tiles = pTileLayer->getTileIDs();

		//Create a vector 2D to hold current position in tile layer
		Vector2D layerPos = pTileLayer->getPosition();

		//Initialize variables for tile
		int x, y, tileColumn, tileRow, tileID = 0;

		//Set x and y to current tile position
		x = layerPos.getX() / pTileLayer->getTileSize();
		y = layerPos.getY() / pTileLayer->getTileSize();

		Vector2D startPos = newPos;
		startPos.m_x += 15;
		startPos.m_y += 20;
		Vector2D endPos(newPos.m_x + (m_width - 15), newPos.m_y + (m_height - 9));

		for (int i = startPos.m_x; i < endPos.m_x; i++) {
			for (int j = startPos.m_y; j < endPos.m_y; j++) {
				tileColumn = i / pTileLayer->getTileSize();
				tileRow = j / pTileLayer->getTileSize();

				tileID = tiles[tileRow + y][tileColumn + x];

				if (tileID != 0) {
					//Collision with tile
					return true;
				}
			}
		}
	}

	//No collision with tile
	return false;
}

//Check if game object has collided with a hazard tile
bool PlatformerObject::checkHazardTile(Vector2D newPos) {
	//For each layer in collision layers vector
	for (std::vector<TileLayer*>::iterator it = m_pHazardLayers->begin(); it != m_pHazardLayers->end(); ++it) {
		//Create tile layer pointer and set it to current tile layer
		TileLayer* pTileLayer = (*it);

		//Create a 2D tile vector to hold tiles in current tile layer
		std::vector<std::vector<int>> tiles = pTileLayer->getTileIDs();

		//Create a vector 2D to hold current position in tile layer
		Vector2D layerPos = pTileLayer->getPosition();

		//Initialize variables for tile
		int x, y, tileColumn, tileRow, tileID = 0;

		//Set x and y to current tile position
		x = layerPos.getX() / pTileLayer->getTileSize();
		y = layerPos.getY() / pTileLayer->getTileSize();

		Vector2D startPos = newPos;
		startPos.m_x += 15;
		startPos.m_y += 20;
		Vector2D endPos(newPos.m_x + (m_width - 15), newPos.m_y + (m_height - 4));

		for (int i = startPos.m_x; i < endPos.m_x; i++) {
			for (int j = startPos.m_y; j < endPos.m_y; j++) {
				tileColumn = i / pTileLayer->getTileSize();
				tileRow = j / pTileLayer->getTileSize();

				tileID = tiles[tileRow + y][tileColumn + x];

				if (tileID != 0) {
					//Collision with tile
					return true;
				}
			}
		}
	}

	//No collision with tile
	return false;
}