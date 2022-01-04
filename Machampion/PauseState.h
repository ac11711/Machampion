#pragma once
#include "MenuState.h"
#include <iostream>
#include <vector>

class GameObject;

class PauseState : public MenuState {
public:

	//Destructor
	virtual ~PauseState() {}

	//Management functions

	virtual void render();
	virtual void update();

	virtual bool onEnter();
	virtual bool onExit();

	//Setters

	virtual void setCallbacks(const std::vector<Callback>& callback);

	//Getters

	virtual std::string getStateID() const { return s_pauseID; };

private:

	//		---Member functions---

	static void s_pauseToMain();
	static void s_resumePlay();

	//		---Memeber variables---

	static const std::string s_pauseID;

	std::vector<GameObject*> m_gameObjects;
};