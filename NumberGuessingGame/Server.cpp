#include "Server.h"
#include <enet/enet.h>
#include <iostream>
#include <string>
#include <random>
#include <time.h>
#include "Packet.h"


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
	
	cout << peerData->name << " connected from " <<
		peerData->address.host << ':' <<
		peerData->address.port << endl;
	
	SendInitialGuessPrompt(e->peer);
}

void Server::HandleReceive(ENetEvent* e)
{
	Packet packetReceived(e);

	if (packetReceived.GetType() == PacketType::GUESS)
	{
		HandleGuess(e);
	}
}

void Server::HandleDisconnect(ENetEvent* e)
{
	PeerData* peerData = reinterpret_cast<PeerData*>(e->peer->data);
	cout << peerData->name << " disconnected." << endl;
	
	delete e->peer->data;
	e->peer->data = nullptr;
}

void Server::SendInitialGuessPrompt(ENetPeer* p)
{
	Packet packetToSend(PacketType::MESSAGE, "Guess the number: ");
	packetToSend.SendToPeer(p);
}

void Server::HandleGuess(ENetEvent* e)
{
	Packet packetReceived(e);
	char* packetData = packetReceived.GetData();
	int packetLength = strlen(packetData);

	int guess = strtol(packetData, &(packetData)+packetLength, 0);
	
	guess == m_winningNumber ?
		RespondCorrectGuess(e, packetData) :
		RespondIncorrectGuess(e, packetData);
}

void Server::RespondIncorrectGuess(ENetEvent* e, char* guess)
{
	PeerData* clientData = reinterpret_cast<PeerData*>(e->peer->data);
	string guesserName = clientData->name;
	string message = guesserName + " incorrectly guessed " + guess + ".";
	Packet broadcastGuess(PacketType::MESSAGE, "");
	broadcastGuess.Broadcast(m_server);
}

void Server::RespondCorrectGuess(ENetEvent* e, char* guess)
{
	PeerData* clientData = reinterpret_cast<PeerData*>(e->peer->data);
	string guesserName = clientData->name;
	string message = guesserName + " correctly guessed " + guess + ".";
	Packet broadcastGuess(PacketType::MESSAGE, "");
	broadcastGuess.Broadcast(m_server);
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
