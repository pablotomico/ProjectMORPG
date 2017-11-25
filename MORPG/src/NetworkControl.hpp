#pragma once

#include "Observer.hpp"

class NetworkControl : public Observer {
public:
	NetworkControl(MessageSystem* l_messageSystem);

private:
	void RegisterClient(int l_clientID);
	void AddClientGameObject(int l_clientID, GameObjectID l_gameObjectID);
	void UpdateClient(int l_clientID, float l_x, float l_y, int l_tick);
	void RemoveClient(int l_clientID);

	bool HasClient(int l_clientID);
protected:
	void Notify(Message l_message);

private:
	std::unordered_map<int, GameObjectID> m_clientGameObjectMap;
	float m_timestep;
};