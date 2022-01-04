#include "MainMenuState.h"
#include "TextManager.h"
#include "InputHandler.h"
#include "MenuButton.h"
#include "StateParser.h"
#include "PlayState.h"
#include "Game.h"

//Initialize static ID
const std::string MainMenuState::s_menuID = "MENU";

//Render main menu
void MainMenuState::render() {
	//If state is done loading and game objects are not empty
	if (m_loadingComplete && !m_gameObjects.empty()) {
		//For each game object
		for (int i = 0; i < m_gameObjects.size(); i++) {
			//Draw object
			m_gameObjects[i]->draw();
		}

		//Draw cursor
		TheCursor::Instance()->draw();
	}
}

//Update main menu
void MainMenuState::update() {
	//If state is done loading and game objects are not empty
	if (m_loadingComplete && !m_gameObjects.empty()) {
		//If there is a joystick and joysticks have not already been initialized
		if (SDL_NumJoysticks > 0 && !TheInputHandler::Instance()->joysticksInitialized()) {
			//Initialize joysticks
			TheInputHandler::Instance()->initializeJoysticks();
		}

		//For each game object
		for (int i = 0; i < m_gameObjects.size(); i++) {
			//Update object
			m_gameObjects[i]->update();
		}

		//Update cursor
		TheCursor::Instance()->update();
	}
}

//Entry function
bool MainMenuState::onEnter() {
	//Declare parser
	StateParser stateParser;
	//Parse the state
	stateParser.parseState("assets/maps.bmna/gamestates.xml", s_menuID, &m_gameObjects, &m_textureIDList);

	//Load the cursor
	TheCursor::Instance()->load("assets/icons.bmna/cursor.png", "cursor", 935, 530, 50, 40, 25);
	
	//Put callbacks in callbacks vector
	m_callbacks.push_back(0);
	m_callbacks.push_back(s_menuToPlay);
	m_callbacks.push_back(s_exitFromMenu);

	//Set callbacks for menu objects
	setCallbacks(m_callbacks);

	//For each state in game states vector
	for (int i = 0; i < TheGame::Instance()->getStateMachine()->getGameStates().size(); i++) {
		//If game state is a play state
		if (TheGame::Instance()->getStateMachine()->getGameStates()[i]->getStateID() == std::string("PLAY")) {
			//Run exit function
			TheGame::Instance()->getStateMachine()->getGameStates()[i]->onExit();
			//Remove play state
			TheGame::Instance()->getStateMachine()->getGameStates().erase(TheGame::Instance()->getStateMachine()->getGameStates().begin() + i);
		}
	}

	//Reset coins
	TheGame::Instance()->setCoins(0);

	//Update coin cointer
	TheTextManager::Instance()->updateCoins();

	//Loading is done
	m_loadingComplete = true;
	
	std::cout << "\nEntering main menu state\n";

	return true;
}

//Exit function
bool MainMenuState::onExit() {
	//Exiting state
	m_exiting = true;

	//If loading is done and there are are objects
	if (m_loadingComplete && !m_gameObjects.empty()) {
		//For each game object
		for (int i = 0; i < m_gameObjects.size(); i++) {
			//Clean game object
			m_gameObjects[i]->clean();
			//Delete pointer
			delete m_gameObjects[i];
		}
		//Clear game objects vector
		m_gameObjects.clear();
	}

	//For each texture
	for (int i = 0; i < m_textureIDList.size(); i++) {
		//Clear texture
		TheTextureManager::Instance()->clearFromTextureMap(m_textureIDList[i]);
	}

	//Reset mouse buttons
	TheInputHandler::Instance()->reset();
	//Reset joystick buttons
	TheInputHandler::Instance()->resetButtons();
	
	std::cout << "Exiting main menu state\n";

	return true;
}

//Callback to play state
void MainMenuState::s_menuToPlay() {
	//Change state to a new play state
	TheGame::Instance()->getStateMachine()->changeState(new PlayState());
}

//Call back to exit menu
void MainMenuState::s_exitFromMenu() {
	//Quit game
	TheGame::Instance()->quit();
}

//Assign callbacks
void MainMenuState::setCallbacks(const std::vector<Callback>& callbacks) {
	//If game objects are not empty
	if (!m_gameObjects.empty()) {
		//For each game object
		for (int i = 0; i < m_gameObjects.size(); i++) {
			//If they are a menu button
			if (dynamic_cast<MenuButton*>(m_gameObjects[i])) {
				//Set button to current object
				MenuButton* pButton = dynamic_cast<MenuButton*>(m_gameObjects[i]);
				//Assign callback
				pButton->setCallback(callbacks[pButton->getCallbackID()]);
			}
		}
	}
}