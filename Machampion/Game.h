#pragma once
#include "SDL.h"
#include "Vector2D.h"
#include "GameStateMachine.h"
#include <vector>

//A class that allows us to create and run the game
class Game {
public:

	//Ensures only one instance
	static Game* Instance() {
		if (s_pInstance == 0) {
			s_pInstance = new Game();

			return s_pInstance;
		}

		return s_pInstance;
	}

	//Management functions

	bool init(const char* title, int xpos, int ypos, int width, int height, bool fullscreen);
	void render();
	void update();
	void handleEvents();
	void clean();

	void quit() { m_bRunning = false; }

	//Setters

	void setPlayerLives(int lives) { m_playerLives = lives; }
	void setCurrentLevel(int currentLevel);
	void setNextLevel(int nextLevel) { m_nextLevel = nextLevel; }
	void setLevelComplete(bool levelComplete) { m_bLevelComplete = levelComplete; }
	void setCoins(int coins) { m_coins = coins; }

	//Getters

	SDL_Window* getWindow() const { return m_pWindow; }
	SDL_Renderer* getRenderer() const { return m_pRenderer; }
	GameStateMachine* getStateMachine() { return m_pGameStateMachine; }
	int getGameWidth() { return m_gameWidth; }
	int getGameHeight() { return m_gameHeight; }
	bool vsync() { return m_bVsync; }
	int getPlayerLives() { return m_playerLives; }
	const int getCurrentLevel() { return m_currentLevel; }
	const int getNextLevel() { return m_nextLevel; }
	const bool getLevelComplete() { return m_bLevelComplete; }
	bool running() { return m_bRunning; }
	std::vector<std::string> getLevelFiles() { return m_levelFiles; }
	int getCoins() { return m_coins; }

private:

	//Constructor/Destructor

	Game();
	~Game();

	//Instance
	static Game* s_pInstance;

	//SDL variables

	SDL_Window* m_pWindow;
	SDL_Renderer* m_pRenderer;

	//Game variables

	GameStateMachine* m_pGameStateMachine;

	int m_gameWidth;
	int m_gameHeight;

	bool  m_bVsync;
	
	int m_playerLives;
	int m_currentLevel;
	int m_nextLevel;
	bool m_bLevelComplete;
	bool m_bRunning;

	int m_coins;

	std::vector<std::string> m_levelFiles;
};

typedef Game TheGame;