#pragma once
#include "MenuState.h"
#include "GameObject.h"
#include <vector>

//A class that allows us to make the game's main menu
class MainMenuState : public MenuState {
public:

	//Destructor
	virtual ~MainMenuState() {}

	//Management functions

	virtual void render();
	virtual void update();

	virtual bool onEnter();
	virtual bool onExit();

	//Getters

	virtual std::string getStateID() const { return s_menuID; }

private:

	//		---Member functions---

	virtual void setCallbacks(const std::vector<Callback>& callbacks);

	static void s_menuToPlay();
	static void s_exitFromMenu();

	//		---Member variables---

	static const std::string s_menuID;

	std::vector<GameObject*> m_gameObjects;
};