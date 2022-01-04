#include "Level.h"
#include "TextureManager.h"
#include "Layer.h"
#include "TileLayer.h"
#include "Game.h"
#include <iostream>
#include <math.h>

//Constructor
Level::Level() :
	m_pPlayer(nullptr)
{}

//Destructor
Level::~Level() {
	//For each layer
	for (int i = 0; i < m_layers.size(); i++) {
		//Delete pointer
		delete m_layers[i];
	}

	//Clear vector
	m_layers.clear();
}

//Render the layers
void Level::render() {
	//For each layer
	for (int i = 0; i < m_layers.size(); i++) {
		//Draw layer
		m_layers[i]->render();
	}
}

//Update the layers
void Level::update() {
	//For each layer
	for (int i = 0; i < m_layers.size(); i++) {
		//Update layer
		m_layers[i]->update(this);
	}
}