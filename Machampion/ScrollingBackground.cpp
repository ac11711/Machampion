#include "ScrollingBackground.h"
#include "TextureManager.h"
#include "Game.h"

//Constructor
ScrollingBackground::ScrollingBackground() : PlatformerObject(),
	m_count(0),
	m_maxCount(5),
	m_scrollSpeed(0)
{}

//Load object and rectangles
void ScrollingBackground::load(std::unique_ptr<LoaderParams> const& pParams) {
	//Load using base class
	PlatformerObject::load(std::move(pParams));

	//Set scroll speed
	m_scrollSpeed = pParams->getAnimSpeed();

	//Set src and dest rect 1 position
	m_srcRect1.x = 0;
	m_srcRect1.y = 0;
	m_destRect1.x = m_position.getX();
	m_destRect1.y = m_position.getY();

	//Set src and dest rect 2 position
	m_srcRect2.x = 0;
	m_srcRect2.y = 0;
	m_destRect2.x = m_position.getX() + m_width;
	m_destRect2.y = m_position.getY();

	//Set src and dest rect 1 dimensions
	m_srcRect1.w = m_destRect1.w = m_srcRect2Width = m_destRect1Width = m_width;
	m_srcRect1.h = m_destRect1.h = m_height;

	//Set src and dest rect 2 dimensions
	m_srcRect2.w = m_destRect2.w = m_srcRect2Width = m_destRect2Width = 0;
	m_srcRect2.h = m_destRect2.h = m_height;
}

//Draw scrolling background
void ScrollingBackground::draw() {
	//Draw first rectangle
	SDL_RenderCopyEx(TheGame::Instance()->getRenderer(), TheTextureManager::Instance()->getTextureMap()[m_textureID], &m_srcRect1, &m_destRect1, 0, 0, SDL_FLIP_NONE);

	//Draw second rectangle
	SDL_RenderCopyEx(TheGame::Instance()->getRenderer(), TheTextureManager::Instance()->getTextureMap()[m_textureID], &m_srcRect2, &m_destRect2, 0, 0, SDL_FLIP_NONE);
}

//Update background to move
void ScrollingBackground::update() {
	//If max count has been reached
	if (m_count == m_maxCount) {
		//Make first rectangle smaller
		m_srcRect1.x += m_scrollSpeed;
		m_srcRect1.w -= m_scrollSpeed;
		m_destRect1.w -= m_scrollSpeed;

		//Make second rectangle bigger
		m_srcRect2.w += m_scrollSpeed;
		m_destRect2.w += m_scrollSpeed;
		m_destRect2.x -= m_scrollSpeed;

		//Second rect has reached left of screen ; start over
		if (m_destRect2.w >= m_width) {
			//Reset src and dest rect 1 position
			m_srcRect1.x = 0;
			m_srcRect1.y = 0;
			m_destRect1.x = m_position.getX();
			m_destRect1.y = m_position.getY();

			//Reset src and dest rect 2 position
			m_srcRect2.x = 0;
			m_srcRect2.y = 0;
			m_destRect2.x = m_position.getX() + m_width;
			m_destRect2.y = m_position.getY();

			//Reset src and dest rect 1 dimensions
			m_srcRect1.w = m_destRect1.w = m_srcRect2Width = m_destRect1Width = m_width;
			m_srcRect1.h = m_destRect1.h = m_height;

			//Reset src and dest rect 2 dimensions
			m_srcRect2.w = m_destRect2.w = m_srcRect2Width = m_destRect2Width = 0;
			m_srcRect2.h = m_destRect2.h = m_height;
		}
		//Reset counter
		m_count = 0;
	}
	//Increment counter
	m_count++;
}

//Clean object
void ScrollingBackground::clean() {
	//Clean using base class
	PlatformerObject::clean();
}