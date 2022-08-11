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
	PeerData* peerData = new PeerData(); 
	e->peer->data = reinterpret_cast<void*>(peerData);
	enet_peer_timeout(e->peer, 1000, 1000, 1500);

	peerData->address = e->peer->address;
	
	cout << "User connected from " <<
		peerData->address.host << ':' <<
		peerData->address.port << endl;
	
	SendInitialGuessPrompt(e->peer);
}

void Server::HandleReceive(ENetEvent* e)
{
	Packet packetReceived(e);

	switch (packetReceived.GetType())
	{
	case PacketType::GUESS: HandleGuess(e); break;
	case PacketType::NAME: HandleName(e); break;
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
	Packet pack(PacketType::MESSAGE, "Guess the number: ");
	
	pack.SendToPeer(p);
}

void Server::HandleGuess(ENetEvent* e)
{
	Packet packetReceived(e);
	char* packetData = packetReceived.GetData();
	int packetLength = sizeof(packetData);

	int guess = strtol(packetData, &(packetData)+packetLength, 0);
	
	guess == m_winningNumber ?
		RespondCorrectGuess(e, packetData) :
		RespondIncorrectGuess(e, packetData);
}

void Server::HandleName(ENetEvent* e)
{
	Packet packetReceived(e);
	PeerData* peerData = reinterpret_cast<PeerData*>(e->peer->data);

	peerData->name = packetReceived.GetData();
}

void Server::RespondIncorrectGuess(ENetEvent* e, char* guess)
{
	PeerData* clientData = reinterpret_cast<PeerData*>(e->peer->data);

	string guesserName = clientData->name;
	string message = guesserName + " incorrectly guessed " + guess + ".";

	Packet broadcastGuess(PacketType::MESSAGE, message.c_str());
	broadcastGuess.Broadcast(m_server);
}

void Server::RespondCorrectGuess(ENetEvent* e, char* guess)
{
	PeerData* clientData = reinterpret_cast<PeerData*>(e->peer->data);
	clientData->wins++;
	string guesserName = clientData->name;
	string message = "\n******WINNER!!!!*****\n"
		+ guesserName + " correctly guessed " + guess + ".\n" +
		guesserName + " has won " + std::to_string(clientData->wins) + " times!";

	Packet broadcastGuess(PacketType::MESSAGE, message.c_str());
	broadcastGuess.Broadcast(m_server);

	m_numberGuessed = true;
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
	string message = "A new game has begun. Number has been randomized!\n";
	Packet newGameMessage(PacketType::MESSAGE, message.c_str());
	newGameMessage.Broadcast(m_server);
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
		enet_host_service(m_server, &event, 500);

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
	// can be overwritten with Server::SetAddress()
	m_address.host = ENET_HOST_ANY;
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
	if (!m_server) exit(EXIT_FAILURE);

	NewGame();
	ServerLoop();
	
	enet_host_destroy(m_server);
}
