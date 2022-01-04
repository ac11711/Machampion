#pragma once
#include <iostream>
#include "GameObjectFactory.h"
#include "PlatformerObject.h"

//A class that allows us to create a moving background in game
class ScrollingBackground : public PlatformerObject {
public:

	//Constructor/Destructor

	ScrollingBackground();
	virtual ~ScrollingBackground() {}

	//Management functions

	virtual void load(std::unique_ptr<LoaderParams> const &pParams);
	virtual void draw();
	virtual void update();
	virtual void clean();

private:

	int m_scrollSpeed;

	//Counters

	int m_count;
	int m_maxCount;

	//Rectangle 1 variables

	SDL_Rect m_srcRect1;
	SDL_Rect m_srcRect2;
	int m_srcRect1Width;
	int m_srcRect2Width;

	//Rectangle 2 variables

	SDL_Rect m_destRect1;
	SDL_Rect m_destRect2;
	int m_destRect1Width;
	int m_destRect2Width;
};

//A class that creates a scrolling background
class ScrollingBackgroundCreator : public BaseCreator {
public:

	virtual GameObject* createGameObject() const {
		return new ScrollingBackground();
	}
};