#pragma once
#include "Vector2D.h"
#include <iostream>

//A class that allows us to draw and follow the target
class Camera {
public:

	//Ensures only one instance
	static Camera* Instance() {
		if (s_pInstance == 0) {
			s_pInstance = new Camera();
		}

		return s_pInstance;
	}

	//Update the camera
	void update(Vector2D velocity);

	//Setters

	void setTarget(Vector2D* target) { m_pTarget = target; }
	void setPosition(const Vector2D& position) { m_position = position; }

	//Getters

	const Vector2D getPosition() const;

private:

	//Constructor/Destructor

	Camera();
	~Camera();

	//Instance
	static Camera* s_pInstance;

	//Cameras position
	Vector2D m_position;
	//Cameras target
	Vector2D* m_pTarget;
};

typedef Camera TheCamera;