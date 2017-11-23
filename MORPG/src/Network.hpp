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
		DATA
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

public:
	Type m_type;
	union {
		int m_clientID;
		InitialData m_initialData;
		Data m_data;
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
	
private:
	int m_client;

	net::UDPSocket* m_udpSocket;
	net::TCPSocket* m_tcpSocket;
	net::Address m_serverAddress;
	std::queue<NetMessage> m_udpWriteQueue;
	std::queue<NetMessage> m_tcpWriteQueue;
};