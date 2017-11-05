#include "Control.hpp"

Control::Control(MessageSystem * l_messageSystem, GameObjectContainer* l_gameObjects, sf::Clock* l_clock)
	: Observer(System::S_Control, l_messageSystem), m_controlledGameObject(-1){
	m_gameObjects = l_gameObjects;
	m_clock = l_clock;
}

Control::~Control() {}

void Control::SetControlledGameObject(GameObjectID l_controlledGameObject) {
	m_controlledGameObject = l_controlledGameObject;
}

void Control::Notify(Message message) {
	if (m_controlledGameObject == -1) {
		return;
	}

	switch (message.m_type) {
	case MessageType::KeyPressed:
		break;
	case MessageType::KeyHold:
		break;
	case MessageType::KeyReleased:
		break;
	default:
		break;
	}

}
