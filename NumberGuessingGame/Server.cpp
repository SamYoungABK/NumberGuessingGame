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

void Server::StartServer()
{
}
