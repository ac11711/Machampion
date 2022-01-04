#pragma once
#include "SDL.h"
#include "GameObject.h"

//A class that allows us to create and manage in game objects
class PlatformerObject : public GameObject {
public:

	//Destructor
	virtual ~PlatformerObject() {}

	//Management functions

	virtual void load(std::unique_ptr<LoaderParams> const& pParams);
	virtual void draw();
	virtual void update();
	virtual void clean() {}

	virtual void collision(std::string fling = "none") {}
	void thrust(std::string fling, int x, int y);
	bool checkCollideTile(Vector2D newPos);
	bool checkProjectileCollideTile(Vector2D newPos);
	bool checkFluidTile(Vector2D newPos);
	bool checkHazardTile(Vector2D newPos);

	//Setters

	void setBouncing(bool bouncing) { m_bBouncing = bouncing; }

	//Getters

	virtual std::string type() { return "SDLGameObject"; }
	bool isFlipped() { return m_bFlipped; }
	bool isShiny() { return m_bShiny; }
	bool isHitting() { return m_bHitting; }

protected:

	//		---Member functions---

	//Constructor
	PlatformerObject();

	//		---Member variables---

	//Orientation variables

	bool m_bFlipped;
	bool m_bShiny;

	//Movement variables

	Vector2D m_lastSafePos;
	bool m_bMoveRight;
	bool m_bMoveLeft;
	bool m_bMoveUp;
	bool m_bMoveDown;
	int m_jumpHeight;
	bool m_bJumping;
	bool m_bFalling;
	bool m_bRunning;
	bool m_bSlowed;
	int m_slowedTime;
	int m_slowedCounter;
	bool m_bBouncing;
	bool m_bCanJump;
	int m_bounceTime;
	int m_bounceCounter;
	bool m_bThrusting;
	int m_thrustTime;
	int m_thrustCounter;

	//Hitting variables

	bool m_bHitting;
	int m_hittingTime;
	int m_hitSpeed;
	int m_hitCounter;

	//Shooting variables

	bool m_bShooting;
	int m_shootingTime;
	int m_shootSpeed;
	int m_bulletCounter;
	int m_bulletVelocity;

	//Hitting and shooting variables

	bool m_bCanHit;
	int m_recoveryCounter;

	//Dying variables

	int m_dyingTime;
	int m_dyingCounter;
	bool m_bPlayedDeathSound;
};