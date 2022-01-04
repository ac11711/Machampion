#pragma once
#include "GameState.h"
#include <vector>

//A class that allows us to create and manage game states
class GameStateMachine {
public:

	//Constructor/Destructor

	GameStateMachine() {}
	~GameStateMachine() {}

	//Management functions

	void render();
	void update();
	void clean();

	//State changing functions

	void pushState(GameState* pState);
	void changeState(GameState* pState);
	void popState();

	//Getters

	std::vector<GameState*>& getGameStates() { return m_gameStates; }

private:

	//A vector that holds game states
	std::vector<GameState*> m_gameStates;
};