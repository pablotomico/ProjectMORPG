#pragma once
#include <unordered_map>
#include <SFML/Window.hpp>
#include <memory>
#include "Observer.hpp"
#include "GameObject.hpp"
#include "Definitions.hpp"

class Input : public Observer {
public:
	Input(const std::shared_ptr<MessageSystem>& l_messageSystem);
	~Input();
	void HandleEvent(const sf::Event& l_event);
	void Update(const float& l_deltaTime);

protected:
	void Notify(const Message& message);

private:
	std::unordered_map<KeyCode, bool> m_keysDown;
};