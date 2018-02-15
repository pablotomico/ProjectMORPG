#pragma once

#include <unordered_map>
#include <queue>
#include "Observer.hpp"
#include "Message.hpp"

class MessageSystem {
public:
	MessageSystem();
	~MessageSystem();

	void AddObserver(System l_system, Callback l_callback);
	void RemoveObserver(System l_system);

	void Send(Message l_message);
	void DispatchMessages();

private:
	std::unordered_map<System, Callback> m_observers;
	std::queue<Message> m_msgQueue;
};