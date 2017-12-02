#include "Game.hpp"
#include "Definitions.hpp"
#include "Input.hpp"

Game::Game()
	: m_inputSystem(&m_messageSystem)
	, m_gameObjectManager(&m_messageSystem, &m_textureManager)
	, m_gameObjects(m_gameObjectManager.GetGameObjectContainer())
	, m_controlSystem(&m_messageSystem, m_gameObjects)
	, m_window(&m_messageSystem, WINDOW_TITLE, sf::Vector2u(WINDOW_DEFAULT_WIDTH, WINDOW_DEFAULT_HEIGHT), &m_inputSystem)
	, m_renderSystem(&m_messageSystem, m_gameObjects, &m_window)
	, m_networkSystem(&m_messageSystem)
	, m_networkControlSystem(&m_messageSystem) {}

Game::Game(std::string l_username)
	: m_username(l_username)
	, m_inputSystem(&m_messageSystem)
	, m_gameObjectManager(&m_messageSystem, &m_textureManager)
	, m_gameObjects(m_gameObjectManager.GetGameObjectContainer())
	, m_controlSystem(&m_messageSystem, m_gameObjects)
	, m_window(&m_messageSystem, WINDOW_TITLE, sf::Vector2u(WINDOW_DEFAULT_WIDTH, WINDOW_DEFAULT_HEIGHT), &m_inputSystem)
	, m_renderSystem(&m_messageSystem, m_gameObjects, &m_window)
	, m_networkSystem(&m_messageSystem, l_username)
	, m_networkControlSystem(&m_messageSystem) {}

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


void Game::Init() {
	// Init GameObjects
	GameObjectID id = m_gameObjectManager.CreateGameObject(m_username, true, true, "character_1.png");
	GameObject* gameobject = m_gameObjectManager.GetGameObject(id);
	gameobject->SetSpriteScale(0.25f, 0.25f);


	m_controlSystem.SetControlledGameObject(id);
	m_networkSystem.SetControlledGameObject(id);
}

void Game::Update() {
	float deltaTime = m_elapsed.asSeconds() * 1000;
	bool writeNetwork = false;
	if (m_networkTime.asMilliseconds() > m_networkSystem.m_serverTimestep) {
		int deltaTick = floor(m_networkTime.asMilliseconds() / m_networkSystem.m_serverTimestep);
		m_networkSystem.m_tick += deltaTick;
		//printf("TICK: %d\n", m_networkSystem.m_tick);
		m_networkTime -= sf::milliseconds(m_networkSystem.m_serverTimestep * deltaTick);
		m_networkSystem.ReadNetwork();
		writeNetwork = true;
	}

	m_window.Update(deltaTime);
	m_messageSystem.DispatchMessages();
	m_controlSystem.Update(deltaTime);
	m_networkSystem.Update();

	m_gameObjectManager.Update(m_networkSystem.m_tick, m_networkTime.asMilliseconds());

	if (writeNetwork) {
		m_networkSystem.WriteNetwork();
	}

	m_renderSystem.Update(deltaTime);

	if (CAMERA_FOLLOW) {
		m_window.UpdateCamera(m_gameObjects->at(m_controlSystem.GetControlledGameObject()));
	}
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

}

void Game::RestartClock() {
	m_elapsed = m_clock.restart();
	m_networkTime += m_networkClock.restart();
}


