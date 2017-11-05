#include "MessageSystem.hpp"

MessageSystem::MessageSystem() {}

MessageSystem::~MessageSystem() {}

void MessageSystem::AddObserver(System l_system, Callback l_callback) {
	m_observers[l_system] = l_callback;
}

void MessageSystem::RemoveObserver(System l_system) {
	m_observers.erase(l_system);
}

void MessageSystem::SendMessage(Message l_message) {
	m_msgQueue.push_back(l_message);
}

void MessageSystem::DispatchMessages() {
	while (!m_msgQueue.empty()) {
		/*Message msg = m_msgQueue.front();
		m_msgQueue.pop_front();

		(m_observers[msg.m_systemReceiver])(msg);*/
	}
}
