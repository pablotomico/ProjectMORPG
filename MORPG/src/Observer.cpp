#include "Observer.hpp"
#include "MessageSystem.hpp"

Observer::Observer(const System& l_system, const std::shared_ptr<MessageSystem>& l_messageSystem) : m_system(l_system), m_messageSystem(l_messageSystem) {
	m_messageSystem->AddObserver(m_system, GetNotify());
}

void Observer::Send(const Message& l_message) {
	m_messageSystem->Send(l_message);
}
