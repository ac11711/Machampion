#pragma once
#include "LoaderParams.h"
#include "Vector2D.h"
#include <string>
#include <vector>
#include <memory>

class TileLayer;

//Allows us to create a blueprint for our game objects
class GameObject {
public:

	//Destructor
	virtual ~GameObject() {}

	//Management functions

	virtual void load(std::unique_ptr<LoaderParams> const& pParams) = 0;
	virtual void draw() = 0;
	virtual void update() = 0;
	virtual void clean() = 0;

	//Handle collisions
	virtual void collision(std::string fling = "none") = 0;

	//Getters

	int getWidth() { return m_width; }
	int getHeight() { return m_height; }
	virtual std::string type() = 0;
	std::string getTextureID() { return m_textureID; }
	Vector2D& getPosition() { return m_position; }
	Vector2D& getVelocity() { return m_velocity; }
	bool updating() { return m_bUpdating; }
	bool startedUpdating() { return m_bStartedUpdating; }
	std::vector<TileLayer*>* getCollisionLayers() { return m_pCollisionLayers; }
	std::vector<TileLayer*>* getFluidLayers() { return m_pFluidLayers; }
	bool dead() { return m_bDead; }
	bool dying() { return m_bDying; }

	//Setters

	void setUpdating(bool updating) { m_bUpdating = updating; }
	void setStartedUpdating(bool startedUpdating) { m_bStartedUpdating = startedUpdating; }
	void setCollisionLayers(std::vector<TileLayer*>* layers) { m_pCollisionLayers = layers; }
	void setFluidLayers(std::vector<TileLayer*>* layers) { m_pFluidLayers = layers; }
	void setHazardLayers(std::vector<TileLayer*>* layers) { m_pHazardLayers = layers; }
	void setPlayer(GameObject* pPlayer) { m_pPlayer = pPlayer; }

protected:

	//Constructor with default initialization
	GameObject() :
		m_width(0),
		m_height(0),
		m_currentRow(0),
		m_currentFrame(0),
		m_numFrames(0),
		m_position(0, 0),
		m_velocity(0, 0),
		m_acceleration(0, 0),
		m_bUpdating(false),
		m_bStartedUpdating(false),
		m_bDead(false),
		m_bDying(false),
		m_angle(0),
		m_alpha(255),
		m_pPlayer(nullptr),
		m_pCollisionLayers(nullptr),
		m_pFluidLayers(nullptr),
		m_pHazardLayers(nullptr)
	{}
	
	//Size variables

	int m_width;
	int m_height;

	//Animation variables

	int m_currentRow;
	int m_currentFrame;
	int m_numFrames;
	std::string m_textureID;

	//Movement variables

	Vector2D m_position;
	Vector2D m_velocity;
	Vector2D m_acceleration;

	//State variables

	bool m_bUpdating;
	bool m_bStartedUpdating;
	bool m_bDead;
	bool m_bDying;

	//Rotation
	double m_angle;

	//Blending
	int m_alpha;

	//Collision
	std::string m_fling;
	std::vector<TileLayer*>* m_pCollisionLayers;
	std::vector<TileLayer*>* m_pFluidLayers;
	std::vector<TileLayer*>* m_pHazardLayers;

	//Player access
	GameObject* m_pPlayer;
};