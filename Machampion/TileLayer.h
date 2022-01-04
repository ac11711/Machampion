#pragma once
#include "Layer.h"
#include "Level.h"
#include "Vector2D.h"
#include <iostream>
#include <vector>

//A class that allows us to create a layer of tiles
class TileLayer : public Layer {
public:

	//Constructor/Destructor

	TileLayer(int tileSize, int mapWidth, int mapHeight, const std::vector<Tileset>& tilesets);
	~TileLayer() {}

	//Management functions

	virtual void render();
	virtual void update(Level* pLevel);

	//Setters

	void setTileIDs(const std::vector<std::vector<int>>& data) { m_tileIDs = data; }
	void setTileSize(int tileSize) { m_tileSize = tileSize; }
	void setMapWidth(int mapWidth) { m_mapWidth = mapWidth; }
	void setPosition(Vector2D position) { m_position = position; }

	//Getters

	Tileset getTilesetByID(int tileID);
	int getTileSize() { return m_tileSize; }
	int getMapWidth() { return m_mapWidth; }
	Vector2D getPosition() { return m_position; }
	std::vector<std::vector<int>>& getTileIDs() { return m_tileIDs; }

private:

	int m_numColumns;
	int m_numRows;
	int m_tileSize;
	int m_mapWidth;

	Vector2D m_position;
	Vector2D m_velocity;
	Vector2D m_acceleration;

	const std::vector<Tileset>& m_tilesets;

	std::vector<std::vector<int>> m_tileIDs;
};