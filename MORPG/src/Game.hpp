#pragma once
#include <string>
#include <memory>
#include <unordered_map>
#include <SFML/System.hpp>

#include "Definitions.hpp"

class MessageSystem;
class GameObjectManager;
class Window;
class Input;
class Control;
class Renderer;
class TextureManager;
class Network;
class NetworkControl;

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