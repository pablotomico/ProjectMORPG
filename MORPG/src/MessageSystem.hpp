#pragma once

#include <unordered_map>
#include <queue>
#include "Observer.hpp"

class Message;

class MessageSystem {
public:
	MessageSystem();
	~MessageSystem();

	void AddObserver(const System& l_system, const Callback& l_callback);
	void RemoveObserver(const System& l_system);

	void Send(const Message& l_message);
	void DispatchMessages();

private:
	std::unordered_map<System, Callback> m_observers;
	std::queue<Message> m_msgQueue;
};