#pragma once

#define _WINSOCK_DEPRECATED_NO_WARNINGS

#include <WinSock2.h>
#include "Observer.hpp"

#include "NetFramework\include\NetFramework.hpp"

#pragma comment(lib, "ws2_32.lib")

struct NetMessage {
	int m_client;
	float m_x;
	float m_y;
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
	net::Address m_serverAddress;

	fd_set m_socketSetRead;
	fd_set m_socketSetWrite;
	SOCKET m_socket;
	sockaddr_in m_serverAddr;
	timeval m_timeout;
};