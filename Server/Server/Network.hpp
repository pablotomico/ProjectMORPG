#pragma once

#define _WINSOCK_DEPRECATED_NO_WARNINGS		// for WSAAsyncSelect

#include <WinSock2.h>
#include <WS2tcpip.h>
#include <list>
#include <unordered_map>
#include <queue>

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

	Client* RegisterClient(SOCKET l_clientSocket, sockaddr_in l_address);
	void RemoveClient(ClientID l_clientID, SOCKET l_clientSocket);

	ClientID GetClientID(SOCKET l_clientSocket);
	Client* GetClient(ClientID l_clientID);

	bool ReadUDP();
	bool WriteUDP();

public:
	SOCKET m_serverTCPSocket;
	SOCKET m_serverUDPSocket;
	bool m_TCPWriteReady;
	bool m_UDPWriteReady;

private:
	void StartWinSock();

	void ProcessMessage(const NetMessage* l_message);
private:
	HWND m_window;
	int m_clientCount;
	std::unordered_map<SOCKET, ClientID> m_clientSocket;
	std::unordered_map<ClientID, Client*> m_clients;
	ClientID m_availableID;
	char m_udpReadBuffer[sizeof NetMessage];
	std::queue<NetMessage> m_udpMsgQueue;
	int m_udpReadCount;
	// TODO: Maybe a queue with available ids
};