#include "Camera.h"
#include "Game.h"

//Instance
Camera* Camera::s_pInstance = 0;

//Constructor
Camera::Camera() : 
	m_pTarget(0),
	m_position(0, 0)
{}

//Destructor
Camera::~Camera() {
	//Delete target pointer
	delete m_pTarget;
}

//Update the camera 
void Camera::update(Vector2D velocity) {
	//Move the camera
	m_position += velocity;

	//If camera Xpos is less than 0
	if (m_position.m_x < 0) {
		//Set Xpos to 0
		m_position.m_x = 0;
	}
}

//Get the position of the camera
const Vector2D Camera::getPosition() const {
	//If there is a target
	if (m_pTarget != 0) {
		//Set a position to (target Xpos - half of game width, 0)
		Vector2D pos(m_pTarget->m_x - (TheGame::Instance()->getGameWidth() / 2), 0);

		//If Xpos is less than 0
		if (pos.m_x < 0) {
			//Set Xpos to 0
			pos.m_x = 0;
		}

		//Return the position
		return pos;
	}

	//Return position of the camera
	return m_position;
}

