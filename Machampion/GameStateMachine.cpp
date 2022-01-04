#include "GameStateMachine.h"
#include <iostream>

//Render the state
void GameStateMachine::render() {
	//If game states is not empty
	if (!m_gameStates.empty()) {
		//Render last state using base class
		m_gameStates.back()->render();
	}
}

//Update the state
void GameStateMachine::update() {
	//If game states is not empty
	if (!m_gameStates.empty()) {
		//Update last state using base class
		m_gameStates.back()->update();
	}
}

//Clean the state
void GameStateMachine::clean() {
	//If game state is not empty
	if (!m_gameStates.empty()) {
		//Handle game state when exiting
		m_gameStates.back()->onExit();

		//Delete pointer
		delete m_gameStates.back();

		//Clear the game states vector
		m_gameStates.clear();
	}
}

//Add a new game state
void GameStateMachine::pushState(GameState* pState) {
	//Add state to states vector
	m_gameStates.push_back(pState);
	//Run entry function
	m_gameStates.back()->onEnter();
}

//Change to a new state
void GameStateMachine::changeState(GameState* pState) {
	//If game states vector is not empty
	if (!m_gameStates.empty()) {
		//If game state is the same as current game state
		if (m_gameStates.back()->getStateID() == pState->getStateID()) {
			//Do nothing
			return;
		}

		//Run exit function
		m_gameStates.back()->onExit();
		//Remove state from states vector
		m_gameStates.pop_back();
	}

	//Run entry function
	pState->onEnter(); 

	//Add state to states vector
	m_gameStates.push_back(pState);
}

//Remove a game state
void GameStateMachine::popState() {
	//If game states vector is not empty
	if (!m_gameStates.empty()) {
		//Run exit function
		m_gameStates.back()->onExit();
		//Remove state from states vector
		m_gameStates.pop_back();
	}

	//Resume current game state
	m_gameStates.back()->resume();
}