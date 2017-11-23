#include "Window.hpp"

#include "Network.hpp"

Network* m_network;
HWND m_window;

HINSTANCE hInst;                                // current instance
WCHAR szTitle[MAX_LOADSTRING];                  // The title bar text
WCHAR szWindowClass[MAX_LOADSTRING];            // the main window class name

ATOM MyRegisterClass(HINSTANCE hInstance) {
	WNDCLASSEX wcex;

	wcex.cbSize = sizeof(wcex);
	wcex.style = CS_HREDRAW | CS_VREDRAW;
	// Use our window procedure to handle messages for this kind of window.
	wcex.lpfnWndProc = WndProc;
	wcex.cbClsExtra = 0;
	wcex.cbWndExtra = 0;
	wcex.hInstance = hInstance;
	wcex.hIcon = 0;
	wcex.hCursor = LoadCursor(nullptr, IDC_ARROW);

	wcex.hbrBackground = (HBRUSH) (COLOR_WINDOW + 1);
	wcex.lpszMenuName = nullptr;
	wcex.lpszClassName = L"WindowClass";
	wcex.hIconSm = 0;

	return RegisterClassExW(&wcex);
}

BOOL InitInstance(HINSTANCE hInstance, int nCmdShow) {
	hInst = hInstance; // Store instance handle in our global variable

	m_window = CreateWindow(L"WindowClass",
							L"Server",
							WS_OVERLAPPEDWINDOW,
							600, 600,
							400, 200,
							nullptr,
							nullptr,
							hInstance,
							nullptr);
	m_network = new Network(m_window);
	if (!m_window) {
		return FALSE;
	}

	ShowWindow(m_window, nCmdShow);
	UpdateWindow(m_window);

	return TRUE;
}


//
//  FUNCTION: WndProc(HWND, UINT, WPARAM, LPARAM)
//
//  PURPOSE:  Processes messages for the main window.
//
//  WM_COMMAND  - process the application menu
//  WM_PAINT    - Paint the main window
//  WM_DESTROY  - post a quit message and return
//
//
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam) {
	switch (message) {
	case WM_DESTROY:
		CloseWindow();
		break;
	case WM_KEYDOWN:
		switch (wParam) {
		case VK_ESCAPE:
			CloseWindow();
			break;
		}
		break;
	case WM_PAINT:
		{
			DrawWindow(hWnd);
		}
		break;

	case WM_SOCKET:
		HandleSocketEvent(lParam, wParam);
		break;


	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
	}
	return 0;
}


void DrawWindow(HWND window) {
	std::wstring message = L"Press Esc to exit\n";

	PAINTSTRUCT ps;
	HDC hdc = BeginPaint(window, &ps);
	RECT rt;
	GetClientRect(window, &rt);
	DrawText(hdc, message.c_str(), message.size(), &rt, DT_LEFT);
	EndPaint(window, &ps);
}

void CloseWindow() {
	delete m_network;
	PostQuitMessage(0);
}


void HandleSocketEvent(LPARAM lParam, WPARAM wParam) {
	switch (WSAGETSELECTEVENT(lParam)) {
	case FD_ACCEPT:
		{
			printf("FD_ACCEPT");
			sockaddr_in clientAddr;
			int addrSize = sizeof(clientAddr);
			SOCKET clientSocket = accept(m_network->m_serverTCPSocket, (sockaddr *) &clientAddr, &addrSize);
			if (clientSocket == INVALID_SOCKET) {
				printf("accept failed\n");
				break;
			}
			if (WSAAsyncSelect(m_network->m_serverTCPSocket, m_window, WM_SOCKET, FD_CLOSE | FD_CONNECT | FD_READ | FD_ACCEPT) == SOCKET_ERROR) {
				std::cerr << "WSAAsyncSelect failed\n";
			}
			
			Client* client = m_network->RegisterClient(clientSocket, clientAddr);


			NetMessage message;
			message.m_type = NetMessage::Type::SET_CLIENT_ID;
			message.m_clientID = client->m_id;

			//send(clientSocket, (char *) &message, sizeof NetMessage, 0);

			m_network->QueueTCPMessage(message, client->m_id);
		}
		break;

	case FD_CLOSE:
		{
			ClientID clientID = m_network->GetClientID((SOCKET) wParam);
			if (clientID != -1) {
				printf("Client [%d] disconected (SOCKET %d)\n", clientID, (SOCKET) wParam);
				m_network->RemoveClient(clientID, (SOCKET) wParam);
			}
		}
		break;

	case FD_READ:
		{
			printf("FD_READ on SOCKET %d\n", (SOCKET) wParam);
			if (m_network->m_serverUDPSocket == (SOCKET) wParam) {
				printf("New UDP message\n");
				m_network->ReadUDP();
				m_network->WriteUDP();
			} else if (m_network->m_serverTCPSocket == (SOCKET) wParam) {
				printf("New TCP message\n");
			} else {
				std::unordered_map<SOCKET, ClientID>* clientSocket = m_network->GetClientSocket();
				for (auto& itr = clientSocket->begin(); itr != clientSocket->end(); ++itr) {
					if (itr->first == (SOCKET) wParam) {
						if (m_network->ReadTCP((SOCKET) wParam)) {
							break;
						}
					}
				}
			}
		}
		break;
	case FD_WRITE:
		{
			printf("FD_WRITE on SOCKET %d\n", (SOCKET) wParam);
			if (m_network->m_serverUDPSocket == (SOCKET) wParam) {
				m_network->WriteUDP();
			} else {
				std::unordered_map<SOCKET, ClientID>* clientSocket = m_network->GetClientSocket();
				for (auto& itr = clientSocket->begin(); itr != clientSocket->end(); ++itr) {
					if (itr->first == (SOCKET) wParam) {
						if (m_network->WriteTCP(itr->second)) {
							break;
						}
					}
				}
			}
		}
		break;
	}
}
