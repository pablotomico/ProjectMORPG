#pragma once
#include <unordered_map>
#include <SFML/Window.hpp>
#include "Observer.hpp"
#include "GameObject.hpp"
#include "Definitions.hpp"

class Input : public Observer {
public:
	Input(MessageSystem* l_messageSystem);
	~Input();
	void HandleEvent(sf::Event l_event);
	void Update(float l_deltaTime);

protected:
	void Notify(Message message);

private:
	std::unordered_map<KeyCode, bool> m_keysDown;
};