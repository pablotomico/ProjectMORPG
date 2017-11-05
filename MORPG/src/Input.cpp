#include "Input.hpp"
#include "MessageSystem.hpp"

Input::Input(MessageSystem* l_messageSystem, GameObjectContainer* l_gameObjects) : Observer(System::S_Input, l_messageSystem) {
	m_gameObjects = l_gameObjects;
}

Input::~Input() {}

void Input::Notify(Message message) {}

void Input::HandleEvent(sf::Event l_event) {
	switch (l_event.type) {
	case sf::Event::KeyPressed:
		{
			sf::Keyboard::Key code = l_event.key.code;

			// Send message in multicast mode
			Message msg(MessageType::KeyPressed, (System) MULTICAST, code);
			Send(msg);

			// Notify all controllable GameObjects
			for (auto& it = m_gameObjects->begin(); it != m_gameObjects->end(); it++) {

				//Check if it is not controllable
				if (!it->second->IsControllable()) {
					continue;
				}

				if (m_keysDown[code]) {
					it->second->OnKey(code);
				} else {
					it->second->OnKeyDown(code);

				}
			}
			// Set Key to pressed for future use (separation between GetKeyDown and GetKey)
			m_keysDown[code] = true;
		}
		break;
	case sf::Event::KeyReleased:
		{
			sf::Keyboard::Key code = l_event.key.code;

			// Send message in multicast mode
			Message msg(MessageType::KeyReleased, (System) MULTICAST, code);
			Send(msg);

			// Notify all controllable GameObjects
			for (auto& it = m_gameObjects->begin(); it != m_gameObjects->end(); it++) {
				//Check if it is not controllable
				if (!it->second->IsControllable()) {
					continue;
				}
				it->second->OnKeyUp(code);
			}
			// Set Key to not pressed
			m_keysDown[code] = false;
		}
		break;
	}
}
