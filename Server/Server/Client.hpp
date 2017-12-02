#pragma once

#include <WinSock2.h>
#include <queue>
#include "NetMessage.hpp"

using ClientID = unsigned int;

class Client {
public:
	Client(ClientID l_id);
	Client(ClientID l_id, SOCKET l_socket, sockaddr_in l_address);
	~Client();


public:
	ClientID m_id;
	SOCKET m_tcpSocket;
	sockaddr_in m_address;
	std::string m_username;

	std::queue<NetMessage> m_tcpMsgQueue;
	char m_tcpWriteBuffer[sizeof NetMessage];
	int m_tcpWriteCount;
};