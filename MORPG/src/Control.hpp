#pragma once

#include "GameObject.hpp"
#include "Observer.hpp"
#include "Definitions.hpp"

class Control : public Observer {
public:
	Control(MessageSystem* l_messageSystem, GameObjectContainer* l_gameObjects, sf::Clock* l_clock);
	~Control();

	void SetControlledGameObject(GameObjectID l_controlledGameObject);

protected:
	void Notify(Message message);

private:
	GameObjectContainer* m_gameObjects;
	GameObjectID m_controlledGameObject;
	sf::Clock* m_clock;
};