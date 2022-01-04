#include "LevelParser.h"
#include "TextureManager.h"
#include "GameObjectFactory.h"
#include "TileLayer.h"
#include "ObjectLayer.h"
#include "Level.h"
#include "Game.h"

//Parse level file
Level* LevelParser::parseLevel(const char* levelFile) {
	//Create XML document
	tinyxml2::XMLDocument levelDocument;
	//Load file in document
	levelDocument.LoadFile(levelFile);

	//Create level object
	Level* pLevel = new Level();

	//Get root element in document
	tinyxml2::XMLElement* pRoot = levelDocument.RootElement();

	//Set properties root element
	tinyxml2::XMLElement* pProperties = pRoot->FirstChildElement();

	//Output
	std::cout << "Loading level: " << levelFile << "\nVersion: " << pRoot->Attribute("version") << std::endl;
	std::cout << "Width: " << pRoot->Attribute("width") << " - Height: " << pRoot->Attribute("height") << std::endl;
	std::cout << "Tile Width: " << pRoot->Attribute("tilewidth") << " - Tile Height: " << pRoot->Attribute("tileheight") << std::endl;

	//Set level dimensions
	pRoot->QueryIntAttribute("tilewidth", &m_tileSize);
	pRoot->QueryIntAttribute("width", &m_width);
	pRoot->QueryIntAttribute("height", &m_height);

	//		---Parse Textures---

	//For each child in properties element
	for (tinyxml2::XMLElement* e = pProperties->FirstChildElement(); e != NULL; e = e->NextSiblingElement()) {
		//If element value is property
		if (e->Value() == std::string("property")) {
			//Parse textures
			parseTextures(e);
		}
	}

	//		---Parse Tilesets---

	//For each child in properties element
	for (tinyxml2::XMLElement* e = pRoot->FirstChildElement(); e != NULL; e = e->NextSiblingElement()) {
		//If element value is tileset
		if (e->Value() == std::string("tileset")) {
			//Get tileset file
			e->QueryStringAttribute("source", &m_tilesetFile);
			//Get tileset first gid
			e->QueryIntAttribute("firstgid", &m_firstGid);

			//Parse tileset
			parseTilesets(m_tilesetFile, m_firstGid, pLevel->getTilesets());
		}
	}
	
	//		---Parse Object Layers---

	//For each child in properties element
	for (tinyxml2::XMLElement* e = pRoot->FirstChildElement(); e != NULL; e = e->NextSiblingElement()) {
		//If element value is objectgroup or layer
		if (e->Value() == std::string("objectgroup") || e->Value() == std::string("layer")) {
			//If value of grandchild is object
			if (e->FirstChildElement()->Value() == std::string("object")) {
				//Parse object layer
				parseObjectLayer(e, pLevel->getLayers(), pLevel);
			}
			//If value of grandchild is data or (grandchild is last and next sibling value is data)
			else if (e->FirstChildElement()->Value() == std::string("data") || (e->FirstChildElement()->NextSiblingElement() != 0 && e->FirstChildElement()->NextSiblingElement()->Value() == std::string("data"))) {
				//Parse tile layer
				parseTileLayer(e, pLevel->getLayers(), pLevel->getTilesets(), pLevel->getCollisionLayers(), pLevel->getFluidLayers(), pLevel->getHazardLayers());
			}
		}
	}

	//Return parsed level
	return pLevel;
}

//Parse textures
void LevelParser::parseTextures(tinyxml2::XMLElement* pTextureRoot) {
	//Output
	std::cout << "Adding texture: " << pTextureRoot->Attribute("value") << " with ID: " << pTextureRoot->Attribute("name") << std::endl;

	//Load texture
	TheTextureManager::Instance()->load(pTextureRoot->Attribute("value"), pTextureRoot->Attribute("name"), TheGame::Instance()->getRenderer());
}

//Parse tilesets
void LevelParser::parseTilesets(std::string tilesetFile, int firstGid, std::vector<Tileset>* pTilesets) {
	//Create assets path
	std::string assetsPath = "assets/";
	//Create maps path
	std::string mapsPath = "maps/";
	//Create new maps path
	std::string mapsPath2 = "maps.bmna/";
	std::cout << "****************TILESET FILE NAME 1: " << tilesetFile << "\n";
	//Create tileset file path
	tilesetFile = assetsPath + mapsPath + tilesetFile;
	std::cout << "****************TILESET FILE NAME 2: " << tilesetFile << "\n";
	//Convert file string to const char*
	const char* charTilesetFile = tilesetFile.c_str();

	//Create XML document
	tinyxml2::XMLDocument TilesetDoc;
	//Load tileset file to document
	TilesetDoc.LoadFile(charTilesetFile);

	//Set tileset element
	tinyxml2::XMLElement* pTilesetRoot = TilesetDoc.RootElement();

	//Set image element
	tinyxml2::XMLElement* pImageRoot = pTilesetRoot->FirstChildElement();
	//Get image path
	std::string tilesetImage = std::string(pImageRoot->Attribute("source"));

	std::cout << "****************TILESET FILE PATH: " << assetsPath + mapsPath2 + std::string(pImageRoot->Attribute("source")) << "\n";
	//Load texture
	TheTextureManager::Instance()->load((assetsPath + mapsPath2 + std::string(pImageRoot->Attribute("source"))), pTilesetRoot->Attribute("name"), TheGame::Instance()->getRenderer());

	//Create a tileset
	Tileset tileset;
	
	//Set tileset values
	pTilesetRoot->QueryIntAttribute("width", &tileset.width);
	pTilesetRoot->QueryIntAttribute("height", &tileset.height);
	tileset.firstGridID = firstGid;
	pTilesetRoot->QueryIntAttribute("tilewidth", &tileset.tileWidth);
	pTilesetRoot->QueryIntAttribute("tileheight", &tileset.tileHeight);
	pTilesetRoot->QueryIntAttribute("spacing", &tileset.spacing);
	pTilesetRoot->QueryIntAttribute("margin", &tileset.margin);
	pTilesetRoot->QueryIntAttribute("columns", &tileset.numColumns);
	tileset.name = pTilesetRoot->Attribute("name");
	
	//Add tileset to vector
	pTilesets->push_back(tileset);
}

//Parse object layers
void LevelParser::parseObjectLayer(tinyxml2::XMLElement* pObjectElement, std::vector<Layer*>* pLayers, Level* pLevel) {
	//Create an object layer
	ObjectLayer* pObjectLayer = new ObjectLayer();

	//For each child in object layer element
	for (tinyxml2::XMLElement* e = pObjectElement->FirstChildElement(); e != NULL; e = e->NextSiblingElement()) {
		//If element value is object
		if (e->Value() == std::string("object")) {
			//Create element variables
			int x, y, width, height, numFrames, callbackID = 0, animSpeed = 1;
			std::string textureID;
			std::string type;

			//Set x, y, type to element's x, y, type values
			e->QueryIntAttribute("x", &x);
			e->QueryIntAttribute("y", &y);
			type = e->Attribute("type");

			//Create game object
			GameObject* pGameObject = TheGameObjectFactory::Instance()->create(type);

			//For each child element in game object element
			for (tinyxml2::XMLElement* properties = e->FirstChildElement(); properties != NULL; properties = properties->NextSiblingElement()) {
				//If element value is property
				if (properties->Value() == std::string("properties")) {
					//For each child element in properties element
					for (tinyxml2::XMLElement* property = properties->FirstChildElement(); property != NULL; property = property->NextSiblingElement()) {
						//If element value is property
						if (property->Value() == std::string("property")) {
							//If element name attribute is numFrames
							if (property->Attribute("name") == std::string("numFrames")) {
								//Set numFrames
								property->QueryIntAttribute("value", &numFrames);
							}
							//If element name attribute is textureHeight
							else if (property->Attribute("name") == std::string("textureHeight")) {
								//Set height
								property->QueryIntAttribute("value", &height);
							}
							//If element name attribute is textureID
							else if (property->Attribute("name") == std::string("textureID")) {
								//Set textureID
								textureID = property->Attribute("value");
							}
							//If element name attribute is textureWidth
							else if (property->Attribute("name") == std::string("textureWidth")) {
								//Set textureWidth
								property->QueryIntAttribute("value", &width);
							}
							//If element name attribute is callbackID
							else if (property->Attribute("name") == std::string("callbackID")) {
								//Set callbackID
								property->QueryIntAttribute("value", &callbackID);
							}
							//If element name attribute is animSpeed
							else if (property->Attribute("name") == std::string("animSpeed")) {
								//Set animSpeed
								property->QueryIntAttribute("value", &animSpeed);
							}
						}
					}
				}
			}

			//Load game object
			pGameObject->load(std::unique_ptr<LoaderParams>(new LoaderParams(x, y, width, height, textureID, numFrames, callbackID, animSpeed)));

			//Set layers for object
			pGameObject->setCollisionLayers(pLevel->getCollisionLayers());
			pGameObject->setFluidLayers(pLevel->getFluidLayers());
			pGameObject->setHazardLayers(pLevel->getHazardLayers());

			//If game object is player
			if (type == "Player") {
				//Set player for objects
				m_pPlayer = pGameObject;

				//Set player for level
				pLevel->setPlayer(dynamic_cast<Player*>(pGameObject));
			}

			//Add game object to object layer vector
			pObjectLayer->getGameObjects()->push_back(pGameObject);
		}
	}

	//Set player acess for objects
	pObjectLayer->loadPlayerAccess(m_pPlayer);

	//Add object layer to layer vector
	pLayers->push_back(pObjectLayer);
}

//Parse tile layers
void LevelParser::parseTileLayer(tinyxml2::XMLElement* pTileElement, std::vector<Layer*>* pLayers, const std::vector<Tileset>* pTilesets, std::vector<TileLayer*>* pCollisionLayers, std::vector<TileLayer*>* pFluidLayers, std::vector<TileLayer*>* pHazardLayers) {
	//Create a tile layer
	TileLayer* pTileLayer = new TileLayer(m_tileSize, m_width, m_height, *pTilesets);

	//Initialize property variables

	bool collidable = false;
	bool fluid = false;
	bool hazard = false;

	//String to hold string of IDs
	std::string ids;
	//2D vector to hold spliced IDs as integers
	std::vector<std::vector<int>> data;

	//For each child in tile element
	for (tinyxml2::XMLElement* e = pTileElement->FirstChildElement(); e != NULL; e = e->NextSiblingElement()) {
		//If element value is properties
		if (e->Value() == std::string("properties")) {
			//For each child in properties element
			for (tinyxml2::XMLElement* property = e->FirstChildElement(); property != NULL; property = property->NextSiblingElement()) {
				//If element value is property
				if (property->Value() == std::string("property")) {
					//If property name attribute is collidable
					if (property->Attribute("name") == std::string("collidable")) {
						//Set as collidable
						collidable = true;
					}
					//If property name attribute is fluid
					else if (property->Attribute("name") == std::string("fluid")) {
						fluid = true;
					}
					//If property name attribute is hazard
					else if (property->Attribute("name") == std::string("hazard")) {
						hazard = true;
					}
				}
			}
		}

		//If element value is data
		if (e->Value() == std::string("data")) {
			//Set string of IDs
			ids = e->GetText();

			//Splice IDs and set to 2D vector of integers
			data = spliceData(ids, m_width, m_height);
		}
		
		//Set tile layer
		pTileLayer->setTileIDs(data);

		//If layer is collidable
		if (collidable) {
			//Add to collision layers vector
			pCollisionLayers->push_back(pTileLayer);
		}
		//If layer is fluid
		else if (fluid) {
			pFluidLayers->push_back(pTileLayer);
		}
		//If layer is hazard
		else if (hazard) {
			pHazardLayers->push_back(pTileLayer);
		}

		//Add tile layer into layers vector
		pLayers->push_back(pTileLayer);
	}
}

//Parse string of IDs
std::vector<std::vector<int>> LevelParser::spliceData(std::string strData, int width, int height) {
	//Declare variables
	int x;
	char c;
	int count = 0;
	std::string strNum;
	std::vector<int> hold;
	std::vector<std::vector<int>> data;

	//For each character in string
	for (int i = 0; i < strData.length(); i++) {
		//Set character
		c = strData[i];
		//Add character to string number
		strNum += c;

		//If character is a comma
		if (c == ',') {
			//Increment number counter
			count++;
			//Remove comma from string
			strNum.pop_back();
			//Convert string to number
			x = stoi(strNum);
			//Put number in hold vector
			hold.push_back(x);
			//Clear the string number
			strNum.clear();

			//If at end of line or at last number
			if (count % width == 0 || count == (width * height) - 1) {
				//If at last number
				if (count == (width * height) - 1) {
					//Save one spot over from current spot in string to skip comma
					int z = ++i;

					//For each character in last number
					for (int j = z; j < strData.length(); j++) {
						//Set character
						c = strData[i];
						//Add character to string number
						strNum += c;
						//Increment
						i++;
					}
					
					//Convert string to number
					x = stoi(strNum);
					//Put number in hold vector
					hold.push_back(x);
				}

				//Put hold vector in data vector
				data.push_back(hold);
				//Clear hold vector
				hold.clear();
			}
		}
	}

	//Return data vector
	return data;
}