#pragma once
#include "GameObject.h"
#include <iostream>
#include <string>
#include <map>

//A class to create game objects
class BaseCreator {
public:

	virtual GameObject* createGameObject() const = 0;
	virtual ~BaseCreator() {}
};

//A class that allows us to create, register, and store types of game objects
class GameObjectFactory {
public:

	//Ensures only one instance
	static GameObjectFactory* Instance() {
		if (s_pInstance == 0) {
			s_pInstance = new GameObjectFactory();
		}

		return s_pInstance;
	}

	//Add type to the map
	bool registerType(std::string typeID, BaseCreator* pCreator) {
		//Create a map iterator
		std::map<std::string, BaseCreator*>::iterator it = m_creators.find(typeID);

		//If type is already registered
		if (it != m_creators.end()) {
			//Do nothing
			delete pCreator;
		}

		//Add type to the map
		m_creators[typeID] = pCreator;

		return true;
	}

	//Create game object
	GameObject* create(std::string typeID) {
		//Create map iterator
		std::map<std::string, BaseCreator*>::iterator it = m_creators.find(typeID);

		//If type is not in map
		if (it == m_creators.end()) {
			std::cout << "Could not find type: " << typeID << std::endl;

			return NULL;
		}

		//Create creator
		BaseCreator* pCreator = (*it).second;

		return pCreator->createGameObject();
	}

private:

	//Constructor/Destructor

	GameObjectFactory() {}
	~GameObjectFactory() {}

	//Instance
	static GameObjectFactory* s_pInstance;

	//Map to hold creators
	std::map<std::string, BaseCreator*> m_creators;
};

typedef GameObjectFactory TheGameObjectFactory;