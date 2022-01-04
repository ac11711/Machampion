#pragma once
#include "Layer.h"
#include "CollisionManager.h"
#include <iostream>
#include <vector>

class GameObject;

//A class that allows us to create a layer of objects
class ObjectLayer : public Layer {
public:

	//Constructor/Destructor

	ObjectLayer() {}
	virtual ~ObjectLayer();

	//Management functions

	virtual void render();
	virtual void update(Level* pLevel);

	void loadPlayerAccess(GameObject* pPlayer);

	//Getters
	std::vector<GameObject*>* getGameObjects() { return &m_gameObjects; }

private:

	//Collision manager to handle collisions
	CollisionManager m_collisionManager;

	//Vector to hold game objects
	std::vector<GameObject*> m_gameObjects;
};