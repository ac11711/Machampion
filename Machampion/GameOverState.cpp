#include "GameOverState.h"
#include "MainMenuState.h"
#include "PlayState.h"
#include "MenuButton.h"
#include "StateParser.h"
#include "TextureManager.h"
#include "TextManager.h"
#include "InputHandler.h"
#include "AnimatedGraphic.h"
#include "Game.h"

//Initialize static ID
const std::string GameOverState::s_gameOverID = "GAMEOVER";

//Render the game over state
void GameOverState::render() {
	//If loading is done and there are game objects
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

//Update the game over state
void GameOverState::update() {
	//If loading is done and there are game objects
	if (m_loadingComplete && !m_gameObjects.empty()) {
		//If there is a joystick and joysticks have not already been initialized
		if (SDL_NumJoysticks > 0 && !TheInputHandler::Instance()->joysticksInitialized()) {
			//Initialize joysticks
			TheInputHandler::Instance()->initializeJoysticks();
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
bool::GameOverState::onEnter() {
	//Create state parser
	StateParser stateParser;

	//Parse game over state
	stateParser.parseState("assets/maps/gamestates.xml", s_gameOverID, &m_gameObjects, &m_textureIDList);

	//Set cursor x and y position
	TheCursor::Instance()->getPosition().m_x = 935;
	TheCursor::Instance()->getPosition().m_y = 480;

	//Insert callbacks
	m_callbacks.push_back(0);
	m_callbacks.push_back(s_gameOverToMain);
	m_callbacks.push_back(s_restartToPlay);

	//Set callbacks for menu buttons
	setCallbacks(m_callbacks);

	//Pause music
	Mix_PauseMusic();

	//Reset coins
	TheGame::Instance()->setCoins(0);

	//Update coin cointer
	TheTextManager::Instance()->updateCoins();

	//Loading done
	m_loadingComplete = true;

	std::cout << "Entering game over state\n";

	return true;
}

//Exit function
bool::GameOverState::onExit() {
	//If loading is done and there are gameObjects
	if (m_loadingComplete && !m_gameObjects.empty()) {
		//For each game object
		for (int i = 0; i < m_gameObjects.size(); i++) {
			//Clean game object
			m_gameObjects[i]->clean();
			//Delete game object pointer
			delete m_gameObjects[i];
		}

		//Clear game objects vector
		m_gameObjects.clear();
	}

	//For each texture
	for (int i = 0; i < m_textureIDList.size(); i++) {
		//Clear texture from textures vector
		TheTextureManager::Instance()->clearFromTextureMap(m_textureIDList[i]);
	}

	//Reset mouse buttons
	TheInputHandler::Instance()->reset();
	//Reset joystick buttons
	TheInputHandler::Instance()->resetButtons();

	std::cout << "Exititng game over state\n";

	return true;
}

//Set callbacks
void GameOverState::setCallbacks(const std::vector<Callback>& callbacks) {
	//For each game object
	for (int i = 0; i < m_gameObjects.size(); i++) {
		//If it is a menu button
		if (dynamic_cast<MenuButton*>(m_gameObjects[i])) {
			//Set menu button
			MenuButton* pButton = dynamic_cast<MenuButton*>(m_gameObjects[i]);
			//Set callback
			pButton->setCallback(callbacks[pButton->getCallbackID()]);
		}
	}
}

//Transition to main menu
void GameOverState::s_gameOverToMain() {
	//Change state to main menu state
	TheGame::Instance()->getStateMachine()->changeState(new MainMenuState());
}

//Transition to game state
void GameOverState::s_restartToPlay() {
	//Change state to main menu state
	TheGame::Instance()->getStateMachine()->changeState(new PlayState());
}