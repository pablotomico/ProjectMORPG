#include "Control.hpp"

Control::Control(MessageSystem * l_messageSystem, GameObjectContainer* l_gameObjects)
	: Observer(System::S_Control, l_messageSystem), m_controlledGameObject(-1), m_movement(0, 0) {
	m_gameObjects = l_gameObjects;
}

Control::~Control() {}

void Control::SetControlledGameObject(GameObjectID l_controlledGameObject) {
	if (m_gameObjects->at(l_controlledGameObject)->IsControllable()) {
		m_controlledGameObject = l_controlledGameObject;
	}
}

void Control::Update(float l_deltaTime) {
	if (m_moving) {
		GameObject* gameObject = m_gameObjects->at(m_controlledGameObject);
		//printf("Movement (%f, %f)\n", m_movement.x, m_movement.y);
		gameObject->Move(m_movement * l_deltaTime);
	}
}

void Control::Notify(Message message) {
	if (m_controlledGameObject == -1) {
		return;
	}

	switch (message.m_type) {
	case MessageType::KeyPressed:
		{
			GameObject* gameObject = m_gameObjects->at(m_controlledGameObject);

			if (message.m_keyCode == sf::Keyboard::W) {
				m_movement.y = -1;
			} else if (message.m_keyCode == sf::Keyboard::S) {
				m_movement.y = 1;
			}

			if (message.m_keyCode == sf::Keyboard::A) {
				m_movement.x = -1;
			} else if (message.m_keyCode == sf::Keyboard::D) {
				m_movement.x = 1;
			}

			UpdateMovement();
		}
		break;
	case MessageType::KeyHold:
		break;
	case MessageType::KeyReleased:
		if (message.m_keyCode == sf::Keyboard::W) {
			m_movement.y = 0;
		}
		if (message.m_keyCode == sf::Keyboard::S) {
			m_movement.y = 0;
		}

		if (message.m_keyCode == sf::Keyboard::A) {
			m_movement.x = 0;
		}
		if (message.m_keyCode == sf::Keyboard::D) {
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

	m_movement *= (float) MOVEMENT_SPEED;
}
