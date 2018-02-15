#pragma once
#include <unordered_map>
#include <string>
#include <SFML/System.hpp>
#include <memory>

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
	Game(const std::string& username);
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
	std::shared_ptr<MessageSystem> m_messageSystem;
	std::shared_ptr<GameObjectManager> m_gameObjectManager;
	std::shared_ptr<GameObjectContainer> m_gameObjects;
	std::shared_ptr<Window> m_window;
	std::shared_ptr<Input> m_inputSystem;
	std::shared_ptr<Control> m_controlSystem;
	std::shared_ptr<Renderer> m_renderSystem;
	std::shared_ptr<TextureManager> m_textureManager;
	std::shared_ptr<Network> m_networkSystem;
	std::shared_ptr<NetworkControl> m_networkControlSystem;


	sf::Clock m_clock;
	sf::Time m_elapsed;

	sf::Clock m_networkClock;
	sf::Time m_networkTime;

	std::string m_username;
};