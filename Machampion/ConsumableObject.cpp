#include "ConsumableObject.h"
#include "Game.h"

//Load the consumable object
void ConsumableObject::load(std::unique_ptr<LoaderParams> const& pParams) {
	//Define variables using loader params
	m_position = Vector2D(pParams->getX(), pParams->getY());
	m_width = pParams->getWidth();
	m_height = pParams->getHeight();
	m_textureID = pParams->getTextureID();
	m_numFrames = pParams->getNumFrames();
}

//Draw the consumable object
void ConsumableObject::draw() {
	//Draw using texture manager
	TheTextureManager::Instance()->drawFrame(m_textureID, (Uint32)m_position.getX() - TheCamera::Instance()->getPosition().m_x, (Uint32)m_position.getY() - TheCamera::Instance()->getPosition().m_y, m_width, m_height, m_currentRow, m_currentFrame, TheGame::Instance()->getRenderer(), m_angle, m_alpha);
}

//Update the consumable object
void ConsumableObject::update() {
	//Animate
	m_currentFrame = int(((SDL_GetTicks() / (20)) % m_numFrames));

	//If dying
	if (m_bDying) {
		//Kill
		m_bDead = true;
	}
}