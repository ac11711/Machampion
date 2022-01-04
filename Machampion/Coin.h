#pragma once
#include "ConsumableObject.h"
#include "Game.h"

//A class that allows us to have coins
class Coin : public ConsumableObject {
public:

	//Constructor/Destructor

	Coin() {}
	~Coin() {}

	//Management functions

	//Load the coin
	virtual void load(std::unique_ptr<LoaderParams> const& pParams) {
		//Load using base class
		ConsumableObject::load(std::move(pParams));
	}

	//Draw the coin
	virtual void draw() {
		//Draw using base class
		ConsumableObject::draw();
	}

	//Update the coin
	virtual void update() {
		//Update using base class
		ConsumableObject::update();
	}

	//Clean the coin
	virtual void clean() {
		//Clean using base class
		ConsumableObject::clean();
	}

	//Handle when coin collides
	virtual void collision(std::string fling) {
		//If coin has not been taken
		if (!m_bDying) {
			//Play coin sound
			TheSoundManager::Instance()->playSound("coin", 0);

			//Add 1 to coin count
			TheGame::Instance()->setCoins(TheGame::Instance()->getCoins() + 1);

			//Redraw coin counter
			TheTextManager::Instance()->updateCoins();

			//Set coin to dying
			m_bDying = true;
		}
	}
};

//A class that allows us to create coins
class CoinCreator : public BaseCreator {
	GameObject* createGameObject() const {
		return new Coin();
	}
};