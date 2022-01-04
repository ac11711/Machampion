#include "MenuButton.h"
#include "InputHandler.h"
#include "Cursor.h"

//Constructor
MenuButton::MenuButton() : PlatformerObject(), 
	m_callback(0),
	m_callbackID(0),
	m_bReleased(true)
{}

//Load the menu button
void MenuButton::load(std::unique_ptr<LoaderParams> const& pParams) {
	//Load using base class
	PlatformerObject::load(std::move(pParams));
	//Set callback ID
	m_callbackID = pParams->getCallbackID();
	//Set current frame
	m_currentFrame = MOUSE_OUT;
}

//Draw the menu button
void MenuButton::draw() {
	//Draw using base class
	PlatformerObject::draw();
}

//Update the menu button
void MenuButton::update() {
	//Set mouse x and y position
	float mouseXpos = TheInputHandler::Instance()->getMouseX();
	float mouseYpos = TheInputHandler::Instance()->getMouseY();

	//If mouse position is inside menu button
	if (mouseXpos < (m_position.getX() + m_width)			//Mouse x position is less than position of the right edge of the button (xpos of button + width of button)
		&& mouseXpos > m_position.getX()					//Mouse x position is greater than the left edge of the button
		&& mouseYpos < (m_position.getY() + m_height)		//Mouse y position is less than position of the top edge of the button (ypos of button + height of button)
		&& mouseYpos > m_position.getY()) {					//Mouse y position is greater than the bottom edge of the button

		//If left mouse button is clicked and button was button was previously released
		if ((TheInputHandler::Instance()->getMouseButtonState(LEFT) && m_bReleased)) {
			//Set button frame to clicked
			m_currentFrame = CLICKED;

			//If callback is not empty
			if (m_callback != 0) {
				//Callback
				m_callback();
			}

			//Click not released
			m_bReleased = false;
		}
		//If left mouse button state is not clicked
		else if (!TheInputHandler::Instance()->getMouseButtonState(LEFT)) {
			//Click released
			m_bReleased = true;
			//Set button frame to mouse over 
			m_currentFrame = MOUSE_OVER;
		}
	}
	//If mouse position is outside menu button
	else {
		//Set button frame to mouse out
		m_currentFrame = MOUSE_OUT;
	}

	//If cursor position is inside menu button
	if ((TheCursor::Instance()->getPosition().m_x + TheCursor::Instance()->getWidth() / 2) < (m_position.getX() + m_width)			//Cursor x position is less than position of the right edge of the button (xpos of button + width of button)
		&& (TheCursor::Instance()->getPosition().m_x + TheCursor::Instance()->getWidth() / 2) > m_position.getX()					//Cursor x position is greater than the left edge of the button
		&& (TheCursor::Instance()->getPosition().m_y + TheCursor::Instance()->getHeight() / 2) < (m_position.getY() + m_height)		//Cursor y position is less than position of the top edge of the button (ypos of button + height of button)
		&& (TheCursor::Instance()->getPosition().m_y + TheCursor::Instance()->getHeight() / 2) > m_position.getY()) {				//Cursor y position is greater than the bottom edge of the button

		//If cursor is clicked
		if (TheCursor::Instance()->isClicked()) {
			//Set button frame to clicked
			m_currentFrame = CLICKED;

			//If callback is not empty
			if (m_callback != 0) {
				//Callback
				m_callback();
			}
		}
		//If cursor is not clicked
		else if (!TheCursor::Instance()->isClicked()) {
			//Set button frame to mouse over 
			m_currentFrame = MOUSE_OVER;
		}
	}
}

//Clean the button
void MenuButton::clean() {
	//Clean using base class
	PlatformerObject::clean();
}