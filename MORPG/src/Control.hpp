#pragma once

#include "GameObject.hpp"
#include "Observer.hpp"
#include "Definitions.hpp"

class Control : public Observer {
public:
	Control(MessageSystem* l_messageSystem, GameObjectContainer* l_gameObjects);
	~Control();

	void SetControlledGameObject(GameObjectID l_controlledGameObject);

	void Update(float l_deltaTime);

protected:
	void Notify(Message message);
	void UpdateMovement();

private:
	GameObjectContainer* m_gameObjects;
	GameObjectID m_controlledGameObject;

	bool m_moving;
	sf::Vector2f m_movement;
};