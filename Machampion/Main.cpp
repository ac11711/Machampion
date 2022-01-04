#include "Game.h"
#include <iostream>
#include <Windows.h>
#include <cstdlib>
#include <ctime>

const int FPS = 60;
const int DELAY_TIME = 1000.0f / FPS;

int main(int argc, char** argv) {
	//Create frame variables
	Uint32 frameStart, frameTime;

	//Output
	std::cout << "Initializing game...\n";

	//If game is initialized
	if (TheGame::Instance()->init("Machampion", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 1920, 960, false)) {

		//GAME LOOP
		while (TheGame::Instance()->running()) {
			//Set frame time
			frameStart = SDL_GetTicks();

			//Set random seed
			srand(time(0));

			TheGame::Instance()->handleEvents();
			TheGame::Instance()->update();
			TheGame::Instance()->render();

			//Set frame time
			frameTime = SDL_GetTicks() - frameStart;

			//If frame duration is less than delay time and vsync is disabled
			if (frameTime < DELAY_TIME && !TheGame::Instance()->vsync()) {
				//Delay for the difference
				SDL_Delay((int)(DELAY_TIME - frameTime));
			}
		}
	}
	//Game initalized failure
	else {
		//Output
		std::cout << "Game initialize failure\n";

		return -1;
	}

	//Output
	std::cout << "Closing game...\n";

	//Clean game
	TheGame::Instance()->clean();

	return 0;
}