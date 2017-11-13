#pragma once

#include <SFML/Network.hpp>
#include "Observer.hpp"

class Network : public Observer {
public:
	Network(MessageSystem* l_messageSystem);
	~Network();

	void Update();

protected:
	void Notify(Message l_message);

private:
	
	
};