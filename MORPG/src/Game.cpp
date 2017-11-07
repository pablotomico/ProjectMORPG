#include "Game.hpp"
#include "Definitions.hpp"
#include "Input.hpp"

Game::Game()
	: m_inputSystem(&m_messageSystem)
	, m_controlSystem(&m_messageSystem, &m_gameObjects)
	, m_window(&m_messageSystem, WINDOW_TITLE, sf::Vector2u(WINDOW_DEFAULT_WIDTH, WINDOW_DEFAULT_HEIGHT), &m_inputSystem)
	, m_renderSystem(&m_messageSystem, &m_gameObjects, &m_window)
	, m_nextAvailableID(0) {}

Game::~Game() {}

void Game::Play() {
	Init();
	while (!m_window.IsDone()) {
		Update();
		Render();
		LateUpdate();
	}
	End();
}

sf::Time Game::GetElapsedTime() {
	return m_elapsed;
}

void Game::AddGameObject(GameObject* l_gameObject) {
	m_gameObjects[l_gameObject->GetGameObjectID()] = l_gameObject;
}

void Game::Init() {
	// Init GameObjects
	GameObject* go = new GameObject(m_nextAvailableID, true, true, &m_textureManager);
	go->SetSprite("intro.png");
	AddGameObject(go);
	m_controlSystem.SetControlledGameObject(m_nextAvailableID);
	m_nextAvailableID++;
}

void Game::Update() {
	float deltaTime = m_elapsed.asSeconds() * 1000;
	m_window.Update(deltaTime);
	m_messageSystem.DispatchMessages();

	m_controlSystem.Update(deltaTime);
	m_renderSystem.Update(deltaTime);
}

void Game::Render() {
	m_window.BeginDraw();
	
	m_renderSystem.Render();

	m_window.EndDraw();
}

void Game::LateUpdate() {
	RestartClock();
}

void Game::End() {
	// Destroy GameObjects
	for (auto& it = m_gameObjects.begin(); it != m_gameObjects.end(); it++) {
		delete it->second;
	}
}

void Game::RestartClock() {
	m_elapsed = m_clock.restart();
}


