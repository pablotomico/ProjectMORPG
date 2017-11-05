#pragma once

#include <iostream>
#include <SFML/Window.hpp>
#include "Systems.hpp"
#include "Definitions.hpp"

#define MULTICAST -1

enum  MessageType {
	String,
	Int,
	KeyPressed,
	KeyHold,
	KeyReleased
	// TODO: Come up with more types
};

class Message {
public:
	Message(MessageType l_type, System l_systemReceiver);
	Message(MessageType l_type, System l_systemReceiver, const std::string& l_string);
	Message(MessageType l_type, System l_systemReceiver, KeyCode l_keyCode);

	~Message();
public:
	MessageType m_type;
	System m_systemReceiver;
	union {
		std::string* m_string;
		int m_integer;
		KeyCode m_keyCode;
	};
};