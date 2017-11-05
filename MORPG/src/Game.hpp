#pragma once

#include <SFML/System.hpp>
#include "MessageSystem.hpp"
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
	MessageSystem m_messageSystem;
	Window m_window;
	Input m_inputSystem;

	sf::Clock m_clock;
	sf::Time m_elapsed;
};