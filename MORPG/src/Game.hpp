#pragma once
#include <unordered_map>
#include <SFML/System.hpp>
#include "MessageSystem.hpp"
#include "Window.hpp"
#include "GameObject.hpp"
#include "Input.hpp"
#include "Control.hpp"

class Game {

public:
	Game();
	~Game();

	void Play();
	
	sf::Time GetElapsedTime();

	void AddGameObject(GameObject* l_gameObject);

private:
	void Init();
	void Update();
	void Render();
	void LateUpdate();
	void End();

	void RestartClock();

private:
	MessageSystem m_messageSystem;
	Window m_window;
	Input m_inputSystem;
	Control m_controlSystem;

	GameObjectID m_nextAvailableID;
	GameObjectContainer m_gameObjects;


	sf::Clock m_clock;
	sf::Time m_elapsed;
};