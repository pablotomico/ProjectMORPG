#pragma once

#include <functional>

#include "Message.hpp"
//#include "Util\Utilities.hpp"

class MessageSystem;

using Callback = std::function<void(Message)>;

class Observer {
public:
	Observer(System l_system, MessageSystem* l_messageSystem);

protected:
	Callback GetNotify() {
		auto messageListener = [=](Message message) -> void {
			this->Notify(message);
		};
		return messageListener;
	}
	void Send(Message l_message);

	virtual void Notify(Message message) {
		//LOG("Notify function not implemented");
	}

protected:
	System m_system;
	MessageSystem* m_messageSystem;

};