#pragma once

#include <functional>
#include <memory>

#include "Message.hpp"
//#include "Util\Utilities.hpp"

class MessageSystem;

using Callback = std::function<void(Message)>;

class Observer {
public:
	Observer(const System& l_system, const std::shared_ptr<MessageSystem>& l_messageSystem);

protected:
	Callback GetNotify() {
		auto messageListener = [=](const Message& message) -> void {
			this->Notify(message);
		};
		return messageListener;
	}
	void Send(const Message& l_message);

	virtual void Notify(const Message& message) {
		//LOG("Notify function not implemented");
	}

protected:
	System m_system;
	std::shared_ptr<MessageSystem> m_messageSystem;
};