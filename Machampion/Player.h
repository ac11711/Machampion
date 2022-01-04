#pragma once
#include "GameObjectFactory.h"
#include "PlatformerObject.h"

//A class that allows us to create the player
class Player : public PlatformerObject {
public:

	//Constructor/Destructor

	Player();
	virtual ~Player() {}

	//Management functions

	virtual void load(std::unique_ptr<LoaderParams> const& pParams);
	virtual void draw();
	virtual void update();
	virtual void clean();
	
	//Handle collisions
	virtual void collision();

	//Getters

	virtual std::string type() { return "Player"; }

private:

	//		---Member Functions---

	void ressurect();
	void handleInput();
	void handleMovement(Vector2D velocity);
	void handleAnimation();

	//		---Member Variables---

	//Invulnerability variables

	bool m_bInvulnerable;
	int m_invulnerableTime;
	int m_invulnerableCounter;

	//Hit variables

	bool m_bPressedJump;
	bool m_bPressedHit;
	bool m_bPressedShoot;
};

//A class that creates a player
class PlayerCreator : public BaseCreator {
	GameObject* createGameObject() const {
		return new Player();
	}
};