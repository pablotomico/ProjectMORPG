/*	AG0907 Lab 4 UDP server example - by Henry Fortuna and Adam Sampson

When the user types a message, the client sends it to the server
as a UDP packet. The server then sends a packet back to the
client, and the client prints it out.
*/
#define _WINSOCK_DEPRECATED_NO_WARNINGS

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <winsock2.h>
#include <map>
#include <algorithm>

#pragma comment(lib, "ws2_32.lib")

// The IP address for the server
#define SERVERIP "127.0.0.1"

// The UDP port number for the server
#define SERVERPORT 5555



// Prototypes
void die(const char *message);

struct NetMessage {
	int m_client;
	float m_x;
	float m_y;
};

int main() {
	printf("Echo Server\n");

	// Initialise the WinSock library -- we want version 2.2.
	WSADATA w;
	int error = WSAStartup(0x0202, &w);
	if (error != 0) {
		die("WSAStartup failed");
	}
	if (w.wVersion != 0x0202) {
		die("Wrong WinSock version");
	}

	// Create a UDP socket.
	SOCKET sock = socket(AF_INET, SOCK_DGRAM, 0);
	if (sock == INVALID_SOCKET) {
		die("socket failed");
	}

	// Fill out a sockaddr_in structure to describe the address we'll listen on.
	sockaddr_in serverAddr;
	serverAddr.sin_family = AF_INET;
	serverAddr.sin_addr.s_addr = inet_addr(SERVERIP);
	// htons converts the port number to network byte order (big-endian).
	serverAddr.sin_port = htons(SERVERPORT);

	// Bind the socket to that address.
	if (bind(sock, (const sockaddr *) &serverAddr, sizeof(serverAddr)) != 0) {
		die("bind failed");
	}

	// ntohs does the opposite of htons.
	printf("Server socket bound to address %s, port %d\n", inet_ntoa(serverAddr.sin_addr), ntohs(serverAddr.sin_port));

	std::map<int, sockaddr_in> clients;

	while (true) {
		NetMessage msg;

		sockaddr_in fromAddr;
		int fromAddrSize = sizeof(fromAddr);
		int count = recvfrom(sock, (char *) &msg, sizeof NetMessage, 0,
			(sockaddr *) &fromAddr, &fromAddrSize);

		if (count < 0) {
			die("recvfrom failed");
		}
		if (count != sizeof NetMessage) {
			die("received odd-sized message");
		}
		clients[msg.m_client] = fromAddr;

		for (auto itr = clients.begin(); itr != clients.end(); ++itr) {
			if (sendto(sock, (const char *) &msg, sizeof NetMessage, 0,
				(const sockaddr *) &(itr->second), sizeof(itr->second)) != sizeof NetMessage) {
				printf("Failed sending\n");
			}
		}
		printf("[%d] -> (%f, %f)\n", msg.m_client, msg.m_x, msg.m_y);
	}

	// We won't actually get here, but if we did then we'd want to clean up...
	printf("Quitting\n");
	closesocket(sock);
	WSACleanup();
	return 0;
}


// Print an error message and exit.
void die(const char *message) {
	fprintf(stderr, "Error: %s (WSAGetLastError() = %d)\n", message, WSAGetLastError());

#ifdef _DEBUG
	// Debug build -- drop the program into the debugger.
	abort();
#else
	exit(1);
#endif
}