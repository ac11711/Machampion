#include <cstdlib>

//A function to generate a random number
int Random(int range = 100) {
	//Return number
	return (rand() % range) + 1;
}

//A function to test if a chance was met eg 50% chance
bool Probability(int chance) {
	//If random number is within chance
	if (((rand() % 100) + 1) <= chance) {
		return true;
	}

	return false;
}