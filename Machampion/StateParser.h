#pragma once
#include "tinyxml2.h"
#include <iostream>
#include <vector>

class GameObject;

//A class that allows us to read game state files
class StateParser {
public:

	bool parseState(const char* stateFile, std::string stateID, std::vector<GameObject*>* pObjects, std::vector<std::string>* pTextureIDs);

private:

	void parseTextures(tinyxml2::XMLElement* pStateRoot, std::vector<std::string>* pObjects);
	void parseObjects(tinyxml2::XMLElement* pStateRoot, std::vector<GameObject*>* pObjects);
};