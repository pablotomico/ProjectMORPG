#include "Message.hpp"

Message::Message(MessageType l_type, System l_systemReceiver) : m_type(l_type), m_systemReceiver(l_systemReceiver) {}

Message::Message(MessageType l_type, System l_systemReceiver, const std::string & l_string)
	: m_type(l_type), m_systemReceiver(l_systemReceiver), m_string(new std::string(l_string)) {}

Message::Message(MessageType l_type, System l_systemReceiver, KeyCode l_keyCode)
	: m_type(l_type), m_systemReceiver(l_systemReceiver), m_keyCode(l_keyCode) {}

Message::~Message() {
	if (m_type == MessageType::String) {
		delete m_string;
	}
}
