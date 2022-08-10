#include "Client.h"
#include <enet/enet.h>
#include <iostream>
#include <conio.h>

using std::cout; using std::endl;

void Client::InitializeEnet()
{
	if (enet_initialize() < 0)
	{
		cout << "Failed to initialize enet." << endl;
		exit(EXIT_FAILURE);
	}

	atexit(enet_deinitialize);
}

void Client::KbListen()
{
	bool listening = true;
	while (listening)
	{
		if (_kbhit())
		{
			char keyPressed = _getch();
			if (keyPressed >= '0' && keyPressed <= '9')
			{

			}
		}
	}
}

void Client::StartThreads()
{
}

void Client::HandleConnect(ENetEvent* e)
{
}

void Client::HandleReceive(ENetEvent* e)
{
}

void Client::HandleDisconnect(ENetEvent* e)
{
}

void Client::SendGuess(ENetEvent* e)
{
}

void Client::DisplayMessage(ENetEvent* e)
{
}

void Client::ClientLoop()
{
	ENetEvent event;
	bool running = true;
	while (running)
	{
		enet_host_service(m_client, &event, 1000);
		switch (event.type)
		{
		case ENET_EVENT_TYPE_CONNECT:
			HandleConnect(&event);
			break;
		case ENET_EVENT_TYPE_RECEIVE:
			HandleReceive(&event);
			break;
		case ENET_EVENT_TYPE_DISCONNECT:
			HandleDisconnect(&event);
			break;
		}

	}
}

void Client::SetAddress(char* address, int port)
{
	enet_address_set_host(&m_serverAddress, address);
	m_serverAddress.port = 1234;
}

void Client::StartClient()
{
	InitializeEnet();

	m_client = enet_host_create(NULL, 1, 2, 0, 0);
	if (!m_client) exit(EXIT_FAILURE);

	m_server = enet_host_connect(m_client, &m_serverAddress, 2, 0);
	if (!m_server) exit(EXIT_FAILURE);

	ENetEvent event;
	if (enet_host_service(m_client, &event, 5000) > 0 &&
		event.type == ENET_EVENT_TYPE_CONNECT)
	{
		cout << "Connected!" << endl;
	}
	else
	{
		cout << "Unable to connect to server :( " << endl;
	}

	ClientLoop();

	enet_host_destroy(m_client);
}
