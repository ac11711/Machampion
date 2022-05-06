#include "PauseState.h"
#include "MainMenuState.h"
#include "PlayState.h"
#include "TextureManager.h"
#include "InputHandler.h"
#include "MenuButton.h"
#include "StateParser.h"
#include "Game.h"
#include "SoundManager.h"

//Initialize static ID
const std::string PauseState::s_pauseID = "PAUSE";

//Render the pause state
void PauseState::render() {
	//If loading is done and there are objects
	if (m_loadingComplete && !m_gameObjects.empty()) {
		//For each game object
		for (int i = 0; i < m_gameObjects.size(); i++) {
			//Draw game object
			m_gameObjects[i]->draw();
		}

		//Draw cursor
		TheCursor::Instance()->draw();
	}
}

//Update the game object
void PauseState::update() {
	//If loading is done and there are objects
	if (m_loadingComplete && !m_gameObjects.empty()) {
		//If there is a joystick and joysticks have not already been initialized
		if ((int)SDL_NumJoysticks > 0 && !TheInputHandler::Instance()->joysticksInitialized()) {
			//Initialize joysticks
			TheInputHandler::Instance()->initializeJoysticks();
		}

		//If start button is pressed
		if (TheInputHandler::Instance()->getButtonState(0, 7)) {
			//Resume play state
			s_resumePlay();
		}

		//For each game object
		for (int i = 0; i < m_gameObjects.size(); i++) {
			//Update game object
			m_gameObjects[i]->update();
		}

		//Update cursor
		TheCursor::Instance()->update();
	}
}

//Entry function
bool PauseState::onEnter() {
	//Create state parser
	StateParser stateParser;

	//Parse pause state
	stateParser.parseState("assets/maps.bmna/gamestates.xml", s_pauseID, &m_gameObjects, &m_textureIDList);

	//Set cursor x and y position
	TheCursor::Instance()->getPosition().m_x = 935;
	TheCursor::Instance()->getPosition().m_y = 480;

	//Add callbacks
	m_callbacks.push_back(0);
	m_callbacks.push_back(s_pauseToMain);
	m_callbacks.push_back(s_resumePlay);

	//Set callbacks
	setCallbacks(m_callbacks);

	//Pause music
	Mix_PauseMusic();

	//Reset joystick buttons
	TheInputHandler::Instance()->resetButtons();

	//Loading done
	m_loadingComplete = true;

	std::cout << "Entering pause state\n";

	return true;
}

//Exit function
bool PauseState::onExit() {
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

	std::cout << "Exiting pause state\n";

	return true;
}

//Assign callbacks
void PauseState::setCallbacks(const std::vector<Callback>& callbacks) {
	//If there are game objects
	if (!m_gameObjects.empty()) {
		//For each game object
		for (int i = 0; i < m_gameObjects.size(); i++) {
			//If object is a menu button
			if (dynamic_cast<MenuButton*>(m_gameObjects[i])) {
				//Set button
				MenuButton* pButton = dynamic_cast<MenuButton*>(m_gameObjects[i]);
				//Assign callback
				pButton->setCallback(callbacks[pButton->getCallbackID()]);
			}
		}
	}
}

//Transition to main menu
void PauseState::s_pauseToMain() {
	//Change state to main menu state
	TheGame::Instance()->getStateMachine()->changeState(new MainMenuState());
}

//Transition to play state
void PauseState::s_resumePlay() {
	//Resume music
	Mix_ResumeMusic();

	//Change state to play state
	TheGame::Instance()->getStateMachine()->popState();
}