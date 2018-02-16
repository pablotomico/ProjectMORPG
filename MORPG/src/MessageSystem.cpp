#include "MessageSystem.hpp"

#include <functional>
#include "Message.hpp"
#include "Util\Utilities.hpp"


MessageSystem::MessageSystem() {}

MessageSystem::~MessageSystem() {}

void MessageSystem::AddObserver(const System& l_system, const Callback& l_callback) {
	//LOG("Adding observer: " << l_system);
	m_observers[l_system] = l_callback;
}

void MessageSystem::RemoveObserver(const System& l_system) {
	m_observers.erase(l_system);
}

void MessageSystem::Send(const Message& l_message) {
	m_msgQueue.push(l_message);
}

void MessageSystem::DispatchMessages() {
	while (!m_msgQueue.empty()) {
		Message msg = m_msgQueue.front();
		m_msgQueue.pop();
		if (msg.m_systemReceiver == MULTICAST) {
			for (auto& e : m_observers) {
				(m_observers[e.first])(msg);
			}
		}
		else {
			(m_observers[msg.m_systemReceiver])(msg);
		}
	}
}
