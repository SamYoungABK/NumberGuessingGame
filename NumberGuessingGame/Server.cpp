#include "Server.h"
#include <enet/enet.h>
#include <iostream>
#include <string>


using std::cout; using std::endl;

void Server::InitialieEnet()
{
	if (enet_initialize() < 0)
	{
		cout << "Failed to initialize enet." << endl;
		exit(EXIT_FAILURE);
	}
}

void Server::HandleConnect(ENetEvent* e)
{
}

void Server::HandleReceive(ENetEvent* e)
{
}

void Server::HandleDisconnect(ENetEvent* e)
{
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

void Server::RandomizeNumber()
{
}

void Server::NewGame()
{
}

void Server::ServerLoop()
{
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
	InitialieEnet();
	
}
