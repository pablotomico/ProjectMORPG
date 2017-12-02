#pragma once
#include <unordered_map>
#include <string>
#include <SFML/System.hpp>
#include "MessageSystem.hpp"
#include "Window.hpp"
#include "GameObject.hpp"
#include "Input.hpp"
#include "Control.hpp"
#include "Renderer.hpp"
#include "TextureManager.hpp"
#include "GameObjectManager.hpp"
#include "Network.hpp"
#include "NetworkControl.hpp"

class Game {

public:
	Game();
	Game(std::string username);
	~Game();

	void Play();
	
	sf::Time GetElapsedTime();

private:
	void Init();
	void Update();
	void Render();
	void LateUpdate();
	void End();

	void RestartClock();

private:
	MessageSystem m_messageSystem;
	GameObjectManager m_gameObjectManager;
	GameObjectContainer* m_gameObjects;
	Window m_window;
	Input m_inputSystem;
	Control m_controlSystem;
	Renderer m_renderSystem;
	TextureManager m_textureManager;
	Network m_networkSystem;
	NetworkControl m_networkControlSystem;


	sf::Clock m_clock;
	sf::Time m_elapsed;

	sf::Clock m_networkClock;
	sf::Time m_networkTime;

	std::string m_username;
};