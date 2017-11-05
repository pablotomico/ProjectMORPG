#pragma once
#include <unordered_map>
#include <SFML/Window.hpp>
#include "Observer.hpp"
#include "GameObject.hpp"
#include "Definitions.hpp"

class Input : public Observer {
public:
	Input(MessageSystem* l_messageSystem, GameObjectContainer* l_gameObjects);
	~Input();
	void HandleEvent(sf::Event l_event);

protected:
	void Notify(Message message);

private:
	GameObjectContainer* m_gameObjects;
	std::unordered_map<KeyCode, bool> m_keysDown;
};