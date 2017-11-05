#include "Game.hpp"
#include "Definitions.hpp"

Game::Game() : m_window(&m_messageSystem, WINDOW_TITLE, sf::Vector2u(WINDOW_DEFAULT_WIDTH, WINDOW_DEFAULT_HEIGHT)){

}

Game::~Game() {}

void Game::Play() {
	while (!m_window.IsDone()) {
		Update();
		Render();
		LateUpdate();
	}
}

sf::Time Game::GetElapsedTime() {
	return m_elapsed;
}

void Game::Update() {
	m_window.Update();
}

void Game::Render() {
	m_window.BeginDraw();
	// Draw things
	m_window.EndDraw();
}

void Game::LateUpdate() {
	RestartClock();
}

void Game::RestartClock() {
	m_elapsed = m_clock.restart();
}


