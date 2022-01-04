#pragma once
#include "CollisionManager.h"
#include "Player.h"
#include "Layer.h"
#include "LevelParser.h"
#include <iostream>
#include <vector>

class TileLayer;

struct Tileset {
	int firstGridID;
	int tileWidth;
	int tileHeight;
	int spacing;
	int margin;
	int width;
	int height;
	int numColumns;
	std::string name;
};

//A class that allows us to display and manage a level
class Level {
public:

	//Destructor
	~Level();

	//Management functions

	void render();
	void update();

	//Setters

	void setPlayer(Player* pPlayer) { m_pPlayer = pPlayer; }

	//Getters

	Player* getPlayer() { return m_pPlayer; }
	std::vector<Layer*>* getLayers() { return &m_layers; }
	std::vector<Tileset>* getTilesets() { return &m_tilesets; }
	std::vector<TileLayer*>* getCollisionLayers() { return &m_collisionLayers; }
	std::vector<TileLayer*>* getFluidLayers() { return &m_fluidLayers; }
	std::vector<TileLayer*>* getHazardLayers() { return &m_hazardLayers; }

private:

	//Constructor
	Level();

	//Friend
	friend class LevelParser;

	//Player
	Player* m_pPlayer;

	//Layers

	std::vector<Layer*> m_layers;
	std::vector<Tileset> m_tilesets;
	std::vector<TileLayer*> m_collisionLayers;
	std::vector<TileLayer*> m_fluidLayers;
	std::vector<TileLayer*> m_hazardLayers;
};