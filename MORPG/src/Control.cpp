#include "Control.hpp"
#include "GameObject.hpp"
#include "Util\Utilities.hpp"

Control::Control(const std::shared_ptr<MessageSystem> & l_messageSystem, const std::shared_ptr<GameObjectContainer>& l_gameObjects)
	: Observer(System::S_Control, l_messageSystem)
	, m_controlledGameObject(-1)
	, m_movement(0, 0)
	, m_gameObjects(l_gameObjects) {
}

Control::~Control() {}

void Control::SetControlledGameObject(const GameObjectID& l_controlledGameObject) {
	if (m_gameObjects->at(l_controlledGameObject)->IsControllable()) {
		m_controlledGameObject = l_controlledGameObject;
	}
}

GameObjectID Control::GetControlledGameObject() {
	return m_controlledGameObject;
}

void Control::Update(const float& l_deltaTime) {
	GameObject* gameObject = m_gameObjects->at(m_controlledGameObject);
	if (m_moving) {
		//printf("Movement (%f, %f)\n", m_movement.x, m_movement.y);
		gameObject->Move(m_movement * l_deltaTime);
	}
	//Message message(MessageType::M_GameObject, System::S_Network, gameObject->GetGameObjectID(), gameObject->GetPosition());
	//Send(message);
}

void Control::Notify(const Message& l_message) {
	if (m_controlledGameObject == -1) {
		return;
	}

	switch (l_message.m_type) {
	case MessageType::M_KeyPressed:
	{
		GameObject* gameObject = m_gameObjects->at(m_controlledGameObject);

		//LOG(l_message.m_keyCode);

		if (l_message.m_keyCode == sf::Keyboard::W) {
			m_movement.y = -1;
		}
		else if (l_message.m_keyCode == sf::Keyboard::S) {
			m_movement.y = 1;
		}

		if (l_message.m_keyCode == sf::Keyboard::A) {
			m_movement.x = -1;
		}
		else if (l_message.m_keyCode == sf::Keyboard::D) {
			m_movement.x = 1;
		}

		UpdateMovement();

		if (l_message.m_keyCode == sf::Keyboard::Num1) {
			Message message(MessageType::M_CastSpell, System::S_Network);
			SpellData spell;
			spell.m_spellID = 1;
			spell.m_duration = 1.0f;
			message.m_spellData = spell;


			Send(message);
		}
		else if (l_message.m_keyCode == sf::Keyboard::Num2) {
			Message message(MessageType::M_CastSpell, System::S_Network);
			SpellData spell;
			spell.m_spellID = 2;
			spell.m_duration = 2.0f;
			message.m_spellData = spell;


			Send(message);
		}
		else if (l_message.m_keyCode == sf::Keyboard::Num3) {
			Message message(MessageType::M_CastSpell, System::S_Network);
			SpellData spell;
			spell.m_spellID = 3;
			spell.m_duration = 3.0f;
			message.m_spellData = spell;


			Send(message);
		}
		else if (l_message.m_keyCode == sf::Keyboard::C) {
			Message message(MessageType::M_CastSpell, System::S_Network);
			SpellData spell;
			spell.m_spellID = -1;
			spell.m_duration = 0;
			message.m_spellData = spell;


			Send(message);
		}
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
	}
	else if (m_movement.x < 0) {
		m_movement.x = -1;
	}

	if (m_movement.y > 0) {
		m_movement.y = 1;
	}
	else if (m_movement.y < 0) {
		m_movement.y = -1;
	}
	float magnitude = sqrt((m_movement.x * m_movement.x) + (m_movement.y * m_movement.y));

	m_movement.x /= magnitude;
	m_movement.y /= magnitude;

	m_movement *= (float)MOVEMENT_SPEED / 10;
}
