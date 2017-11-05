#include "Input.hpp"
#include "MessageSystem.hpp"

Input::Input(MessageSystem* l_messageSystem) : Observer(System::S_Input, l_messageSystem) {}

Input::~Input() {}

void Input::Notify(Message message) {}

void Input::HandleEvent(sf::Event l_event) {
	switch (l_event.type) {
	case sf::Event::KeyPressed:
		Message msg(MessageType::KeyPressed, (System) MULTICAST, l_event.key.code);
		Send(msg);
		break;
	}
}
