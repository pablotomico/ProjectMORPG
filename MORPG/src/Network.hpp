#pragma once

#define _WINSOCK_DEPRECATED_NO_WARNINGS

#include <WinSock2.h>
#include <queue>
#include "Observer.hpp"

#include "NetFramework\include\NetFramework.hpp"

#pragma comment(lib, "ws2_32.lib")

struct NetMessage {
public:
	enum Type {
		SET_CLIENT_ID,
		INITIAL_DATA,
		DATA,
		CAST_SPELL
	};
	struct InitialData {
		int m_clientID;
		char m_username[16];
		float x;
		float y;
	};

	struct Data {
		int m_clientID;
		float x;
		float y;
	};
	struct ServerData {
		int m_clientID;
		float m_serverTimestep;
	};

	struct SpellData {
		int m_clientID;
		int m_spellID;
		float m_duration;
		int m_endTick;
	};

public:
	Type m_type;
	int m_tick;
	union {
		ServerData m_serverData;
		InitialData m_initialData;
		Data m_data;
		SpellData m_spellData;
	};
};


class Network : public Observer {
public:
	Network(MessageSystem* l_messageSystem);
	~Network();

	void Update();
	void ReadNetwork();
	void WriteNetwork();

protected:
	void Notify(Message l_message);

private:
	void StartWinSock();
	

public:
	float m_serverTimestep;
	int m_tick;

private:
	int m_client;
	
	net::UDPSocket* m_udpSocket;
	net::TCPSocket* m_tcpSocket;
	net::Address m_serverAddress;
	std::queue<NetMessage> m_udpWriteQueue;
	std::queue<NetMessage> m_tcpWriteQueue;
};