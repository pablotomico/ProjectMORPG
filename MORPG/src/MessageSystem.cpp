#include <functional>

#include "MessageSystem.hpp"
#include "Util\Utilities.hpp"


MessageSystem::MessageSystem() {}

MessageSystem::~MessageSystem() {}

void MessageSystem::AddObserver(System l_system, Callback l_callback) {
	//LOG("Adding observer: " << l_system);
	m_observers[l_system] = l_callback;
}

void MessageSystem::RemoveObserver(System l_system) {
	m_observers.erase(l_system);
}

void MessageSystem::Send(Message l_message) {
	m_msgQueue.push(l_message);
}

void MessageSystem::DispatchMessages() {
	while (!m_msgQueue.empty()) {
		Message msg = m_msgQueue.front();
		m_msgQueue.pop();
		if (msg.m_systemReceiver == MULTICAST) {
			for (auto& it = m_observers.begin(); it != m_observers.end(); it++) {
				(m_observers[it->first])(msg);
			}
		}
		else {
			(m_observers[msg.m_systemReceiver])(msg);
		}
	}
}
