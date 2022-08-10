#include "Client.h"
#include <enet/enet.h>
#include <iostream>

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
}

void Client::StartThreads()
{
}

void Client::HandleConnect()
{
}

void Client::HandleReceive()
{
}

void Client::HandleDisconnect()
{
}

void Client::SendGuess()
{
}

void Client::DisplayMessage()
{
}

void Client::ClientLoop()
{
}

void Client::SetAddress()
{
}

void Client::StartClient()
{
}
