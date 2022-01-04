#pragma once
#include "MenuState.h"
#include <iostream>
#include <vector>

class GameObject;

//A class that allows us to make the game over state
class GameOverState : public MenuState {
public:

	//Destructor
	virtual ~GameOverState() {}

	//Management functions

	virtual void render();
	virtual void update();

	virtual bool onEnter();
	virtual bool onExit();

	//Setters

	virtual void setCallbacks(const std::vector<Callback>& callbacks);

	//Getters

	virtual std::string getStateID() const { return s_gameOverID; }

private:

	//		---Memeber Functions---

	static void s_gameOverToMain();
	static void s_restartToPlay();

	//		---Member Variables---

	static const std::string s_gameOverID;

	std::vector<GameObject*> m_gameObjects;
};