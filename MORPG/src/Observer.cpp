#include "Observer.hpp"
#include "MessageSystem.hpp"

void Observer::Send(Message l_message) {
	m_messageSystem->SendMessage(l_message);
}
