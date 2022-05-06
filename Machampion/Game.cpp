#include "Game.h"
#include "TextureManager.h"
#include "SoundManager.h"
#include "TextManager.h"
#include "InputHandler.h"
#include "AnimatedGraphic.h"
#include "ScrollingBackground.h"
#include "GameObjectFactory.h"
#include "Player.h"
#include "Psyduck.h"
#include "Pidgey.h"
#include "Coin.h"
#include "Berry.h"
#include "MenuButton.h"
#include "MainMenuState.h"
#include "GameOverState.h"
#include <iostream>

//Initialize static instance
Game* Game::s_pInstance = 0;

Game::Game() :
	m_pWindow(0),
	m_pRenderer(0),
	m_bRunning(false),
	m_gameWidth(0),
	m_gameHeight(0),
	m_bVsync(false),
	m_pGameStateMachine(0),
	m_bLevelComplete(false),
	m_nextLevel(0),
	m_playerLives(3),
	m_coins(0)
{
	//Add levels to levels vector
	m_levelFiles.push_back("assets/maps.bmna/testlevel2.tmx");

	//Start at level 1
	m_currentLevel = 1;
}

Game::~Game() {
	//Reset pointers
	m_pRenderer = 0;
	m_pWindow = 0;
}

bool Game::init(const char* title, int xpos, int ypos, int width, int height, bool fullscreen) {
	//Set flags
	int flags = 0;

	//Set game width and height
	m_gameWidth = width;
	m_gameHeight = height;

	//If fullscreen
	if (fullscreen) {
		//Set flag to fullscreen
		flags = SDL_WINDOW_FULLSCREEN;
	}

	//		---Initialze Game---

	//--SUCCESS-- SDL initialized
	if (SDL_Init(SDL_INIT_EVERYTHING) == 0 && TTF_Init() == 0) {
		//Output
		std::cout << "SDL initialized successfully\n";

		//--INIT-- window
		m_pWindow = SDL_CreateWindow(title, xpos, ypos, width, height, flags);

		//--SUCCESS-- window created
		if (m_pWindow != 0) {
			//Output
			std::cout << "Window initialized successfully\n";

			//Set Vsync
			//SDL_SetHint(SDL_HINT_RENDER_VSYNC, "1");
			//m_bVsync = true;

			//--INIT-- renderer
			m_pRenderer = SDL_CreateRenderer(m_pWindow, -1, SDL_RENDERER_ACCELERATED);

			//--SUCCESS-- renderer created
			if (m_pRenderer != 0) {
				//Output
				std::cout << "Renderer initialized successfully\n";

				//Load loading screen
				TheTextureManager::Instance()->load("assets/backgrounds.bmna/loadingscreen.png", "loading", m_pRenderer);

				//Set renderer color to clear
				SDL_SetRenderDrawColor(m_pRenderer, 0, 0, 0, 255);

				//Draw loading screen
				TheTextureManager::Instance()->draw("loading", 0, 0, width, height, m_pRenderer);

				//Draw renderer
				SDL_RenderPresent(m_pRenderer);
			}
			//--FAILURE-- renderer failed
			else {
				//Output
				std::cout << "Renderer initialize failure\n";

				return false;
			}
		}
		//--FAILURE-- window failed
		else {
			//Output
			std::cout << "Window initialize failure\n";

			return false;
		}
	}
	//--FAILURE-- SDL failed
	else {
		std::cout << "SDL initialize failure\n";

		return false;
	}
	//Game initialized successfully
	std::cout << "Game initialized successfully\n";

	//Register types used in game
	TheGameObjectFactory::Instance()->registerType("AnimatedGraphic", new AnimatedGraphicCreator());
	TheGameObjectFactory::Instance()->registerType("ScrollingBackground", new ScrollingBackgroundCreator());
	TheGameObjectFactory::Instance()->registerType("MenuButton", new MenuButtonCreator());
	TheGameObjectFactory::Instance()->registerType("Player", new PlayerCreator());
	TheGameObjectFactory::Instance()->registerType("Psyduck", new PsyduckCreator());
	TheGameObjectFactory::Instance()->registerType("Pidgey", new PidgeyCreator());
	TheGameObjectFactory::Instance()->registerType("Coin", new CoinCreator());
	TheGameObjectFactory::Instance()->registerType("Berry", new BerryCreator());

	//Load sound effects
	TheSoundManager::Instance()->load("assets/sounds.bmna/route420.ogg", "music1", SOUND_MUSIC);
	TheSoundManager::Instance()->load("assets/sounds.bmna/crack.wav", "death", SOUND_SFX);
	TheSoundManager::Instance()->load("assets/sounds.bmna/bouncesound.wav", "bounce", SOUND_SFX);
	TheSoundManager::Instance()->load("assets/sounds.bmna/rockbreak.wav", "break", SOUND_SFX);
	TheSoundManager::Instance()->load("assets/sounds.bmna/coinsound.wav", "coin", SOUND_SFX);
	TheSoundManager::Instance()->load("assets/sounds.bmna/eat.wav", "eat", SOUND_SFX);

	//Load text
	TheTextManager::Instance()->load(std::to_string(m_coins), "coins", "RetroGame.ttf", 32, 0, 0, 0, TheGame::Instance()->getRenderer());
	
	//Initialize joystick
	TheInputHandler::Instance()->initializeJoysticks();
	
	//Create game state machine
	m_pGameStateMachine = new GameStateMachine(); 
	//Set to main menu state
	m_pGameStateMachine->changeState(new MainMenuState());
	
	//Start game loop
	m_bRunning = true;

	//Delay for loading screen
	SDL_Delay(1500);

	return true;
}

//Render the game
void Game::render() {
	//Clear renderer
	SDL_RenderClear(m_pRenderer);
	//Render game state
	m_pGameStateMachine->render();
	//Present renderer
	SDL_RenderPresent(m_pRenderer);
}

//Update the game
void Game::update() {
	//Update the game state
	m_pGameStateMachine->update();
}

//Handle inputs
void Game::handleEvents() {
	//Update input handler
	TheInputHandler::Instance()->update();
}

//Clean game
void Game::clean() {
	//Output
	std::cout << "Cleaning game\n";

	//Clear texture map
	TheTextureManager::Instance()->clearTextureMap();
	//Clean sound manager
	TheSoundManager::Instance()->clearSoundMap();
	//Clear texture map
	TheTextManager::Instance()->clearTextureMap();
	//Clean input handler
	TheInputHandler::Instance()->clean();

	//Delete music
	TheSoundManager::Instance()->deleteMusic("assets/route420.ogg");

	//Clean game state machine
	m_pGameStateMachine->clean();
	//Clear game state machine
	m_pGameStateMachine = 0;
	//Delete pointer
	delete m_pGameStateMachine;

	//Delete window
	SDL_DestroyWindow(m_pWindow);
	//Delete renderer
	SDL_DestroyRenderer(m_pRenderer);

	//Quit SDL_ttf
	TTF_Quit();
	//Quit SDL
	SDL_Quit();
}

//Set level
void Game::setCurrentLevel(int currentLevel) {
	//Set level
	m_currentLevel = currentLevel;
	//Change to game over state
	m_pGameStateMachine->changeState(new GameOverState());
	//Level not complete
	m_bLevelComplete = false;
}