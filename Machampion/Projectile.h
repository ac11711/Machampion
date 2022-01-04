#pragma once
#include "PlatformerObject.h"
#include "TextureManager.h"
#include "SoundManager.h"
#include "Game.h"
#include "Camera.h"

//A class that allows us to have projectiles
class PlayerProjectile : public PlatformerObject {
public:

	//Constructor
	PlayerProjectile() : PlatformerObject() {
		m_dyingTime = 12;
	}

	//Destructor
	virtual ~PlayerProjectile() {}

	//Management functions

	//Load the projectile
	virtual void load(std::unique_ptr<LoaderParams> pParams, Vector2D heading) {
		//Load using base class
		PlatformerObject::load(std::move(pParams));

		//Set heading
		m_heading = heading;
	}

	//Draw the projectile
	virtual void draw() {
		TheTextureManager::Instance()->drawFrame(m_textureID, (Uint32)m_position.m_x - TheCamera::Instance()->getPosition().m_x, (Uint32)m_position.m_y - TheCamera::Instance()->getPosition().m_y, m_width, m_height, m_currentRow, m_currentFrame, TheGame::Instance()->getRenderer(), m_angle, m_alpha);
	}

	//Update the projectile
	virtual void update() {
		//If projectile is not dying
		if (!m_bDying) {
			//Set velocity
			m_velocity.m_x = m_heading.m_x;
			m_velocity.m_y = m_heading.m_y;

			//Update using base class
			PlatformerObject::update();

			//Handle movement
			handleMovement(m_velocity);
		}
		//If projectile is dying
		else {
			//Stop velocity
			m_velocity.m_x = 0;
			m_velocity.m_y = 0;

			//If dying counter is up
			if (m_dyingCounter == m_dyingTime) {
				//Projectile is dead
				m_bDead = true;
			}
			//Dying counter is not up
			else {
				//Increment dying counter
				m_dyingCounter++;
			}

			//Animate projectile
			m_currentFrame = int(((SDL_GetTicks() / (1000 / 3)) % m_numFrames));
		}
	}

	//Clean the projectile
	virtual void clean() {
		//Clean using base class
		PlatformerObject::clean();
	}

	//Handle when projectile collides
	virtual void collision(std::string fling) {
		//If have not played death sound
		if (!m_bPlayedDeathSound) {
			//Play death sound
			TheSoundManager::Instance()->playSound("break", 0);

			m_bPlayedDeathSound = true;
		}

		//Set object to death state
		m_currentRow = 1;
		m_currentFrame = 0;
		m_numFrames = 1;
		m_width = 32;
		m_height = 32;
		m_bDying = true;
	}

	//Getters

	//Return type
	virtual std::string type() { return "PlayerProjectile"; }

private:

	//		---Member Functions---

	//Handle projectile movement
	void handleMovement(Vector2D velocity) {
		//Create next position
		Vector2D newPos = m_position;

		//Add velocity to current Xpos
		newPos.m_x = m_position.m_x + velocity.m_x;

		//If collision with a tile
		if (checkProjectileCollideTile(newPos)) {
			//Handle projectile collision
			collision("none");
		}
	}

	//		---Member Variables---

	Vector2D m_heading;
};

//A class that allows us to have enemy projectiles
class EnemyProjectile : public PlayerProjectile {
public:

	//Constructor/Destructor

	EnemyProjectile() : PlayerProjectile() {}
	virtual ~EnemyProjectile() {}

	//Getters

	virtual std::string type() { return "EnemyProjectile"; }
};