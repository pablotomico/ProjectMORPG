#pragma once
#include <unordered_map>
#include <SFML/System.hpp>
#include "MessageSystem.hpp"
#include "Window.hpp"
#include "GameObject.hpp"
#include "Input.hpp"
#include "InputControl.hpp"
#include "Renderer.hpp"
#include "TextureManager.hpp"
#include "GameObjectManager.hpp"

class Game {

public:
	Game();
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
	InputControl m_inputControlSystem;
	Renderer m_renderSystem;
	TextureManager m_textureManager;


	sf::Clock m_clock;
	sf::Time m_elapsed;
};