#pragma once

#include "GameObject.hpp"
#include "Observer.hpp"
#include "Definitions.hpp"

class Control : public Observer {
public:
	Control(const std::shared_ptr<MessageSystem> & l_messageSystem, const std::shared_ptr<GameObjectContainer>& l_gameObjects);
	~Control();

	void SetControlledGameObject(const GameObjectID& l_controlledGameObject);
	GameObjectID GetControlledGameObject();

	void Update(const float& l_deltaTime);

protected:
	void Notify(const Message& l_message);
	void UpdateMovement();

private:
	std::shared_ptr<GameObjectContainer> m_gameObjects;
	GameObjectID m_controlledGameObject;

	bool m_moving;
	sf::Vector2f m_movement;
};