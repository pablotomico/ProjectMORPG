#include "Observer.hpp"
#include "MessageSystem.hpp"

Observer::Observer(System l_system, MessageSystem * l_messageSystem) : m_system(l_system), m_messageSystem(l_messageSystem) {
	m_messageSystem->AddObserver(m_system, GetNotify());
}

void Observer::Send(Message l_message) {
	m_messageSystem->SendMessage(l_message);
}
