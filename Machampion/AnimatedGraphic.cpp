#include "AnimatedGraphic.h"

//Constructor
AnimatedGraphic::AnimatedGraphic() : PlatformerObject(),
	m_animSpeed(1),
	m_frameCount(0)
{}

//Load the animated graphic
void AnimatedGraphic::load(std::unique_ptr<LoaderParams> const& pParams) {
	//Load using base class
	PlatformerObject::load(std::move(pParams));

	//Set animation speed
	m_animSpeed = pParams->getAnimSpeed();
}

//Draw the animated graphic
void AnimatedGraphic::draw() {
	//Draw using base class
	PlatformerObject::draw();
}

//Update the animated graphic to change frames
void AnimatedGraphic::update() {
	//If animSpeed and numFrames is more than 0
	if (m_animSpeed > 0 && m_numFrames > 0) {
		//Set current frame
		m_currentFrame = int(((SDL_GetTicks() / (1000 / m_animSpeed)) % m_numFrames));
	}
	else {
		//Display error
		std::cout << "Error: Devide by 0 - " << "Speed: " << m_animSpeed << " Frames: " << m_numFrames << std::endl;
		
		//Set current frame to first frame
		m_currentFrame = 0;
	}
}

//Clean the animated graphic
void AnimatedGraphic::clean() {
	//Clean using base class
	PlatformerObject::clean();
}