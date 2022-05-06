#include "PlayState.h"
#include "TextureManager.h"
#include "SoundManager.h"
#include "TextManager.h"
#include "InputHandler.h"
#include "ProjectileHandler.h"
#include "PauseState.h"
#include "GameOverState.h"
#include "LevelParser.h"
#include "Level.h"
#include "Game.h"

//Initialize static ID
const std::string PlayState::s_playID = "PLAY";

//Entry function
bool PlayState::onEnter() {
	//Set 3 player lives
	TheGame::Instance()->setPlayerLives(3);

	//Create level parser
	LevelParser levelParser;

	//Parse level
	pLevel = levelParser.parseLevel(TheGame::Instance()->getLevelFiles()[TheGame::Instance()->getCurrentLevel() - 1].c_str());

	//Load textures
	TheTextureManager::Instance()->load("assets/icons.bmna/life.png", "lives", TheGame::Instance()->getRenderer());

	//If there is a level
	if (pLevel != 0) {
		m_loadingComplete = true;
	}

	//If music is paused
	if (Mix_PausedMusic()) {
		//Resume music
		Mix_ResumeMusic();
	}
	//Music is not paused
	else {
		//Play music
		TheSoundManager::Instance()->playMusic("music1", -1);
	}

	std::cout << "Entering play state\n";

	return true;
}

//Render the play state
void PlayState::render() {
	//If loading done
	if (m_loadingComplete && !m_exiting) {
		//If there is a level
		if (pLevel != 0) {
			//Render the level
			pLevel->render();
		}

		//For each player life
		for (int i = 0; i < TheGame::Instance()->getPlayerLives(); i++) {
			//Draw lives
			TheTextureManager::Instance()->drawFrame("lives", i * 30, 0, 35, 38, 0, 0, TheGame::Instance()->getRenderer(), 0.0, 255);
		}

		//Draw textures
		TheTextureManager::Instance()->drawFrame("coin", TheGame::Instance()->getGameWidth() - 105, 4, 32, 32, 0, 0, TheGame::Instance()->getRenderer(), 0.0, 255);
		TheTextManager::Instance()->draw("coins", TheGame::Instance()->getGameWidth() - 75, 0, TheGame::Instance()->getRenderer());
		TheProjectileHandler::Instance()->draw();
	}
}

//Update the play state
void PlayState::update() {
	//If loading done and not leaving play state
	if (m_loadingComplete && !m_exiting) {
		//If there is a joystick and joysticks have not already been initialized
		if ((int)SDL_NumJoysticks > 0 && !TheInputHandler::Instance()->joysticksInitialized()) {
			//Initialize joysticks
			TheInputHandler::Instance()->initializeJoysticks();
		}

		//If escape button is down or start button is pressed
		if (TheInputHandler::Instance()->isKeyDown(SDL_SCANCODE_ESCAPE) || TheInputHandler::Instance()->getButtonState(0, 7)) {
			//Switch to pause state
			TheGame::Instance()->getStateMachine()->pushState(new PauseState());
		}

		//If player lives is 0
		if (TheGame::Instance()->getPlayerLives() == 0) {
			//Switch to pause state
			TheGame::Instance()->getStateMachine()->changeState(new GameOverState());
		}

		//If there is a level
		if (pLevel != 0) {
			pLevel->update();
		}

		//Update projectiles
		TheProjectileHandler::Instance()->update();
	}
}

//Exit function
bool PlayState::onExit() {
	//Set to exiting
	m_exiting = true;

	//If loading is done and there are objects
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

	//Clear projectiles
	TheProjectileHandler::Instance()->clearProjectiles();

	std::cout << "Exiting play state\n";

	return true;
}