#include "TileLayer.h"
#include "TextureManager.h"
#include "InputHandler.h"
#include "Camera.h"
#include "Game.h"

//Constructor that initializes variables
TileLayer::TileLayer(int tileSize, int mapWidth, int mapHeight, const std::vector<Tileset>& tilesets) :
	m_tileSize(tileSize),
	m_tilesets(tilesets),
	m_position(0, 0),
	m_velocity(0, 0)
{
	m_numColumns = mapWidth;
	m_numRows = mapHeight;
	m_mapWidth = mapWidth;
}

//Render the tile layer
void TileLayer::render() {
	//Declare position variables
	int x, y, x2, y2 = 0;

	//Set x and y values
	x = m_position.getX() / m_tileSize;
	y = m_position.getY() / m_tileSize;

	//Set x2 and y2 values
	x2 = int(m_position.getX()) % m_tileSize;
	y2 = int(m_position.getY()) % m_tileSize;

	//For each row
	for (int i = 0; i < m_numRows; i++) {
		//For each column
		for (int j = 0; j < m_numColumns; j++) {
			//Set ID
			int id = m_tileIDs[i + y][j + x];

			//If ID is first
			if (id == 0) {
				//Do nothing
				continue;
			}

			//If to the left of camera position or more than game width
			if (((j * m_tileSize) - x2) - TheCamera::Instance()->getPosition().m_x < -m_tileSize || ((j * m_tileSize) - x2) - TheCamera::Instance()->getPosition().m_x > TheGame::Instance()->getGameWidth()) {
				//Do nothing
				continue;
			}

			//Set tileset
			Tileset tileset = getTilesetByID(id);

			id--;

			//Draw tile
			TheTextureManager::Instance()->drawTile(tileset.name, tileset.margin, tileset.spacing, ((j * m_tileSize) - x2) - TheCamera::Instance()->getPosition().m_x, ((i * m_tileSize) - y2), m_tileSize, m_tileSize, (id - (tileset.firstGridID - 1)) / tileset.numColumns, (id - (tileset.firstGridID - 1)) % tileset.numColumns, TheGame::Instance()->getRenderer());
		}
	}
}

//Update the tile layer
void TileLayer::update(Level* pLevel) {

}

//Get tileset
Tileset TileLayer::getTilesetByID(int tileID) {
	//For each tileset
	for (int i = 0; i < m_tilesets.size(); i++) {
		//If tileset + 1 is less than or equal to tileset size - 1
		if (i + 1 <= m_tilesets.size() - 1) {
			//If tile ID is >= tileset's first grid ID and tileset < next tileset's first grid ID
			if (tileID >= m_tilesets[i].firstGridID && tileID < m_tilesets[i + 1].firstGridID) {
				//Return current tileset
				return m_tilesets[i];
			}
		}
		else {
			return m_tilesets[i];
		}
	}

	std::cout << "Did not find tileset, returning empty tileset\n";
	Tileset t;
	return t;
}