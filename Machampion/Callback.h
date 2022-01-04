#pragma once

//A class that allows us to go to different game states
class Callback {
public:

	virtual void call() = 0;
};

class StaticCallback : public Callback {
public:

	//Constructor
	StaticCallback(void (*function)()) {
		m_function = function;
	}

	//Call the function
	virtual void call() {
		m_function();
	}

private:

	//Function member variable
	void (*m_function)();
};