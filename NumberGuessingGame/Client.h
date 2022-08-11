#pragma once

#include <enet/enet.h>
#include <thread>

class Client
{
	std::thread* m_inputThread = nullptr;
	ENetAddress m_serverAddress;

	ENetHost* m_client = nullptr;
	ENetPeer* m_server = nullptr;

	void InitializeEnet();
	void KbListen();
	void StartThreads();

	void HandleConnect(ENetEvent* e);
	void HandleReceive(ENetEvent* e);
	void HandleDisconnect(ENetEvent* e);

	void SendGuess(ENetEvent* e);
	void DisplayMessage(char* message);

	void ClientLoop();
public:
	Client()
	{
		enet_address_set_host_ip(&m_serverAddress, "127.0.0.1");
		m_serverAddress.port = 1234;
	}

	Client(ENetAddress addr) :
		m_serverAddress(addr)
	{}

	void SetAddress(char* address, int port);
	void StartClient();
};

