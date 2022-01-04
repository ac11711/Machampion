#pragma once
#include "GameState.h"

//A class that allows us to make the game's main menu
class MenuState : public GameState {
public:

	//Destructor
	virtual ~MenuState() {}

protected:

	//		---Member functions---

	typedef void(*Callback)();
	virtual void setCallbacks(const std::vector<Callback>& callbacks) = 0;

	//		---Member variables---

	std::vector<Callback> m_callbacks;
};