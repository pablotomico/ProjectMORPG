#pragma once

#define _WINSOCK_DEPRECATED_NO_WARNINGS

#include <WinSock2.h>
#include <queue>
#include "Observer.hpp"
#include "NetFramework\include\NetMessage.hpp"

#include "NetFramework\include\NetFramework.hpp"

#pragma comment(lib, "ws2_32.lib")


class Network : public Observer {
public:
	Network(MessageSystem* l_messageSystem);
	Network(MessageSystem* l_messageSystem, std::string l_username);
	~Network();

	void Update();
	void ReadNetwork();
	void WriteNetwork();

	void SetControlledGameObject(GameObject* l_gameObject);

protected:
	void Notify(Message l_message);

private:
	void StartWinSock();

	void ReadTCP();
	void ReadUDP();
	void WriteTCP();
	void WriteUDP();
	
	void ProcessMessage(const NetMessage* l_message);

public:
	float m_serverTimestep;
	int m_tick;

private:
	int m_client;
	GameObject* m_controlledGameObject;

	bool m_connected;
	
	net::UDPSocket* m_udpSocket;
	net::TCPSocket* m_tcpSocket;
	net::Address m_serverAddress;
	std::queue<NetMessage> m_udpWriteQueue;
	std::queue<NetMessage> m_tcpWriteQueue;

	char m_tcpReadBuffer[sizeof NetMessage];
	char m_tcpWriteBuffer[sizeof NetMessage];

	std::string m_username;

	int m_tcpReadCount;
	int m_tcpWriteCount;
};