#pragma once

#include <WinSock2.h>

using ClientID = unsigned int;

class Client {
public:
	Client(ClientID l_id);
	Client(ClientID l_id, SOCKET l_socket);
	~Client();

public:
	ClientID m_id;
	SOCKET m_tcpSocket;
};