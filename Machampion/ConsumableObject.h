#pragma once
#include "TextureManager.h"
#include "Camera.h"
#include "GameObject.h"
#include "GameObjectFactory.h"

//A class that allows us to create and manage consumable game objects
class ConsumableObject : public GameObject {
public:

	//Constructor/Destructor

	ConsumableObject() {}
	virtual ~ConsumableObject() {}

	//Management functions

	virtual void load(std::unique_ptr<LoaderParams> const& pParams);
	virtual void draw();
	virtual void update();
	virtual void clean() {}

	//Handle collisions
	virtual void collision(std::string fling = "none") {}

	//Getters

	virtual std::string type() { return "ConsumableObject"; }
};