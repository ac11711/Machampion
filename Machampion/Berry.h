#pragma once
#include "ConsumableObject.h"
#include "Game.h"

//A class that allows us to have berries
class Berry : public ConsumableObject {
public:

	//Constructor/Destructor

	Berry() {}
	~Berry() {}

	//Management functions

	//Load the berry
	virtual void load(std::unique_ptr<LoaderParams> const& pParams) {
		//Load using base class
		ConsumableObject::load(std::move(pParams));
	}

	//Draw the berry
	virtual void draw() {
		//Draw using base class
		ConsumableObject::draw();
	}

	//Update the berry
	virtual void update() {
		//Update using base class
		ConsumableObject::update();
	}

	//Clean the berry
	virtual void clean() {
		//Clean using base class
		ConsumableObject::clean();
	}

	//Handle when coin collides
	virtual void collision(std::string fling) {
		//If berry has not been taken
		if (!m_bDying) {
			//Play berry sound
			TheSoundManager::Instance()->playSound("eat", 0);

			//Add 1 to player lives
			TheGame::Instance()->setPlayerLives(TheGame::Instance()->getPlayerLives() + 1);

			//Set berry to dying
			m_bDying = true;
		}
	}
};

//A class that creates berries
class BerryCreator : public BaseCreator {
	GameObject* createGameObject() const {
		return new Berry();
	}
};