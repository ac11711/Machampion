#pragma once
#include "Enemy.h"
#include "TextureManager.h"
#include "SoundManager.h"
#include "Camera.h"
#include "Random.h"
#include "Level.h"
#include "Game.h"

/*
	_____________________
	---Alternate Forms---
	_____________________

	---Tier 1---
	
	- Spearow

	---Tier 2---

	- Pidgeotto

	---Tier 3---

	- Pidgeot
	- Fearow
	_____________________
*/

class Pidgey : public Enemy {
public:

	//Constructor
	Pidgey() :
		m_bPidgey(false),
		m_bSpearow(false),
		m_bPidgeotto(false),
		m_bPidgeot(false),
		m_bFearow(false),
		m_bXColliding(false),
		m_bYColliding(false),
		m_window(3)
	{
		//Set variables
		m_dyingTime = 100;
		m_thrustTime = 20;
	}

	//Destructor
	~Pidgey() {}

	//Management functions

	//Load the pidgey
	virtual void load(std::unique_ptr<LoaderParams> const& pParams) {
		//Load using base class
		PlatformerObject::load(std::move(pParams));

		//If pidgey
		if (m_textureID == "pidgey") {
			//If spearow; 40% chance met
			if (Probability(40)) {
				//Set spearow values
				m_bSpearow = true;
				m_textureID = "spearow";
				m_width = 33;
				m_height = 39;
				m_numFrames = 61;
			}
			//Pidgey
			else {
				//Set pidgey values
				m_bPidgey = true;
			}
		}
		//Pidgeotto
		else if (m_textureID == "pidgeotto") {
			//Set pidgeotto values
			m_bPidgeotto = true;
			m_health = 1;
			m_window = 10;
		}
		//Pidgeot
		else {
			//If fearow; 40% change met
			if (Probability(40)) {
				//Set fearow values
				m_bFearow = true;
				m_textureID = "fearow";
				m_width = 82;
				m_height = 105;
				m_numFrames = 21;
			}
			//Pidgeot
			else {
				//Set pidgeot values
				m_bPidgeot = true;
			}

			//Set shared values
			m_health = 2;
			m_window = 50;
			m_bCanHit = true;
		}

		//If shiny
		if (m_bShiny) {
			//Set shiny values
			m_textureID.append("_s");
			m_health += 1;
		}
	}

	//Draw the pidgey
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

	//Update the pidgey
	virtual void update() {
		//If not dying
		if (!m_bDying) {
			//If enemy is thrusting
			if (m_bThrusting) {
				//Enemy is not hitting
				m_bHitting = false;

				//If thrust counter is met
				if (m_thrustCounter == m_thrustTime) {
					//Enemy is not thrusting
					m_bThrusting = false;
					//Reset thrust counter
					m_thrustCounter = 0;
				}
				//Thrust counter not met
				else {
					//Thrust enemy
					thrust(m_fling, 3, 3);

					//Increment thrust count
					m_thrustCounter++;
				}
			}
			//Enemy is not thrusting
			else {
				// ------------------
				// Pidgey || Spearow
				// ------------------
				if(m_bPidgey || m_bSpearow) {
					//If to the right of player and window
					if (m_bMoveLeft || m_position.m_x + (m_width / 2) > m_pPlayer->getPosition().m_x + (m_pPlayer->getWidth() / 2) + m_window && !m_bMoveRight) {
						//Set X velocity
						m_velocity.m_x = -2;

						//If enemy is close to player
						if (m_position.m_x - (m_pPlayer->getPosition().m_x + m_pPlayer->getWidth()) <= 60 && m_position.m_x - (m_pPlayer->getPosition().m_x + m_pPlayer->getWidth()) > 0) {
							//Set to hitting
							m_bHitting = true;
						}
						//Enemy is not close to player
						else {
							//Not hitting
							m_bHitting = false;
						}
					}
					//If to the left of player and window
					else if (m_bMoveRight || m_position.m_x + (m_width / 2) < m_pPlayer->getPosition().m_x + (m_pPlayer->getWidth() / 2) - m_window && !m_bMoveLeft) {
						//Set X velocity
						m_velocity.m_x = 2;

						//If X position is close to player
						if ((m_position.m_x + m_width) - m_pPlayer->getPosition().m_x >= -60 && (m_position.m_x + m_width) - m_pPlayer->getPosition().m_x < 0) {
							//Set to hitting
							m_bHitting = true;
						}
						//Enemy is not close to player
						else {
							//Not hitting
							m_bHitting = false;
						}
					}
					//To the center of player and window
					else {
						//Stop X velocity
						m_velocity.m_x = 0;
					}

					//If above player and window
					if (m_bMoveDown || m_position.m_y + 2 < m_pPlayer->getPosition().m_y - m_window && !m_bMoveUp) {
						//Set Y velocity
						m_velocity.m_y = 2;
					}
					//If below player and window
					else if (m_bMoveUp || m_position.m_y > m_pPlayer->getPosition().m_y + m_window && !m_bMoveDown) {
						//Set Y velocity
						m_velocity.m_y = -2;
					}
					//To the center of player and window
					else {
						//Stop Y velocity
						m_velocity.m_y = 0;
					}
				}
				// ----------
				// Pidgeotto
				// ----------
				else if (m_bPidgeotto) {
					//If to the right of player and window
					if (m_bMoveLeft || m_position.m_x + (m_width / 2) > m_pPlayer->getPosition().m_x + (m_pPlayer->getWidth() / 2) + m_window && !m_bMoveRight) {
						//Set X velocity
						m_velocity.m_x = -3;
					}
					//If to the left of player and window
					else if (m_bMoveRight || m_position.m_x + (m_width / 2) < m_pPlayer->getPosition().m_x + (m_pPlayer->getWidth() / 2) - m_window && !m_bMoveLeft) {
						//Set X velocity
						m_velocity.m_x = 3;
					}
					//To the center of player and window
					else {
						//Stop X velocity
						m_velocity.m_x = 0;
					}

					//If above player
					if (m_bMoveDown || m_position.m_y + (m_height / 2) + 12 < m_pPlayer->getPosition().m_y + (m_pPlayer->getHeight() / 2) - m_window && !m_bMoveUp) {
						//Set Y velocity
						m_velocity.m_y = 3;
					}
					//If below player
					else if (m_bMoveUp || m_position.m_y + (m_height / 2) > m_pPlayer->getPosition().m_y + (m_pPlayer->getHeight() / 2) + m_window && !m_bMoveDown) {
						//Set Y velocity
						m_velocity.m_y = -3;
					}
					//To the center of player and window
					else {
						//Stop Y velocity
						m_velocity.m_y = 0;
					}
				}
				// ------------------
				// Pidgeot || Fearow 
				// ------------------
				else {
					//If to the right of player and window
					if (m_bMoveLeft || m_position.m_x + (m_width / 2) > m_pPlayer->getPosition().m_x + (m_pPlayer->getWidth() / 2) + m_window && !m_bMoveRight) {
						//Set X velocity
						m_velocity.m_x = -4;
						
						//If enemy is close to player
						if (m_position.m_x - (m_pPlayer->getPosition().m_x + m_pPlayer->getWidth()) <= 60 && m_position.m_x - (m_pPlayer->getPosition().m_x + m_pPlayer->getWidth()) > 0) {
							//Set to hitting
							m_bHitting = true;
						}
						//Enemy is not close to player
						else if (m_position.m_x - (m_pPlayer->getPosition().m_x + m_pPlayer->getWidth()) <= 80 && m_position.m_x - (m_pPlayer->getPosition().m_x + m_pPlayer->getWidth()) > 0) {
							//Not hitting
							m_bHitting = false;
						}
					}
					//If to the left of player and window
					else if (m_bMoveRight || m_position.m_x + (m_width / 2) < m_pPlayer->getPosition().m_x + (m_pPlayer->getWidth() / 2) - m_window && !m_bMoveLeft) {
						//Set X velocity
						m_velocity.m_x = 4;

						//If X position is close to player
						if ((m_position.m_x + m_width) - m_pPlayer->getPosition().m_x >= -60 && (m_position.m_x + m_width) - m_pPlayer->getPosition().m_x < 0) {
							//Set to hitting
							m_bHitting = true;
						}
						//Enemy is not close to player
						else if ((m_position.m_x + m_width) - m_pPlayer->getPosition().m_x >= -80 && (m_position.m_x + m_width) - m_pPlayer->getPosition().m_x < 0) {
							//Not hitting
							m_bHitting = false;
						}
					}
					//To the center of player and window
					else {
						//Stop X velocity
						m_velocity.m_x = 0;
						
						//Enemy is hitting
						m_bHitting = true;
					}

					//If can hit and close to player
					if (m_bCanHit && m_bHitting) {
						//If above player; Drop down on player
						if (m_bMoveDown || m_position.m_y + (m_height / 2) + 12 < m_pPlayer->getPosition().m_y + (m_pPlayer->getHeight() / 2) - m_window && !m_bMoveUp) {
							//Set Y velocity
							m_velocity.m_y = 4;
						}

						//If hit 0 velocity
						if (m_velocity.m_y == 0) {
							//Cant hit anymore
							m_bCanHit = false;
						}
					}
					//If cant hit
					else if (!m_bCanHit) {
						//Decrease window
						m_window = 10;

						//If above player
						if (m_bMoveDown || m_position.m_y + (m_height / 2) + 12 < m_pPlayer->getPosition().m_y + (m_pPlayer->getHeight() / 2) - m_window && !m_bMoveUp) {
							//Set Y velocity
							m_velocity.m_y = 4;
						}
						//If below player
						else if (m_bMoveUp || m_position.m_y + (m_height / 2) > m_pPlayer->getPosition().m_y + (m_pPlayer->getHeight() / 2) + m_window && !m_bMoveDown) {
							//Set Y velocity
							m_velocity.m_y = -4;
						}
						//To the center of player and window
						else {
							//Stop Y velocity
							m_velocity.m_y = 0;
						}
					}
				}
			}

			//Handle velocity
			handleMovement(m_velocity);

			//Handle AI
			handleAI();
		}
		//If dying
		else {
			//Stop X velocity
			m_velocity.m_x = 0;

			//Set Y velocity to 5 (gravity)
			m_velocity.m_y = 5;

			//If dying counter is up
			if (m_dyingCounter == m_dyingTime) {
				//Set enemy to dead
				m_bDead = true;
			}
			m_dyingCounter++;

			//Handle velocity
			handleMovement(m_velocity);
		}

		//Animate enemy
		handleAnimation();
	}


	//Handle when pidgey collides
	virtual void collision(std::string fling) {
		//If not dying
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
				//Enemy is dying
				m_bDying = true;
			}
		}
	}

private:

	//		---Member functions----

	//Handle pidgey movement
	void handleMovement(Vector2D velocity) {
		//Declare new position probe
		Vector2D newPos = m_position;

		//Add velocity to current Xpos
		newPos.m_x = m_position.m_x + velocity.m_x;

		//If new Xpos will not collide with a tile
		if (!checkCollideTile(newPos)) {
			//No collision, update actual Xpos
			m_position.m_x = newPos.m_x;

			//Set to not colliding
			m_bXColliding = false;
		}
		//New Xpos will collide with a tile
		else {
			//Collision, stop enemy's X velocity
			m_velocity.m_x = 0;

			//Set to colliding
			m_bXColliding = true;
		}

		//Set current position after movement
		newPos = m_position;

		//Add velocity to current Y pos
		newPos.m_y += velocity.m_y;

		//Workaround for Pidgeotto Y collision when climbing from left side
		if ((m_bPidgeotto || m_bPidgeot || m_bFearow) && m_bYColliding) {
			newPos.m_y += velocity.m_y - 2;
		}

		//If new Ypos will not collide with a tile
		if (!checkCollideTile(newPos)) {
			//No collision, update actual Y pos
			m_position.m_y = newPos.m_y;

			//Set to not colliding
			m_bYColliding = false;
		}
		//New Ypos will collide with a tile
		else {
			//Collision, stop enemy's Y velocity
			m_velocity.m_y = 0;

			//Set to colliding
			m_bYColliding = true;
		}
	}

	//Handle pidgey animation
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

		//If hitting
		if (m_bHitting) {
			//If pidgey or spearow
			if (m_bPidgey || m_bSpearow) {
				//Set to attacking values
				m_currentRow = 1;
				m_numFrames = 1;
			}
			//If pidgeot
			else if (m_bPidgeot) {
				//Set to attacking values
				m_currentRow = 2;
				m_numFrames = 1;
			}
			//If fearow
			else if (m_bFearow) {
				//Set to attacking values
				m_currentRow = 2;
				m_numFrames = 1;
			}
		}
		//Not hitting
		else {
			//If pidgey
			if (m_bPidgey) {
				//Set to normal values
				m_numFrames = 41;
			}
			//If spearow
			else if (m_bSpearow) {
				//Set to normal values
				m_numFrames = 61;
			}
			//If pidgeot
			else if (m_bPidgeot) {
				//Set to normal values
				m_numFrames = 71;
			}
			//If fearow
			else if (m_bFearow) {
				//Set to normal values
				m_numFrames = 21;
			}

			//Set to normal row
			m_currentRow = 0;
		}

		//If thrusting
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
			//Set to dying values
			m_currentRow = 1;
			m_numFrames = 1;

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

	//Handle enemy AI
	void handleAI() {
		//If no X or Y collision
		if (!m_bXColliding && !m_bYColliding) {
			//Stop AI movement
			stopAI();
		}
		//X or Y collision
		else {
			//If X collision
			if (m_bXColliding) {
				//If enemy Y position is less than player Y position
				if (m_position.m_y + m_height - 1 < m_pPlayer->getPosition().m_y - 1 && !m_bMoveUp) {
					//Move down
					m_bMoveDown = true;
				}
				//Enemy Y position is more than Player Y position
				else if (!m_bMoveDown) {
					//Move up
					m_bMoveUp = true;
				}
			}

			//If Y collision and enemy is less than 75 pixels away
			if (m_bYColliding && m_position.m_x - m_pPlayer->getPosition().m_x < 75) {
				//If enemy X position is more than player X position
				if (m_position.m_x > m_pPlayer->getPosition().m_x && !m_bMoveRight) {
					//Move left
					m_bMoveLeft = true;
				}
				//Enemy X position is more than player X position
				else if (!m_bMoveLeft) {
					//Move right
					m_bMoveRight = true;
				}
			}

			//If X and Y collision
			if (m_bXColliding && m_bYColliding) {
				//If moving left
				if (m_bMoveLeft) {
					//Move right
					m_bMoveRight = true;
					//Stop left movement
					m_bMoveLeft = false;
				}
				//Moving right
				else if (m_bMoveRight) {
					//Move left
					m_bMoveLeft = true;
					//Stop right movement
					m_bMoveRight = false;
				}
				//AI not moving
				else {
					//If flipped
					if (m_bFlipped) {
						//Keep moving left
						m_bMoveLeft = true;
						m_bMoveRight = false;
					}
					//Not flipped
					else {
						//Keep moving right
						m_bMoveRight = true;
						m_bMoveLeft = false;
					}
				}
			}
		}
	}

	//Stop AI
	void stopAI() {
		//Stop movement
		m_bMoveRight = false;
		m_bMoveLeft = false;
		m_bMoveUp = false;
		m_bMoveDown = false;
	}

	//		---Member variables---

	bool m_bPidgey;
	bool m_bSpearow;
	bool m_bPidgeotto;
	bool m_bPidgeot;
	bool m_bFearow;
	bool m_bXColliding;
	bool m_bYColliding;
	int m_window;
};

//Pidgey creator
class PidgeyCreator : public BaseCreator {
	GameObject* createGameObject() const {
		return new Pidgey();
	}
};