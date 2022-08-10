#include "Server.h"
#include <enet/enet.h>
#include <iostream>
#include <string>
#include <random>
#include <time.h>


using std::cout; using std::endl;
using std::time; using std::rand;

void Server::InitialieEnet()
{
	if (enet_initialize() < 0)
	{
		cout << "Failed to initialize enet." << endl;
		exit(EXIT_FAILURE);
	}

	atexit(enet_deinitialize);
}

void Server::HandleConnect(ENetEvent* e)
{
	PeerData* peerData = reinterpret_cast<PeerData*>(e->peer->data);
	peerData = new PeerData();

	peerData->name = (char*)(e->packet->data);
	peerData->address = e->peer->address;
	
	cout << "User connected from " << peerData->address.host <<
		':' << peerData->address.port << endl;
}

void Server::HandleReceive(ENetEvent* e)
{
}

void Server::HandleDisconnect(ENetEvent* e)
{
	cout << "User connected from " << peerAddress.host << ':' << peerAddress.port;
	
	delete e->peer->data;

	e->peer->data = nullptr;
}

void Server::HandleGuess(ENetPeer* p, int guess)
{
}

void Server::RespondIncorrectGuess(ENetPeer* p)
{
}

void Server::RespondCorrectGuess(ENetPeer* p)
{
}

int Server::RandomizeNumber(int lowRange, int highRange)
{
	int newNumber = (std::rand() % (highRange)) + lowRange+1;
	return newNumber;
}

void Server::NewGame()
{
	m_winningNumber = RandomizeNumber(1, 10);
	m_numberGuessed = false;
}

void Server::ServerGameTick()
{
	if (m_numberGuessed == true)
		NewGame();
}

void Server::ServerLoop()
{
	bool running = true;
	while (running)
	{
		ENetEvent event;
		enet_host_service(m_server, &event, 1000);

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
		ServerGameTick();
	}
}

Server::Server()
{
	// set default address and port for the server to bind to.
	// can be overwrittenwith SetAddress()
	enet_address_set_host_ip(&m_address, ENET_HOST_ANY);
	m_address.port = 1234;
}

void Server::SetAddress(char* address, int port)
{
	enet_address_set_host_ip(&m_address, address);
	m_address.port = 1234;
}

void Server::StartServer()
{
	srand(std::time(nullptr));

	InitialieEnet();

	m_server = enet_host_create(&m_address, 10, 2, 0, 0);
	NewGame();
	ServerLoop();
	
	enet_host_destroy(m_server);
}
