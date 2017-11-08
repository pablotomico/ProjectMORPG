#pragma once

#include "GameObject.hpp"
#include "Observer.hpp"
#include "Definitions.hpp"

class InputControl : public Observer {
public:
	InputControl(MessageSystem* l_messageSystem, GameObjectContainer* l_gameObjects);
	~InputControl();

	void SetControlledGameObject(GameObjectID l_controlledGameObject);
	GameObjectID GetControlledGameObject();

	void Update(float l_deltaTime);

protected:
	void Notify(Message l_message);
	void UpdateMovement();

private:
	GameObjectContainer* m_gameObjects;
	GameObjectID m_controlledGameObject;

	bool m_moving;
	sf::Vector2f m_movement;
};