#include "Cursor.h"
#include "TextureManager.h"
#include "InputHandler.h"
#include "Game.h"

//Initialize static instance
Cursor* Cursor::s_pInstance = 0;

//Constructor
Cursor::Cursor() {
	m_bMoveUp = false;
	m_bMoveDown = false;
	m_clicked = false;
}

//Load the Cursor
void Cursor::load(std::string filename, std::string id, int x, int y, int width, int height, int numFrames) {
	//Load using texture manager
	TheTextureManager::Instance()->load(filename, id, TheGame::Instance()->getRenderer());

	//Set variables
	m_textureID = id;
	m_position.m_x = x;
	m_position.m_y = y;
	m_width = width;
	m_height = height;
	m_numFrames = numFrames;
}

//Draw the cursor
void Cursor::draw() {
	//If flipped
	if (m_bFlipped) {
		//Draw cursor flipped
		TheTextureManager::Instance()->drawFrame(m_textureID, (Uint32)m_position.m_x, (Uint32)m_position.m_y, m_width, m_height, m_currentRow, m_currentFrame, TheGame::Instance()->getRenderer(), 0, 255, SDL_FLIP_HORIZONTAL);
	}
	//Not flipped
	else {
		//Draw cursor not flipped
		TheTextureManager::Instance()->drawFrame(m_textureID, (Uint32)m_position.m_x, (Uint32)m_position.m_y, m_width, m_height, m_currentRow, m_currentFrame, TheGame::Instance()->getRenderer(), 0, 255);
	}
}

//Update the cursor
void Cursor::update() {
	//If joysticks are initialized
	if (TheInputHandler::Instance()->joysticksInitialized()) {
		//Handle input
		handleInput();

		//If moving right
		if (m_bMoveRight) {
			m_velocity.m_x = 7;
		}
		//If moving left
		else if (m_bMoveLeft) {
			m_velocity.m_x = -7;
		}
		//If not moving left or right
		else {
			m_velocity.m_x = 0;
		}

		//If moving up
		if (m_bMoveUp) {
			m_velocity.m_y = -7;
		}
		//If moving down
		else if (m_bMoveDown) {
			m_velocity.m_y = 7;
		}
		//If not moving up or down
		else {
			m_velocity.m_y = 0;
		}

		//Move cursor
		handleMovement(m_velocity);
	}

	//Animate cursor
	handleAnimation();
}

//Clean the cursor
void Cursor::clean() {
	//Clean using base class
	PlatformerObject::clean();
}

//Handle input of cursor
void Cursor::handleInput() {
	//If left stick is right
	if (TheInputHandler::Instance()->getAxisX(0, 1) > 0) {
		m_bMoveRight = true;
		m_bMoveLeft = false;
	}
	//If left stick is left
	else if (TheInputHandler::Instance()->getAxisX(0, 1) < 0) {
		m_bMoveRight = false;
		m_bMoveLeft = true;
	}
	//Left stick is not left or right
	else {
		m_bMoveRight = false;
		m_bMoveLeft = false;
	}

	//If left stick is up
	if (TheInputHandler::Instance()->getAxisY(0, 1) < 0) {
		m_bMoveUp = true;
		m_bMoveDown = false;
	}
	//If left stick is down
	else if (TheInputHandler::Instance()->getAxisY(0, 1) > 0) {
		m_bMoveUp = false;
		m_bMoveDown = true;
	}
	//Left stick is not up or down
	else {
		m_bMoveUp = false;
		m_bMoveDown = false;
	}

	//If clicked
	if (m_clicked) {
		m_clicked = false;
	}
	//If A is pressed
	if (TheInputHandler::Instance()->getButtonState(0, 0)) {
		m_clicked = true;
	}
}

//Move the cursor
void Cursor::handleMovement(Vector2D velocity) {
	//Declare new position probe
	Vector2D newPos = m_position;

	//Add velocity to Xpos
	newPos.m_x = m_position.m_x + velocity.m_x;

	//If new Xpos is not less than 0 or not more than game width
	if (!(newPos.m_x < 0 || newPos.m_x > TheGame::Instance()->getGameWidth() - m_width)) {
		//Update actual Xpos
		m_position.m_x = newPos.m_x;
	}
	//New Xpos will be outsite limits
	else {
		m_velocity.m_x = 0;
	}

	//Set new position
	newPos = m_position;

	//Add velocity to Ypos
	newPos.m_y = m_position.m_y + velocity.m_y;

	//If new Ypos is not less than 0 or not more than game height
	if (!(newPos.m_y < 0 || newPos.m_y > TheGame::Instance()->getGameHeight() - m_height)) {
		//Update actual Ypos
		m_position.m_y = newPos.m_y;
	}
	//New Ypos will be outsite limits
	else {
		m_velocity.m_y = 0;
	}
}

//Animate the cursor
void Cursor::handleAnimation() {
	//If x velocity is left
	if (m_velocity.m_x < 0) {
		//Flip cursor
		m_bFlipped = true;
	}
	//X velocity is right
	else if (m_velocity.m_x > 0) {
		m_bFlipped = false;
	}

	//Animate
	m_currentFrame = int(((SDL_GetTicks() / (1000 / 15)) % m_numFrames));
}