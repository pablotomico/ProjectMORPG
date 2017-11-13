#include "Control.hpp"

Control::Control(MessageSystem * l_messageSystem, GameObjectContainer* l_gameObjects)
	: Observer(System::S_InputControl, l_messageSystem), m_controlledGameObject(-1), m_movement(0, 0) {
	m_gameObjects = l_gameObjects;
}

Control::~Control() {}

void Control::SetControlledGameObject(GameObjectID l_controlledGameObject) {
	if (m_gameObjects->at(l_controlledGameObject)->IsControllable()) {
		m_controlledGameObject = l_controlledGameObject;
	}
}

GameObjectID Control::GetControlledGameObject() {
	return m_controlledGameObject;
}

void Control::Update(float l_deltaTime) {
	if (m_moving) {
		GameObject* gameObject = m_gameObjects->at(m_controlledGameObject);
		//printf("Movement (%f, %f)\n", m_movement.x, m_movement.y);
		gameObject->Move(m_movement * l_deltaTime);
		sf::Time time; // TODO: change this
		Message message(MessageType::M_GameObject, System::S_Network, gameObject->GetGameObjectID(), gameObject->GetPosition(), time);
		Send(message);
	}
}

void Control::Notify(Message l_message) {
	if (m_controlledGameObject == -1) {
		return;
	}

	switch (l_message.m_type) {
	case MessageType::M_KeyPressed:
		{
			GameObject* gameObject = m_gameObjects->at(m_controlledGameObject);

			if (l_message.m_keyCode == sf::Keyboard::W) {
				m_movement.y = -1;
			} else if (l_message.m_keyCode == sf::Keyboard::S) {
				m_movement.y = 1;
			}

			if (l_message.m_keyCode == sf::Keyboard::A) {
				m_movement.x = -1;
			} else if (l_message.m_keyCode == sf::Keyboard::D) {
				m_movement.x = 1;
			}

			UpdateMovement();
		}
		break;
	case MessageType::M_KeyHold:
		break;
	case MessageType::M_KeyReleased:
		if (l_message.m_keyCode == sf::Keyboard::W) {
			m_movement.y = 0;
		}
		if (l_message.m_keyCode == sf::Keyboard::S) {
			m_movement.y = 0;
		}

		if (l_message.m_keyCode == sf::Keyboard::A) {
			m_movement.x = 0;
		}
		if (l_message.m_keyCode == sf::Keyboard::D) {
			m_movement.x = 0;
		}

		UpdateMovement();

		break;
	default:
		break;
	}

}

void Control::UpdateMovement() {
	if (m_movement.x == 0 && m_movement.y == 0) {
		m_moving = false;
		return;
	}

	m_moving = true;

	if (m_movement.x > 0) {
		m_movement.x = 1;
	} else if (m_movement.x < 0) {
		m_movement.x = -1;
	}

	if (m_movement.y > 0) {
		m_movement.y = 1;
	} else if (m_movement.y < 0) {
		m_movement.y = -1;
	}
	float magnitude = sqrt((m_movement.x * m_movement.x) + (m_movement.y * m_movement.y));

	m_movement.x /= magnitude;
	m_movement.y /= magnitude;

	m_movement *= (float) MOVEMENT_SPEED / 10;
}
