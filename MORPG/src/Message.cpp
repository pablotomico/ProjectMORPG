#include "Message.hpp"
#include "Util\Utilities.hpp"

Message::Message(const MessageType& l_type, const System& l_systemReceiver) : m_type(l_type), m_systemReceiver(l_systemReceiver) {}

Message::Message(const MessageType& l_type, const System& l_systemReceiver, const std::string & l_string)
	: m_type(l_type), m_systemReceiver(l_systemReceiver), m_string(new std::string(l_string)) {}

Message::Message(const MessageType& l_type, const System& l_systemReceiver, const KeyCode& l_keyCode)
	: m_type(l_type), m_systemReceiver(l_systemReceiver), m_keyCode(l_keyCode) {}

Message::Message(const MessageType& l_type, const System& l_systemReceiver, const GameObjectID& l_id, const sf::Vector2f& l_position)
	: m_type(l_type), m_systemReceiver(l_systemReceiver), m_gameObject(l_id, l_position) {}

Message::Message(const MessageType& l_type, const System& l_systemReceiver, const GameObjectID& l_id, const sf::Vector2f& l_position, const int& l_tick)
	: m_type(l_type), m_systemReceiver(l_systemReceiver), m_gameObject(l_id, l_position, l_tick) {}

Message::~Message() {
	if (m_type == MessageType::String) {
		delete m_string;
	}
}

void GameObjectMessage::Print() {
	LOG("[" << m_gameObjectID << "] (" << m_position.x << ", " << m_position.y << ")");
}
