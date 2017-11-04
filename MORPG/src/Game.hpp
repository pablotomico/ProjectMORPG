#pragma once

#include <SFML/System.hpp>
#include "Window.hpp"


class Game {

public:
	Game();
	~Game();

	void Play();

	sf::Time GetElapsedTime();

private:
	void Update();
	void Render();
	void LateUpdate();

	void RestartClock();

private:
	Window m_window;

	sf::Clock m_clock;
	sf::Time m_elapsed;
};