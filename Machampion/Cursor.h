#pragma once
#include "GameObjectFactory.h"
#include "PlatformerObject.h"

//A class that allows us to navigate the menus with a joystick
class Cursor : public PlatformerObject {
public:

	//Ensures only one instance
	static Cursor* Instance() {
		if (s_pInstance == 0) {
			s_pInstance = new Cursor();
		}

		return s_pInstance;
	}

	//Management functions

	virtual void load(std::string filename, std::string id, int x, int y, int width, int height, int numFrames);
	virtual void draw();
	virtual void update();
	virtual void clean();

	//Getters

	bool isClicked() { return m_clicked; }

private:


	//Constructor/Destructor

	Cursor();
	~Cursor() {}

	//Handlers

	void handleInput();
	void handleMovement(Vector2D velocity);
	void handleAnimation();

	//Instance
	static Cursor* s_pInstance;

	bool m_bMoveUp;
	bool m_bMoveDown;

	bool m_clicked;
};

typedef Cursor TheCursor;