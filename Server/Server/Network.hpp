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



class Network {
public:
	Network(HWND l_window);
	~Network();

	Client* RegisterClient(SOCKET l_clientSocket, sockaddr_in l_address);
	void RemoveClient(ClientID l_clientID, SOCKET l_clientSocket);

	ClientID GetClientID(SOCKET l_clientSocket);
	Client* GetClient(ClientID l_clientID);

	std::unordered_map<SOCKET, ClientID>* GetClientSocket();

	bool ReadUDP();
	bool WriteUDP();

	bool ReadTCP(SOCKET l_tcpSocket);
	bool WriteTCP(ClientID l_client);

	void QueueTCPMessage(NetMessage l_message, SOCKET l_socket);

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
	char m_tcpReadBuffer[sizeof NetMessage];
	std::queue<NetMessage> m_udpMsgQueue;
	
	int m_udpReadCount;
	int m_tcpReadCount;



	// TODO: Maybe a queue with available ids
};