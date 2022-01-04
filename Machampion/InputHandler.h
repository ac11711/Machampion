#pragma once
#include <vector>
#include "Vector2D.h"
#include "SDL.h"

enum mous_buttons {
	LEFT = 0,
	MIDDLE = 1,
	RIGHT = 2,
};

//A class that allows us to add controls to the game
class InputHandler {
public:

	//Ensures only one instance
	static InputHandler* Instance() {
		if (s_pInstance == 0) {
			s_pInstance = new InputHandler();
		}

		return s_pInstance;
	}

	//Management functions

	void update();
	void clean();

	//		---KEYBOARD---

	bool isKeyDown(SDL_Scancode key) const;

	//		---MOUSE---

	bool getMouseButtonState(int buttonNumber) const;
	float getMouseX() const;
	float getMouseY() const;
	void reset();

	//		---JOYSTCK---

	void initializeJoysticks();
	bool joysticksInitialized() { return m_bJoysticksInitialized; }
	bool getButtonState(int joy, int buttonNumber) const;
	bool getTriggerState(int joy, int trigger) const;
	int getAxisX(int joy, int stick) const;
	int getAxisY(int joy, int stick) const;
	void resetButtons();

private:

	//Constructor/Destructor

	InputHandler();
	~InputHandler();

	//Instance
	static InputHandler* s_pInstance;

	//		---KEYBOARD---


	//Keyboard member functions

	void onKeyDown();
	void onKeyUp();

	//Keyboard member variables

	const Uint8* m_keystates;

	//		---MOUSE---

	//Mouse member functions

	void onMouseButtonDown(SDL_Event& event);
	void onMouseButtonUp(SDL_Event& event);
	void onMouseMove(SDL_Event& event);

	//Mouse member variables

	float m_mouseX;
	float m_mouseY;
	std::vector<bool> m_mouseButtonStates;

	//		---JOYSTICK---

	//Joystick member functions

	void onJoystickButtonDown(SDL_Event& event);
	void onJoystickButtonUp(SDL_Event& event);
	void onJoystickAxisMove(SDL_Event& event);

	//Joystick member variables

	bool m_bJoysticksInitialized;
	const int m_joystickDeadZone = 10000;
	std::vector<SDL_Joystick*> m_joysticks;
	std::vector<std::pair<Vector2D*, Vector2D*>> m_joystickValues;
	std::vector<std::vector<bool>> m_triggers;
	std::vector<std::vector<bool>> m_buttonStates;
};

typedef InputHandler TheInputHandler;

//		---Joystick Button Numbers---
//Stick 1 = left analog stick
//Stick 2 = right analog stick
//POV = dpad
//Button 0 = A
//Button 1 = B
//Button 2 = X
//Button 3 = Y
//Button 4 = LB
//Button 5 = RB
//Button 6 = back
//Button 7 = start
//Button 8 = left analog stick center pushed in
//Button 9 = right analog stick center pushed in
//Axis 2 = left trigger
//Axis 5 = right trigger
//Button 13 = X silver guide button