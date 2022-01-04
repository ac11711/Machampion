#include "Player.h"
#include "TextureManager.h"
#include "SoundManager.h"
#include "InputHandler.h"
#include "ProjectileHandler.h"
#include "TileLayer.h"
#include "Camera.h"
#include "Game.h"

//Constructor
Player::Player() : PlatformerObject(),
	m_bInvulnerable(false),
	m_invulnerableTime(100),
	m_invulnerableCounter(0),
	m_bPressedJump(false),
	m_bPressedHit(false),
	m_bPressedShoot(false)
{
	//Set jump height
	m_jumpHeight = 90;
	//Time it takes for bounce to end
	m_bounceTime = 10;
	//Time for slowed effect
	m_slowedTime = 5;

	//Player can hit or shoot
	m_bCanHit = true;
	//Set hitting time
	m_hittingTime = 15;
	//Set hit speed ; Speed between hits
	m_hitSpeed = 15;
	//Set shooting time
	m_shootingTime = 50;
	//Set shoot speed ; Speed between shots
	m_shootSpeed = 15;
	//Set bullet velocity
	m_bulletVelocity = 3;

	//Time it takes for death
	m_dyingTime = 100;
}

//Load the player
void Player::load(std::unique_ptr<LoaderParams> const& pParams) {
	//Load using base class
	PlatformerObject::load(std::move(pParams));

	//Set camera to player
	TheCamera::Instance()->setTarget(&m_position);

	//If shiny
	if (m_bShiny) {
		//Set shiny values
		m_textureID.append("_s");
		TheGame::Instance()->setPlayerLives(TheGame::Instance()->getPlayerLives() + 1);
	}
}

//Draw the player
void Player::draw() {
	//If flipped
	if (m_bFlipped) {
		//Draw player flipped
		TheTextureManager::Instance()->drawFrame(m_textureID, (Uint32)m_position.m_x - TheCamera::Instance()->getPosition().m_x, (Uint32)m_position.m_y - TheCamera::Instance()->getPosition().m_y, m_width, m_height, m_currentRow, m_currentFrame, TheGame::Instance()->getRenderer(), m_angle, m_alpha, SDL_FLIP_HORIZONTAL);
	}
	//Not flipped
	else {
		//Draw player not flipped
		TheTextureManager::Instance()->drawFrame(m_textureID, (Uint32)m_position.m_x - TheCamera::Instance()->getPosition().m_x, (Uint32)m_position.m_y - TheCamera::Instance()->getPosition().m_y, m_width, m_height, m_currentRow, m_currentFrame, TheGame::Instance()->getRenderer(), m_angle, m_alpha);
	}
}

//Update the player
void Player::update() {
	//If player is not dying
	if (!m_bDying) {
		//Input
		handleInput();

		//If left move
		if (m_bMoveLeft) {
			//If running
			if (m_bRunning) {
				//If slowed
				if (m_bSlowed) {
					m_velocity.m_x = -2;
				}
				//Not slowed
				else {
					m_velocity.m_x = -5;
				}
			}
			//If slowed
			else if (m_bSlowed) {
				m_velocity.m_x = -2;
			}
			//Not running
			else {
				m_velocity.m_x = -3;
			}
		}
		//If right move
		else if (m_bMoveRight) {
			//If running
			if (m_bRunning) {
				//If slowed
				if (m_bSlowed) {
					m_velocity.m_x = 2;
				}
				//Not slowed
				else {
					m_velocity.m_x = 5;
				}
			}
			//If slowed
			else if (m_bSlowed) {
				m_velocity.m_x = 2;
			}
			//Not running
			else {
				m_velocity.m_x = 3;
			}
		}
		//No move
		else {
			m_velocity.m_x = 0;
		}

		//If position is less than last ground height - jump height
		if (m_position.m_y < m_lastSafePos.m_y - m_jumpHeight) {
			//Player not jumping
			m_bJumping = false;
		}

		//If not jumping
		if (!m_bJumping) {
			//If slowed
			if (m_bSlowed) {
				m_velocity.m_y = 2;
			}
			//Gravity
			else {
				m_velocity.m_y = 5;
			}
		}
		//if jumping
		else {
			//If slowed
			if (m_bSlowed) {
				m_velocity.m_y = -2;
			}
			//Gravity
			else {
				m_velocity.m_y = -5;
			}
		}

		//If player is hitting
		if (m_bHitting) {
			//Hit timer is up
			if (m_hitCounter == m_hittingTime) {
				//Player is not hitting
				m_bHitting = false;

				//Reset hit counter
				m_hitCounter = 0;

				//Reset width
				m_width = 64;
			}
			//Hit timer is not up
			else {
				//Increment hit counter
				m_hitCounter++;
			}
		}
		//If player is shooting
		if (m_bShooting) {
			//Shoot timer is up
			if (m_hitCounter == m_shootingTime) {
				//Player is not shooting
				m_bShooting = false;

				//Reset hit counter
				m_hitCounter = 0;

				//Reset width
				m_width = 64;
			}
			//Shoot timer is not up
			else {
				//Increment hit counter
				m_hitCounter++;
			}
		}
		//If player pressed hit and is not hitting
		else if (m_bPressedHit && !m_bHitting && !m_bCanHit) {
			//If recovery time is met
			if (m_recoveryCounter == m_hitSpeed) {
				//Player can hit again
				m_bPressedHit = false;
				m_bCanHit = true;

				//Reset counter
				m_recoveryCounter = 0;
			}
			//Recovery time is not met
			else {
				//Increment recovery counter
				m_recoveryCounter++;
			}
		}
		//If player pressed shoot and is not hitting
		else if (m_bPressedShoot && !m_bHitting && !m_bCanHit) {
			//If recovery time is met
			if (m_recoveryCounter == m_shootSpeed) {
				//Player can hit again
				m_bPressedShoot = false;
				m_bCanHit = true;

				//Reset counter
				m_recoveryCounter = 0;
			}
			//Recovery time is not met
			else {
				m_recoveryCounter++;
			}
		}

		//If player is bouncing
		if (m_bBouncing) {
			m_velocity.m_y = -4;
			//Player can not jump
			m_bCanJump = false;

			//If bouncing timer is up
			if (m_bounceCounter == m_bounceTime) {
				//Player is not bouncing
				m_bBouncing = false;

				//Reset bounce counter
				m_bounceCounter = 0;

				//Gravity
				m_velocity.m_y = 5;
			}
			//Bouncing timer is not up
			else {
				//Increment bounce counter
				m_bounceCounter++;
			}
		}

		//Handle velocity
		handleMovement(m_velocity);
	}
	//Player is dying
	else {
		m_velocity.m_x = 0;

		//If dying counter is up
		if (m_dyingCounter == m_dyingTime) {
			//Revive
			ressurect();
		}
		//Dying counter is not up
		else {
			//Increment dying counter
			m_dyingCounter++;
		}
		
		//Gravity
		m_velocity.m_y = 5;
	}

	//Animate
	handleAnimation();
}

//Clean the player
void Player::clean() {
	//Clean using base class
	PlatformerObject::clean();
}

//Handle when player collision
void Player::collision() {
	//If player is not dying and is not invulnerable
	if (!m_bDying && !m_bInvulnerable) {
		//Set current frame to beginning
		m_currentFrame = 0;
		//Set current row in texture to dying row
		m_currentRow = 3;
		//Set to amount of frames in dying row 
		m_numFrames = 1;
		//Set width of player
		m_width = 68;
		//Set dying to true
		m_bDying = true;
	}
}

//Bring player back to life
void Player::ressurect() {
	//Subtract a life
	TheGame::Instance()->setPlayerLives(TheGame::Instance()->getPlayerLives() - 1);

	//Set player back to last safe position
	m_position = m_lastSafePos;
	//Set dying to false
	m_bDying = false;

	//Set current frame to beginning
	m_currentFrame = 0;
	//Set current row in texture to alive row
	m_currentRow = 0;
	//Set width of player
	m_width = 64;

	//Reset dying counter
	m_dyingCounter = 0;
	//Set invulnerable for a moment
	m_bInvulnerable = true;
}

//Handle input of player
void Player::handleInput() {
	//If D key is pressed and player x pos is less than right edge of map
	if ((TheInputHandler::Instance()->isKeyDown(SDL_SCANCODE_D) || TheInputHandler::Instance()->getAxisX(0, 1) > 0) && m_position.m_x < ((*m_pCollisionLayers->begin())->getMapWidth() * 32)) {
		//If left SHIFT key is pressed
		if (TheInputHandler::Instance()->isKeyDown(SDL_SCANCODE_LSHIFT) || TheInputHandler::Instance()->getTriggerState(0, 0)) {
			//Set running to true
			m_bRunning = true;
		}
		//Left SHIFT key not pressed
		else {
			//Set running to false
			m_bRunning = false;
		}

		//Set move bools
		m_bFlipped = false;
		m_bMoveRight = true;
		m_bMoveLeft = false;
	}
	//If A key is pressed and player x pos is greater than left edge of map
	else if ((TheInputHandler::Instance()->isKeyDown(SDL_SCANCODE_A) || TheInputHandler::Instance()->getAxisX(0, 1) < 0) && m_position.m_x > 32) {
		//If left SHIFT key is pressed
		if (TheInputHandler::Instance()->isKeyDown(SDL_SCANCODE_LSHIFT) || TheInputHandler::Instance()->getTriggerState(0, 0)) {
			//Set running to true
			m_bRunning = true;
		}
		//Left SHIFT key not pressed
		else {
			//Set running to false
			m_bRunning = false;
		}

		//Set move bools
		m_bFlipped = true;
		m_bMoveRight = false;
		m_bMoveLeft = true;
	}
	//D and W not pressed
	else {
		//Set move bools
		m_bMoveRight = false;
		m_bMoveLeft = false;
	}

	//If SPACE is pressed and player can jump and jump button not pressed
	if ((TheInputHandler::Instance()->isKeyDown(SDL_SCANCODE_SPACE) || TheInputHandler::Instance()->getButtonState(0, 0)) && m_bCanJump && !m_bPressedJump) {
		//Play jump sound
		//TheSoundManager::Instance()->playSound("jump", 0);

		//If jump not pressed
		if (!m_bPressedJump) {
			//Set variables
			m_bPressedJump = true;
			m_bJumping = true;
			m_bCanJump = false;
			m_lastSafePos = m_position;
		}
	}

	//If SPACE is not pressed and player can jump
	if ((!TheInputHandler::Instance()->isKeyDown(SDL_SCANCODE_SPACE) || !TheInputHandler::Instance()->getButtonState(0, 0)) && m_bCanJump) {
		//Set pressed jump to false
		m_bPressedJump = false;
	}

	//If left click is pressed and player can hit
	if ((TheInputHandler::Instance()->getMouseButtonState(LEFT) || TheInputHandler::Instance()->getTriggerState(0, 1)) && m_bCanHit) {
		//Set variables
		m_bPressedHit = true;
		m_bHitting = true;
		m_bCanHit = false;
	}

	//If left click is not pressed and player can hit
	if ((!TheInputHandler::Instance()->getMouseButtonState(LEFT) || !TheInputHandler::Instance()->getTriggerState(0, 1)) && m_bCanHit) {
		m_bPressedHit = false;
	}

	//If right click is pressed and player can hit
	if ((TheInputHandler::Instance()->getMouseButtonState(RIGHT) || TheInputHandler::Instance()->getButtonState(0, 3)) && m_bCanHit) {
		m_bPressedShoot = true;
		m_bShooting = true;
		m_bCanHit = false;
		
		//If player is not flipped
		if (!m_bFlipped) {
			//Throw projectile to right
			TheProjectileHandler::Instance()->loadPlayerProjectile("rock", m_position.m_x + m_width - 20, m_position.m_y + 12, 32, 32, 1, Vector2D(10, 0), getCollisionLayers());
		}
		//Player is flipped
		else {
			//Throw projectile to left
			TheProjectileHandler::Instance()->loadPlayerProjectile("rock", m_position.m_x - 12, m_position.m_y + 12, 32, 32, 1, Vector2D(-10, 0), getCollisionLayers());
		}
	}

	//If right click is not pressed and player can hit
	if ((!TheInputHandler::Instance()->getMouseButtonState(RIGHT) || !TheInputHandler::Instance()->getButtonState(0, 3)) && m_bCanHit) {
		m_bPressedShoot = false;
	}
}

//Handle player movement
void Player::handleMovement(Vector2D velocity) {
	//Declare new position probe
	Vector2D newPos = m_position;

	//Add velocity to current Xpos
	newPos.m_x = m_position.m_x + velocity.m_x;

	//If new Xpos will not collide with a tile
	if (!checkCollideTile(newPos)) {
		//No collision, update actual Xpos
		m_position.m_x = newPos.m_x;
	}
	//New Xpos will collide with a tile
	else {
		//Collision, stop player's X velocity
		m_velocity.m_x = 0;
	}

	//Set current position after movement
	newPos = m_position;

	//Add velocity to current Ypos
	newPos.m_y += velocity.m_y;

	//If new Ypos will not collide with a tile
	if (!checkCollideTile(newPos)) {
		//No collision, update actual Ypos
		m_position.m_y = newPos.m_y;
	}
	//New Ypos will collide with a tile
	else {
		//If bounce timer is up
		if (m_bounceCounter == m_bounceTime) {
			//Offset players Y pos to fix bug when player hits his head after bouncing
			m_position.m_y -= 1;
		}

		//Collision, stop player's Y velocity
		m_velocity.m_y = 0;

		//Disable jump
		m_bCanJump = false;

		//Set last safe position to current position since collision with map
		m_lastSafePos = m_position;

		//If X velocity is right
		if (m_velocity.m_x > 0) {
			//Move last safe Xpos left 32
			m_lastSafePos.m_x -= 32;
		}
		//If X velocity is left
		else if (m_velocity.m_x < 0) {
			//Move last safe Xpos right 32
			m_lastSafePos.m_x += 32;
		}

		//Player can now jump again
		m_bCanJump = true;

		//Player is not currently jumping
		m_bJumping = false;
	}

	//If new position will collide with a fluid tile
	if (checkFluidTile(newPos)) {
		m_bSlowed = true;
		m_bCanJump = true;
		m_jumpHeight = 2;
		m_slowedCounter = 0;
	}
	//New position will not collide with a fluid tile
	else {
		//If slowed timer is up
		if (m_slowedCounter == m_slowedTime) {
			m_bSlowed = false;
			m_jumpHeight = 90;
			m_slowedCounter = 0;
		}
		//Slowed timer is not up
		else {
			m_slowedCounter++;
		}
	}

	//If new position will collide with a hazard tile
	if (checkHazardTile(newPos)) {
		//Handle player collision
		collision();
	}

	//If player is hitting
	if (m_bHitting) {
		//If player will collide with a tile
		if (checkCollideTile(m_position)) {
			//Move to left 6 to bounce player when he hits a wall
			m_position.m_x -= 6;
		}
	}
	//If player is shooting
	else if (m_bShooting) {
		//If player will collide with a tile
		if (checkCollideTile(m_position)) {
			m_position.m_x -= 7;
		}
	}
}

//Handle player animation
void Player::handleAnimation() {
	//If player is invulnerable, flash alpha
	if (m_bInvulnerable) {
		//If invulnerability counter is met
		if (m_invulnerableCounter == m_invulnerableTime) {
			//Remove invunerability effect
			m_bInvulnerable = false;
			m_invulnerableCounter = 0;
			m_alpha = 255;
		}
		//Invulnerability counter not met
		else {
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

		//Set normal height
		m_height = 76;

		//Increment counter
		m_invulnerableCounter++;
	}

	//If the player is not dead and not dying
	if (!m_bDead && !m_bDying) {
		//If player is hitting
		if (m_bHitting) {
			//Set frame to first frame
			m_currentFrame = 0;
			//Set player to hitting row
			m_currentRow = 1;
			//Set number of frames in row
			m_numFrames = 1;
			//Set width
			m_width = 70;
		}
		//If player is shooting
		else if (m_bShooting) {
			//Set frame to first frame
			m_currentFrame = 0;
			//Set player to shooting row
			m_currentRow = 2;
			//Set number of frames in row
			m_numFrames = 1;
			//Set width
			m_width = 71;
		}
		//If y velocity is not 0 ; Player is jumping or falling
		else if (m_velocity.m_y != 0) {
			//Set player landing frame
			m_currentFrame = 29;
			//Set player landing row
			m_currentRow = 0;
			//Set number of frames in row
			m_numFrames = 1;
		}
		else {
			//If x velocity is left
			if (m_velocity.m_x < 0) {
				//Set to first frame
				m_currentFrame = 0;
				//Set player walking row
				m_currentRow = 0;
				//Set number of frames in row
				m_numFrames = 59;
				//Flip player
				m_bFlipped = true;
			}
			//If x velocity is right
			else if (m_velocity.m_x > 0) {
				//Set to first frame
				m_currentFrame = 0;
				//Set player walking row
				m_currentRow = 0;
				//Set number of frames in row
				m_numFrames = 59;
				//Do not flip player
				m_bFlipped = false;
			}
			//X velocity is 0
			else {
				//Set to first frame
				m_currentFrame = 0;
				//Set player standing row
				m_currentRow = 0;
				//Set number of frames in row
				m_numFrames = 59;
			}
		}

		//If Y velocity is 0
		if (m_velocity.m_y == 0) {
			//Animate player
			m_currentFrame = int(((SDL_GetTicks() / (40)) % m_numFrames));
		}

		////If player is running
		//if (m_bRunning) {
		//	//Make frames transition faster
		//	//m_currentFrame = int(((SDL_GetTicks() / (100)) % m_numFrames));
		//}
		////Player is not running
		//else {
		//	//Animate normally
		//	//m_currentFrame = int(((SDL_GetTicks() / (120)) % m_numFrames));
		//}
	}
}