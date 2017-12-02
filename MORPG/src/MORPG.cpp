#include <iostream>
#include "Game.hpp"

int main() {
	std::cout << "Welcome to the next World of Wacraft!" << std::endl;
	std::cout << "Introduce a username: ";
	std::fflush(stdout);
	std::string username;
	int i = 0;
	while (std::cin.peek() != '\n') {
		std::cin >> username;
	}
	printf("\nWelcome %s!\n\n\n", username.c_str());

	Game game(username);
	game.Play();

	return 0;
}

