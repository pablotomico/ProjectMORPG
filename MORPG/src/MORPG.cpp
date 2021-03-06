#include <iostream>
#include "Game.hpp"
#include "Util\Utilities.hpp"


int main() {
	LOG("Welcome to the next World of Wacraft!");
	LOG("Introduce a username: ");
	std::fflush(stdout);
	std::string username;
	int i = 0;
	while (std::cin.peek() != '\n') {
		std::cin >> username;
	}
	LOG("\Hello " << username.c_str() << "!\n\n");

	Game game(username);
	game.Play();

	return 0;
}

