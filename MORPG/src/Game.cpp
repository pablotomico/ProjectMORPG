#include "Game.hpp"
#include "Definitions.hpp"
#include "Input.hpp"

Game::Game()
	: m_inputSystem(&m_messageSystem)
	, m_controlSystem(&m_messageSystem, &m_gameObjects, &m_clock)
	, m_window(&m_messageSystem, WINDOW_TITLE, sf::Vector2u(WINDOW_DEFAULT_WIDTH, WINDOW_DEFAULT_HEIGHT), &m_inputSystem)
	, m_nextAvailableID(0) {}

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

void Game::AddGameObject(GameObject* l_gameObject) {
	m_gameObjects[l_gameObject->GetGameObjectID()] = l_gameObject;
}

void Game::Init() {
	// Init GameObjects
}

void Game::Update() {
	m_window.Update();
	m_messageSystem.DispatchMessages();
}

void Game::Render() {
	m_window.BeginDraw();
	// Draw things
	m_window.EndDraw();
}

void Game::LateUpdate() {
	RestartClock();
}

void Game::End() {
	// Destroy GameObjects
}

void Game::RestartClock() {
	m_elapsed = m_clock.restart();
}


