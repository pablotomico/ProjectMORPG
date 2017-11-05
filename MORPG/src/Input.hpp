#pragma once
#include <unordered_map>
#include <SFML/Window.hpp>
#include "Observer.hpp"

class Input : public Observer {
public:
	Input(MessageSystem* l_messageSystem);
	~Input();
	void HandleEvent(sf::Event l_event);

protected:
	void Notify(Message message);
};