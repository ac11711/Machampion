#pragma once
#include "GameState.h"
#include "CollisionManager.h"
#include "Level.h"
#include <vector>

class GameObject;
class SDLGameObject;
class Level;

//A class that allows us to display the current level
class PlayState : public GameState {
public:

	//Destructor
	virtual ~PlayState() { delete pLevel; }

	//Management functions

	virtual void render();
	virtual void update();

	virtual bool onEnter();
	virtual bool onExit();

	//Getters

	virtual std::string getStateID() const { return s_playID; }

private:

	static const std::string s_playID;

	Level* pLevel;

	CollisionManager m_collisionManager;

	std::vector<GameObject*> m_gameObjects;
};