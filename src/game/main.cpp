#include "engine/GameEngine.h"
#include <stdexcept>
#include <iostream>


int main() {
	try {
		GameEngine gameEngine;
		gameEngine.run();
	}
	catch (std::exception& e) {
		std::cout << e.what() << std::endl;
	}

	return 0;
}