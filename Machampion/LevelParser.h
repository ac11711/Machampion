#pragma once
#include "tinyxml2.h"
#include "GameObject.h"
#include <iostream>
#include <vector>
#include <memory>

class Layer;
struct Tileset;
class TileLayer;
class Level;

//A class that allows us to read a level file
class LevelParser {
public:

	Level* parseLevel(const char* levelFile);

private:

	//		---Member Functions---

	void parseTextures(tinyxml2::XMLElement* pTextureRoot);
	void parseTilesets(std::string tilesetFile, int firsGid, std::vector<Tileset>* pTilesets);
	void parseObjectLayer(tinyxml2::XMLElement* pObjectElement, std::vector<Layer*>* pLayers, Level* pLevel);
	void parseTileLayer(tinyxml2::XMLElement* pTileElement, std::vector<Layer*>* pLayers, const std::vector<Tileset>* pTilesets, std::vector<TileLayer*>* pCollisionLayers, std::vector<TileLayer*>* pFluidLayers, std::vector<TileLayer*>* pHazardLayers);

	std::vector<std::vector<int>> spliceData(const std::string& strData, int width, int height);

	//		---Memeber Variables---

	const char* m_tilesetFile;
	int m_firstGid;
	int m_tileSize;
	int m_width;
	int m_height;

	//Player object
	GameObject* m_pPlayer;
};