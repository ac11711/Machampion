#include "StateParser.h"
#include "TextureManager.h"
#include "CompressionHandler.h"
#include "GameObjectFactory.h"
#include "Game.h"

//Parse the game state
bool StateParser::parseState(const char* stateFile, std::string stateID, std::vector<GameObject*>* pObjects, std::vector<std::string>* pTextureIDs) {
	//Create XML document
	tinyxml2::XMLDocument XMLDoc;

	//Create parsed path
	std::vector<std::string> parsedPath = parsePath(stateFile);
	//Create archive name
	std::string archiveName = parsedPath.front() + getArchiveNameFromParsedPath(parsedPath);
	//Replace state file with compressed file
	stateFile = parsedPath.back().c_str();
	//Create destination path
	std::string destPath = parsedPath.front() + std::string(stateFile);
	//Decompress file
	decompressFile(archiveName, stateFile, destPath);

	//Load state file
	XMLDoc.LoadFile(destPath.c_str());

	//Delete extracted file
	deleteFile(destPath);

	//Set root element in state file
	tinyxml2::XMLElement* pRoot = XMLDoc.RootElement();

	//Pre declare states root element
	tinyxml2::XMLElement* pStateRoot = 0;
	
	//		---Save state's root element---

	//Go though each child of root element
	for (tinyxml2::XMLElement* e = pRoot->FirstChildElement(); e != NULL; e = e->NextSiblingElement()) {
		//If currnent element value maches state ID
		if (e->Value() == stateID) {
			//Set state root to current element
			pStateRoot = e;

			break;
		}
	}
	
	//		---Parse Textures---

	//Pre declare texture's root element
	tinyxml2::XMLElement* pTextureRoot = 0;

	//Go through each child of state's root element
	for (tinyxml2::XMLElement* e = pStateRoot->FirstChildElement(); e != NULL; e = e->NextSiblingElement()) {
		//If current element value maches TEXTURES
		if (e->Value() == std::string("TEXTURES")) {
			//Set texture root to current element
			pTextureRoot = e;

			break;
		}
	}	
	//Parse textures
	parseTextures(pTextureRoot, pTextureIDs);
	
	//		---Parse Objects---

	//Pre declare object's root element
	tinyxml2::XMLElement* pObjectRoot = 0;

	//Go through each child of state's root element
	for (tinyxml2::XMLElement* e = pStateRoot->FirstChildElement(); e != NULL; e = e->NextSiblingElement()) {
		//If current element matches OBJECTS
		if (e->Value() == std::string("OBJECTS")) {
			//Set object root to current element
			pObjectRoot = e;
			
			break;
		}
	}
	//Parse objects
	parseObjects(pObjectRoot, pObjects);

	return true;
}

//Parse textures
void StateParser::parseTextures(tinyxml2::XMLElement* pStateRoot, std::vector<std::string>* pTextureIDs) {
	//For each texture 
	for (tinyxml2::XMLElement* e = pStateRoot->FirstChildElement(); e != NULL; e = e->NextSiblingElement()) {
		//Get filename
		std::string filenameAttribute = e->Attribute("filename");
		//Get ID
		std::string idAttribute = e->Attribute("ID");
		
		//Add ID to texture ID vector
		pTextureIDs->push_back(idAttribute);

		//Load the texture
		TheTextureManager::Instance()->load(filenameAttribute, idAttribute, TheGame::Instance()->getRenderer());
	}
}

//Parse game objects
void StateParser::parseObjects(tinyxml2::XMLElement* pStateRoot, std::vector<GameObject*>* pObjects) {
	//For each object
	for (tinyxml2::XMLElement* e = pStateRoot->FirstChildElement(); e != NULL; e = e->NextSiblingElement()) {
		//Declare variables
		int x, y, width, height, numFrames, callbackID, animSpeed;
		std::string textureID;

		//Set variables
		e->QueryIntAttribute("x", &x);
		e->QueryIntAttribute("y", &y);
		e->QueryIntAttribute("width", &width);
		e->QueryIntAttribute("height", &height);
		e->QueryIntAttribute("numFrames", &numFrames);
		e->QueryIntAttribute("callbackID", &callbackID);
		e->QueryIntAttribute("animSpeed", &animSpeed);
		textureID = e->Attribute("textureID");

		//Create object pointer
		GameObject* pGameObject = TheGameObjectFactory::Instance()->create(e->Attribute("type"));
		//Load object
		pGameObject->load(std::unique_ptr<LoaderParams>(new LoaderParams(x, y, width, height, textureID, numFrames, callbackID, animSpeed)));
		//Add object to game objects vector
		pObjects->push_back(pGameObject);
	}
}