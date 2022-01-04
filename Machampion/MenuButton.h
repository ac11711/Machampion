#pragma once
#include "GameObjectFactory.h"
#include "PlatformerObject.h"
#include "Cursor.h"

//A class that allows us to create and define menu buttons
class MenuButton : public PlatformerObject {
public:

	//Constructor/Destructor
	
	MenuButton();
	virtual ~MenuButton() {}

	//Management functions

	virtual void load(std::unique_ptr<LoaderParams> const& pParams);
	virtual void draw();
	virtual void update();
	virtual void clean();

	//Setters

	void setCallbackID(int callbackID) { m_callbackID = callbackID; }
	void setCallback(void(*callback)()) { m_callback = callback; }

	//Getters

	int getCallbackID() { return m_callbackID; }

private:

	//Button states
	enum button_state {
		MOUSE_OUT = 0,
		MOUSE_OVER = 1,
		CLICKED = 2
	};

	bool m_bReleased;
	std::string m_lastEvent;

	int m_callbackID;
	void (*m_callback)();
};

//A class that creates a menu button
class MenuButtonCreator : public BaseCreator {
	GameObject* createGameObject() const {
		return new MenuButton();
	}
};