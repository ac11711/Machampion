#pragma once
#include "Enemy.h"
#include "TextureManager.h"
#include "GameObjectFactory.h"
#include "Camera.h"
#include "TileLayer.h"
#include <iostream>

class Psyduck : public Enemy {
public:

	//Constructor
	Psyduck() : Enemy(),
		m_bounces(0)
	{
		//Set variables
		m_bMoveLeft = true;
		m_dyingTime = 300;
		m_thrustTime = 20;
	}

	//Destructor
	~Psyduck() {}

	//Management functions

	//Load the enemy
	virtual void load(std::unique_ptr<LoaderParams> const& pParams) {
		//Load using base class
		PlatformerObject::load(std::move(pParams));

		//If shiny
		if (m_bShiny) {
			//Set shiny values
			m_textureID.append("_s");
			m_health += 1;
		}
	}

	//Draw the enemy
	virtual void draw() {
		//If flipped
		if (m_bFlipped) {
			//Draw the enemy flipped
			TheTextureManager::Instance()->drawFrame(m_textureID, (Uint32)m_position.getX() - TheCamera::Instance()->getPosition().m_x, (Uint32)m_position.getY() - TheCamera::Instance()->getPosition().m_y, m_width, m_height, m_currentRow, m_currentFrame, TheGame::Instance()->getRenderer(), m_angle, m_alpha, SDL_FLIP_HORIZONTAL);
		}
		//Not flipped
		else {
			//Draw the enemy not flipped
			TheTextureManager::Instance()->drawFrame(m_textureID, (Uint32)m_position.getX() - TheCamera::Instance()->getPosition().m_x, (Uint32)m_position.getY() - TheCamera::Instance()->getPosition().m_y, m_width, m_height, m_currentRow, m_currentFrame, TheGame::Instance()->getRenderer(), m_angle, m_alpha);
		}
	}

	//Update the enemy
	virtual void update() {
		//If enemy is not dying
		if (!m_bDying) {
			//If enemy is thrusting back from hit
			if (m_bThrusting) {
				//If thrust counter is met
				if (m_thrustCounter == m_thrustTime) {
					//Enemy is not thrusting
					m_bThrusting = false;
					//Reset thrust count
					m_thrustCounter = 0;
				}
				//Thrust counter not met
				else {
					//Thrust enemy
					thrust(m_fling, 3, 4);

					//Increment thrust count
					m_thrustCounter++;
				}
			}
			//If enemy moving left
			else if (m_bMoveLeft) {
				m_velocity.m_x = -2;
			}
			//If enemy moving right
			else if (m_bMoveRight) {
				m_velocity.m_x = 2;
			}
			//Enemy not moving
			else {
				m_velocity.m_x = 0;
			}

			//If not thrusting
			if (!m_bThrusting) {
				//Set Y velocity to 5 (gravity)
				m_velocity.m_y = 5;
			}

			//Handle enemy movement
			handleMovement(m_velocity);
		}
		//Enemy is dying
		else {
			//If enemy is moving left
			if (m_bMoveLeft) {
				//If left fling
				if (m_fling == std::string("left")) {
					if (m_bounces == 0) {
						m_velocity.m_x = -8;
					}
					else if (m_bounces == 1) {
						m_velocity.m_x = -6;
						m_velocity.m_y = -1;
					}
					else if (m_bounces == 2) {
						m_velocity.m_x = -4;
						m_velocity.m_y = 5;
					}
					else {
						m_velocity.m_x = 0;
					}
				}
				//If right fling
				else if (m_fling == std::string("right")) {
					if (m_bounces == 0) {
						m_velocity.m_x = 8;
					}
					else if (m_bounces == 1) {
						m_velocity.m_x = 6;
						m_velocity.m_y = -1;
					}
					else if (m_bounces == 2) {
						m_velocity.m_x = 4;
						m_velocity.m_y = 5;
					}
					else {
						m_velocity.m_x = 0;
					}
				}
				//Fling is center
				else {
					m_velocity.m_x = 0;
					m_velocity.m_y = 5;
				}
			}
			//If enemy is moving right
			else if (m_bMoveRight) {
				//If left fling
				if (m_fling == std::string("left")) {
					if (m_bounces == 0) {
						m_velocity.m_x = -8;
					}
					else if (m_bounces == 1) {
						m_velocity.m_x = -6;
						m_velocity.m_y = -1;
					}
					else if (m_bounces == 2) {
						m_velocity.m_x = -4;
						m_velocity.m_y = 5;
					}
					else {
						m_velocity.m_x = 0;
					}
				}
				//If right fling
				else if (m_fling == std::string("right")) {
					if (m_bounces == 0) {
						m_velocity.m_x = 8;
					}
					else if (m_bounces == 1) {
						m_velocity.m_x = 6;
						m_velocity.m_y = -1;
					}
					else if (m_bounces == 2) {
						m_velocity.m_x = 4;
						m_velocity.m_y = 5;
					}
					else {
						m_velocity.m_x = 0;
					}
				}
				//Fling is center
				else {
					m_velocity.m_x = 0;
					m_velocity.m_y = 5;
				}
			}
			//Enemy is not moving
			else {
				//If left fling
				if (m_fling == std::string("left")) {
					if (m_bounces == 0) {
						m_velocity.m_x = -8;
					}
					else if (m_bounces == 1) {
						m_velocity.m_x = -6;
						m_velocity.m_y = -1;
					}
					else if (m_bounces == 2) {
						m_velocity.m_x = -4;
						m_velocity.m_y = 5;
					}
					else {
						m_velocity.m_x = 0;
					}
				}
				//If right fling
				else if (m_fling == std::string("right")) {
					if (m_bounces == 0) {
						m_velocity.m_x = 8;
					}
					else if (m_bounces == 1) {
						m_velocity.m_x = 6;
						m_velocity.m_y = -1;
					}
					else if (m_bounces == 2) {
						m_velocity.m_x = 4;
						m_velocity.m_y = 5;
					}
					else {
						m_velocity.m_x = 0;
					}
				}
				//Fling is center
				else {
					m_velocity.m_x = 0;
					m_velocity.m_y = 5;
				}
			}

			//If dying timer is up or bouces is more than 2
			if (m_dyingCounter == m_dyingTime || m_bounces > 2 ) {
				//Set enemy to dead
				m_bDead = true;
			}
			//Increment dying counter
			m_dyingCounter++;

			//Handle enemy movement
			handleMovement(m_velocity);
		}

		//Handle animation
		handleAnimation();
	}

	//Handle when enemy collision
	virtual void collision(std::string fling) {
		//If player is not dying
		if (!m_bDying && !m_bThrusting) {
			//Play death sound
			TheSoundManager::Instance()->playSound("death", 0);

			//If health
			if (m_health > 0) {
				//Take a life away
				m_health -= 1;
				//Set direction
				m_fling = fling;
				//Enemy is thrusting back from a hit
				m_bThrusting = true;
			}
			//Health is 0
			else {
				//Set current row in texture to dying row
				m_currentRow = 1;
				//Set current frame to beginning
				m_currentFrame = 0;
				//Set to amount of frames in dying row
				m_numFrames = 1;
				//Set width of dying psyduck
				m_width = 46;

				//Set direction
				m_fling = fling;

				//Set to dying
				m_bDying = true;
			}
		}
	}

private:

	//		---Member Functions---

	//Handle enemy movement
	void handleMovement(Vector2D velocity) {
		//Set current position
		Vector2D newPos = m_position;

		//Add velocity to new Xpos
		newPos.m_x = m_position.m_x + velocity.m_x;

		//If new Xpos will not collide with a tile
		if (!checkCollideTile(newPos)) {
			//No collision, update actual X pos
			m_position.m_x = newPos.m_x;
		}
		//New Xpos will collide with a tile
		else {
			//If moving left
			if (m_bMoveLeft) {
				m_bMoveLeft = false;
				m_bMoveRight = true;
			}
			//If moving right
			else if (m_bMoveRight) {
				m_bMoveLeft = true;
				m_bMoveRight = false;
			}

			//If enemy is dying
			if (m_bDying) {
				//Play bounce sound
				//TheSoundManager::Instance()->playSound("bounce", 0);

				//Increment bounces
				m_bounces++;

				//If fling is left
				if (m_fling == std::string("left")) {
					//Set fling to right
					m_fling = "right";
				}
				//Fling is right
				else {
					//Set fling to right
					m_fling = "left";
				}
			}
		}

		//Set current position
		newPos = m_position;
		//Add velocity to new Ypos
		newPos.m_y += m_velocity.m_y;

		//If new Ypos will not collide with a tile
		if (!checkCollideTile(newPos)) {
			//if Y velocity is 0
			if (m_velocity.m_y == 0) {
				//If moving left
				if (m_bMoveLeft) {
					m_bMoveLeft = false;
					m_bMoveRight = true;
				}
				//If moving right
				else if (m_bMoveRight) {
					m_bMoveLeft = true;
					m_bMoveRight = false;
				}
			}

			//Update Ypos
			m_position.m_y = newPos.m_y;
		}
		//New Ypos will collide with a tile
		else {
			//Stop Y velocity
			m_velocity.m_y = 0;

			//If enemy is dying
			if (m_bDying) {
				//Play bounce sound
				//TheSoundManager::Instance()->playSound("bounce", 0);

				//Increment bounces
				m_bounces++;

				//If fling is left
				if (m_fling == std::string("left")) {
					//Set fling to right
					m_fling = "right";
				}
				//Fling is right
				else {
					//Set fling to right
					m_fling = "left";
				}
			}
		}
	}

	//Handle enemy animation
	void handleAnimation() {
		//If X velocity is left
		if (m_velocity.m_x < 0) {
			//Flip
			m_bFlipped = true;
		}
		//If X velocity is right
		else if (m_velocity.m_x > 0) {
			//No flip
			m_bFlipped = false;
		}

		//If enemy is thrusting
		if (m_bThrusting) {
			//If alpha is off
			if (m_alpha == 255) {
				//Turn alpha on
				m_alpha = 0;
			}
			//Alpha is on
			else {
				//Turn alpha off
				m_alpha = 255;
			}
		}
		
		//If enemy is dying and not dead
		if (m_bDying && !m_bDead) {
			//Set dying frames
			m_numFrames = 1;
			m_currentRow = 1;

			//If alpha is off
			if (m_alpha == 255) {
				//Turn alpha on
				m_alpha = 0;
			}
			//Alpha is on 
			else {
				//Turn alpha off
				m_alpha = 255;
			}
		}

		//Animate
		m_currentFrame = int(((SDL_GetTicks() / (40)) % m_numFrames));
	}

	//		---Member Variables---

	int m_bounces;
};

//Psyduck creator
class PsyduckCreator : public BaseCreator {
	GameObject* createGameObject() const {
		return new Psyduck();
	}
};