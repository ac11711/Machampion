#pragma once
#include "PlatformerObject.h"
#include <iostream>

//A class that allows us to create enemies
class Enemy : public PlatformerObject {
public:

	//Getters

	virtual std::string type() { return "Enemy"; }

protected:

	//Constructor
	Enemy() : PlatformerObject(),
		m_health(0)
	{}

	//Destructor
	virtual ~Enemy() {}

	//		---Member variables---

	int m_health;
};