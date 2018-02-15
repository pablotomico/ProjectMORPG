#pragma once

#include "Observer.hpp"

class NetworkControl : public Observer {
public:
	NetworkControl(const std::shared_ptr<MessageSystem>& l_messageSystem);

private:
	void RegisterClient(const int& l_clientID);
	void RegisterClient(const int& l_clientID, const std::string& l_username);
	void AddClientGameObject(const int& l_clientID, const GameObjectID& l_gameObjectID);
	void UpdateClient(const int& l_clientID, const float& l_x, const float& l_y, const int& l_tick);
	void RemoveClient(const int& l_clientID);

	bool HasClient(const int& l_clientID);
protected:
	void Notify(const Message& l_message);

private:
	std::unordered_map<int, GameObjectID> m_clientGameObjectMap;
	float m_timestep;
};