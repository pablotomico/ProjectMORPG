#pragma once

#include <iostream>

#include "Systems.hpp"

enum MessageType {
	General
	// TODO: Come up with more types
};

struct MessageData {
	union {
		std::string m_content;
	};
};

class Message {
public:
	Message(MessageType l_type, System l_systemReceiver) : m_type(l_type), m_systemReceiver(l_systemReceiver) {}
public:
	MessageType m_type;
	System m_systemReceiver;

private:
	MessageData m_data;
};