#pragma once

class Level;

//A class that allows us to create a foundation for layers
class Layer {
public:

	//Destructor
	virtual ~Layer() {}

	//Management functions

	virtual void render() = 0;
	virtual void update(Level* pLevel) = 0;
};