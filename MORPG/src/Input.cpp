#include "Input.hpp"
#include "MessageSystem.hpp"
#include "Util\Utilities.hpp"

Input::Input(const std::shared_ptr<MessageSystem>& l_messageSystem) : Observer(System::S_Input, l_messageSystem) {}

Input::~Input() {}

void Input::Notify(const Message& message) {}

void Input::Update(const float& l_deltaTime) {}

void Input::HandleEvent(const sf::Event& l_event) {
	switch (l_event.type) {
	case sf::Event::KeyPressed:
		{
			KeyCode code = l_event.key.code;
			//LOG("KEY: " << code);

			// Send message in multicast mode
			if (m_keysDown[code]) {
				Message msg(MessageType::M_KeyHold, (System) MULTICAST, code);
				Send(msg);
			} else {
				Message msg(MessageType::M_KeyPressed, (System) MULTICAST, code);
				Send(msg);

				// Set Key to pressed for future use (separation between GetKeyDown and GetKey)
				m_keysDown[code] = true;
			}
		}
		break;
	case sf::Event::KeyReleased:
		{
			KeyCode code = l_event.key.code;

			// Send message in multicast mode
			Message msg(MessageType::M_KeyReleased, (System) MULTICAST, code);
			Send(msg);

			// Set keycode as released
			m_keysDown[code] = false;
		}
		break;
	}
}

