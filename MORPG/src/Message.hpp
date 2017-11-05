#pragma once

#include <iostream>
#include <SFML/Window.hpp>
#include "Systems.hpp"

#define MULTICAST -1

enum  MessageType {
	String,
	Int,
	KeyPressed
	// TODO: Come up with more types
};

class Message {
public:
	Message(MessageType l_type, System l_systemReceiver);
	Message(MessageType l_type, System l_systemReceiver, const std::string& l_string);
	Message(MessageType l_type, System l_systemReceiver, sf::Keyboard::Key l_keyCode);

	~Message();
public:
	MessageType m_type;
	System m_systemReceiver;
	union {
		std::string* m_string;
		int m_integer;
		sf::Keyboard::Key m_keyCode;
	};
};