#include <random>
#include <ctime>

//Initialize random
std::default_random_engine randomEngine(time(nullptr));
//Set seed
std::uniform_int_distribution<int> seed(1, 100);

//A function to generate a random number
int Random(int range = 100) {
	//If range is not 100
	if (range != 100) {
		//Set seed
		std::uniform_int_distribution<int> seed(1, range);
	}
	
	//Get random number
	int rNum = seed(randomEngine);

	//If range is not 100
	if (range != 100) {
		//Reset seed
		std::uniform_int_distribution<int> seed(1, 100);
	}

	//Return number
	return rNum;
}

//A function to test if a chance was met eg 50% chance
bool Probability(int chance) {
	//If random number is within chance
	if (seed(randomEngine) <= chance) {
		return true;
	}

	return false;
}