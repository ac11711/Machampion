#pragma once
#include <string>
#include <vector>

//A class that allows us to set up a foundation for game states
class GameState {
public:

	//Destructor
	~GameState() {}

	//Management functions

	virtual void render() = 0;
	virtual void update() = 0;

	virtual bool onEnter() = 0;
	virtual bool onExit() = 0;

	virtual void resume() {}

	//Getters
	virtual std::string getStateID() const = 0;

protected:

	//Constructor
	GameState() : 
		m_loadingComplete(false),
		m_exiting(false) 
	{}
	
	bool m_loadingComplete;
	bool m_exiting;

	//Vector that holds texture IDs
	std::vector<std::string> m_textureIDList;
};