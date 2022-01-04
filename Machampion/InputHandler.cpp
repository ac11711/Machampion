#include "InputHandler.h"
#include "Game.h"
#include <iostream>

//Initialize static instance
InputHandler* InputHandler::s_pInstance = 0;

//Constructor
InputHandler::InputHandler() {
	//Initialize keystates to 0
	m_keystates = 0;

	//Set joysticks to not initialized
	m_bJoysticksInitialized = false;

	//Initialize mouse coordinates to 0
	m_mouseX = 0;
	m_mouseY = 0;

	//Initialize mouse button states to false
	for (int i = 0; i < 3; i++) {
		m_mouseButtonStates.push_back(false);
	}
}

//Destructor
InputHandler::~InputHandler() {
	//Delete pointers
	delete m_keystates;
	
	//Clear vectors
	m_joystickValues.clear();
	m_joysticks.clear();
	m_buttonStates.clear();
	m_mouseButtonStates.clear();
}

//Handle input
void InputHandler::update() {
	SDL_Event event;

	while (SDL_PollEvent(&event)) {
		switch (event.type)
		{
		case SDL_QUIT:
			TheGame::Instance()->quit();
			break;

		case SDL_KEYDOWN: 
			onKeyDown();
			break;

		case SDL_KEYUP:
			onKeyDown();
			break;

		case SDL_MOUSEBUTTONDOWN:
			onMouseButtonDown(event);
			break;

		case SDL_MOUSEBUTTONUP:
			onMouseButtonUp(event);
			break;

		case SDL_MOUSEMOTION:
			onMouseMove(event);
			break;

		case SDL_JOYBUTTONDOWN:
			onJoystickButtonDown(event);
			break;

		case SDL_JOYBUTTONUP:
			onJoystickButtonUp(event);
			break;

		case SDL_JOYAXISMOTION:
			onJoystickAxisMove(event);

		default:
			break;
		}
	}
}

//Clean class
void InputHandler::clean() {
	//if joysticks were initialized
	if (m_bJoysticksInitialized) {
		for (unsigned int i = 0; i < SDL_NumJoysticks(); i++) {
			//Close joystick
			SDL_JoystickClose(m_joysticks[i]);
		}
	}
}

//		---KEYBOARD---

//Set key to down
void InputHandler::onKeyDown() {
	m_keystates = SDL_GetKeyboardState(0);
}

//Set key to up
void InputHandler::onKeyUp() {
	m_keystates = SDL_GetKeyboardState(0);
}

//Check if key is down
bool InputHandler::isKeyDown(SDL_Scancode key) const {
	//If keystates is not empty
	if (m_keystates != 0) {
		//If keystate is down
		if (m_keystates[key] == 1) {
			return true;
		}
		//If keystate is up
		else {
			return false;
		}
	}

	return false;
}

//		---MOUSE---

//Set button state as down
void InputHandler::onMouseButtonDown(SDL_Event& event) {
	//If left button down
	if (event.button.button == SDL_BUTTON_LEFT) {
		m_mouseButtonStates[LEFT] = true;
	}

	//If middle button down
	if (event.button.button == SDL_BUTTON_MIDDLE) {
		m_mouseButtonStates[MIDDLE] = true;
	}

	//If right button down
	if (event.button.button == SDL_BUTTON_RIGHT) {
		m_mouseButtonStates[RIGHT] = true;
	}
}

//Set button state as up
void InputHandler::onMouseButtonUp(SDL_Event& event) {
	//If left button up
	if (event.button.button == SDL_BUTTON_LEFT) {
		m_mouseButtonStates[LEFT] = false;
	}

	//If middle button up
	if (event.button.button == SDL_BUTTON_MIDDLE) {
		m_mouseButtonStates[MIDDLE] = false;
	}

	//if right button up
	if (event.button.button == SDL_BUTTON_RIGHT) {
		m_mouseButtonStates[RIGHT] = false;
	}
}

//Set mouse coords to mouse motion
void InputHandler::onMouseMove(SDL_Event& event) {
	m_mouseX = event.motion.x;
	m_mouseY = event.motion.y;
}

//Check button state
bool InputHandler::getMouseButtonState(int buttonNumber) const {
	return m_mouseButtonStates[buttonNumber];
}

//Get mouse x position
float InputHandler::getMouseX() const {
	return m_mouseX;
}

//Get mouse y position
float InputHandler::getMouseY() const {
	return m_mouseY;
}

//Reset mouse button states
void InputHandler::reset() {
	m_mouseButtonStates[LEFT] = false;
	m_mouseButtonStates[MIDDLE] = false;
	m_mouseButtonStates[RIGHT] = false;
}

//		---JOYSTICK---

//Initialize joysticks
void InputHandler::initializeJoysticks() {
	//If subsystem was not initialized
	if (SDL_WasInit(SDL_INIT_JOYSTICK) == 0) {
		//Initialize subsystem
		SDL_InitSubSystem(SDL_INIT_JOYSTICK);
	}
	//Get number of joysticks, skip if none
	if (SDL_NumJoysticks() > 0) {
		//For each joystick
		for (int i = 0; i < SDL_NumJoysticks(); i++) {
			//Create a joystick
			SDL_Joystick* joy = SDL_JoystickOpen(i);

			//If joystick opened
			if(SDL_JoystickOpen(i)) {
				//Add joystick to vector
				m_joysticks.push_back(joy);

				//Add pair of values for each stick to joystick values vector
				m_joystickValues.push_back(std::make_pair(new Vector2D(0, 0), new Vector2D(0, 0)));

				//Create vector to hold buttons
				std::vector<bool> tempButtons;

				//Create vector to hold triggers
				std::vector<bool> tempTriggers;

				//Fill button vector for each button on joystick
				for (int j = 0; j < SDL_JoystickNumButtons(joy); j++) {
					//Initialize state to false (up)
					tempButtons.push_back(false);
				}

				//For each trigger
				for (int k = 0; k < 2; k++) {
					//Initialize to false
					tempTriggers.push_back(false);
				}

				//Fill button states vector with buttons
				m_buttonStates.push_back(tempButtons);

				//Fill triggers vector with triggers vector
				m_triggers.push_back(tempTriggers);
			}
			//If joystick not opened
			else {
				std::cout << "Error opening joystick " << i << " ERROR - " << SDL_GetError() << std::endl;
			}
		}

		//Enable joystick events
		SDL_JoystickEventState(SDL_ENABLE);

		//Joystick initialize success
		m_bJoysticksInitialized = true;
		std::cout << "Initialized " << m_joysticks.size() << " joystick(s)\n";
	}
	//Joysticks initialize failed
	else {
		m_bJoysticksInitialized = false;
	}
}

//Set button state as down
void InputHandler::onJoystickButtonDown(SDL_Event& event) {
	//Identify which joystick
	int whichOne = event.jaxis.which;

	m_buttonStates[whichOne][event.jbutton.button] = true;
}

//Set button state as up
void InputHandler::onJoystickButtonUp(SDL_Event& event) {
	//Identify which joystick
	int whichOne = event.jaxis.which;

	m_buttonStates[whichOne][event.jbutton.button] = false;
}

//Set joystick coords to joystick motion
void InputHandler::onJoystickAxisMove(SDL_Event& event) {
	//Identify wich joystick
	int whichOne = event.jaxis.which;

	//If left sick moved horizontal
	if (event.jaxis.axis == 0) {
		//If motion is right
		if (event.jaxis.value > m_joystickDeadZone) {
			//Set left stick x value to 1
			m_joystickValues[whichOne].first->setX(1);
		}
		//If motion is left
		else if (event.jaxis.value < -m_joystickDeadZone) {
			//Set left stick x value to -1
			m_joystickValues[whichOne].first->setX(-1);
		}
		//If no motion
		else {
			//Set left stick x value to 0
			m_joystickValues[whichOne].first->setX(0);
		}
	}

	//If left stick moved verical
	if (event.jaxis.axis == 1) {
		//If motion is up
		if (event.jaxis.value > m_joystickDeadZone) {
			//Set left stick y value to 1
			m_joystickValues[whichOne].first->setY(1);
		}
		//If motion is down
		else if (event.jaxis.value < -m_joystickDeadZone) {
			//Set left stick y value to -1
			m_joystickValues[whichOne].first->setY(-1);
		}
		//If no motion
		else {
			//Set left stick value to 0
			m_joystickValues[whichOne].first->setY(0);
		}
	}

	//If right stick moved horizontal
	if (event.jaxis.axis == 3) {
		//If motion is right
		if (event.jaxis.value > m_joystickDeadZone) {
			//Set right stick x value to 1
			m_joystickValues[whichOne].second->setX(1);
		}
		//If motion is left
		else if (event.jaxis.value < -m_joystickDeadZone) {
			//Set right stick x value to -1
			m_joystickValues[whichOne].second->setX(-1);
		}
		//If no motion
		else {
			//Set right stick x value to 0
			m_joystickValues[whichOne].second->setX(0);
		}
	}

	//If right stick moved vertical
	if (event.jaxis.axis == 4) {
		//If motion is up
		if (event.jaxis.value > m_joystickDeadZone) {
			//Set right stick y value to 1
			m_joystickValues[whichOne].second->setY(1);
		}
		//If motion is down
		else if (event.jaxis.value < -m_joystickDeadZone) {
			//Set right stick y value to -1
			m_joystickValues[whichOne].second->setY(-1);
		}
		//if no motion
		else {
			//Set right stick y value to 0
			m_joystickValues[whichOne].second->setY(0);
		}
	}

	//If left trigger is pressed
	if (event.jaxis.axis == 2) {
		//If value is more than 0
		if (event.jaxis.value > 0) {
			m_triggers[whichOne][0] = true;
		}
		//Value is 0
		else {
			m_triggers[whichOne][0] = false;
		}
	}

	//If right trigger is pressed
	if (event.jaxis.axis == 5) {
		//If value is more than 0
		if (event.jaxis.value > 0) {
			m_triggers[whichOne][1] = true;
		}
		//Value is 0
		else {
			m_triggers[whichOne][1] = false;
		}
	}
}

//Check button state
bool InputHandler::getButtonState(int joy, int buttonNumber) const {
	//If there are joysticks
	if (SDL_NumJoysticks() > 0) {
		//Return button state
		return m_buttonStates[joy][buttonNumber];
	}
	
	//Else return false
	return false;
}

//Check trigger state
bool InputHandler::getTriggerState(int joy, int trigger) const {
	//If there are joysticks
	if (SDL_NumJoysticks() > 0) {
		return m_triggers[joy][trigger];
	}

	//Else return false
	return false;
}

//Get stick x axis
int InputHandler::getAxisX(int joy, int stick) const {
	//If there are sticks
	if (m_joystickValues.size() > 0) {
		//If left stick
		if (stick == 1) {
			return m_joystickValues[joy].first->getX();
		}
		//If right stick
		else if (stick == 2) {
			return m_joystickValues[joy].second->getX();
		}
	}

	return 0;
}

//Get stick y axis
int InputHandler::getAxisY(int joy, int stick) const {
	//If there are sticks
	if (m_joystickValues.size() > 0) {
		//If left stick
		if (stick == 1) {
			return m_joystickValues[joy].first->getY();
		}
		//If right stick
		else if (stick == 2) {
			return m_joystickValues[joy].second->getY();
		}
	}

	return 0;
}

//Reset joystick button states
void InputHandler::resetButtons() {
	//For each joystick
	for (int i = 0; i < SDL_NumJoysticks(); i++) {
		//For each button
		for (int j = 0; j < SDL_JoystickNumButtons(m_joysticks[i]); j++) {
			//Set to false
			m_buttonStates[i][j] = false;
		}
	}
}