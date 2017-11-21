#pragma once

#define _WINSOCK_DEPRECATED_NO_WARNINGS		// for WSAAsyncSelect

#include <WinSock2.h>
#include <WS2tcpip.h>
#include <list>
#include <unordered_map>

#include "Client.hpp"

#pragma comment(lib, "ws2_32.lib")
#define WM_SOCKET (WM_USER + 1)

#define SERVERIP "127.0.0.1"
#define SERVERPORT "5555"

struct NetMessage {
public:
	enum Type {
		SET_CLIENT_ID,
		DATA
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
		Data m_data;
	};
};

class Network {
public:
	Network(HWND l_window);
	~Network();

	Client* RegisterClient(SOCKET l_clientSocket);

public:
	SOCKET m_serverTCPSocket;
	SOCKET m_serverUDPSocket;


private:
	void StartWinSock();
private:
	HWND m_window;
	std::unordered_map<SOCKET, ClientID> m_clientSocket;
	std::unordered_map<ClientID, Client*> m_clients;
	ClientID m_availableID;
	// TODO: Maybe a queue with available ids
};