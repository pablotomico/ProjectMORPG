#include "Message.hpp"

Message::Message(MessageType l_type, System l_systemReceiver) : m_type(l_type), m_systemReceiver(l_systemReceiver) {}

Message::Message(MessageType l_type, System l_systemReceiver, const std::string & l_string)
	: m_type(l_type), m_systemReceiver(l_systemReceiver), m_string(new std::string(l_string)) {}

Message::Message(MessageType l_type, System l_systemReceiver, KeyCode l_keyCode)
	: m_type(l_type), m_systemReceiver(l_systemReceiver), m_keyCode(l_keyCode) {}

Message::Message(MessageType l_type, System l_systemReceiver, GameObjectID l_id, sf::Vector2f l_position)
	: m_type(l_type), m_systemReceiver(l_systemReceiver), m_gameObject(l_id, l_position) {}

Message::Message(MessageType l_type, System l_systemReceiver, GameObjectID l_id, sf::Vector2f l_position, int l_tick)
	: m_type(l_type), m_systemReceiver(l_systemReceiver), m_gameObject(l_id, l_position, l_tick) {}

Message::~Message() {
	if (m_type == MessageType::String) {
		delete m_string;
	}
}
