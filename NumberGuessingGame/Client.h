#pragma once

#include <enet/enet.h>
#include <thread>

class Client
{
	std::thread* m_inputThread = nullptr;
	ENetAddress m_serverAddress;

	ENetHost* client;

	void InitializeEnet();
	void KbListen();
	void StartThreads();

	void HandleConnect();
	void HandleReceive();
	void HandleDisconnect();

	void SendGuess();
	void DisplayMessage();

	void ClientLoop();
public:
	Client()
	{
		enet_address_set_host(&m_serverAddress, "localhost");
		m_serverAddress.port = 1234;
	}

	Client(ENetAddress addr) :
		m_serverAddress(addr)
	{}

	void SetAddress();
	void StartClient();
};

