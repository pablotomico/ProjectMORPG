#pragma once

#include <functional>
#include <unordered_map>
#include <queue>

#include "Observer.hpp"

class MessageSystem {
public:
	MessageSystem();
	~MessageSystem();

	void AddObserver(System l_system, Callback l_callback);
	void RemoveObserver(System l_system);

	void SendMessage(Message l_message);
	void DispatchMessages();

private:
	std::unordered_map<System, Callback> m_observers;
	std::deque<Message> m_msgQueue;
};