#pragma once

#include <enet/enet.h>
#include <thread>
#include <vector>
#include <string>

using std::vector; using std::string;

class Client
{
	ENetAddress m_serverAddress;

	string m_name;

	std::thread* m_inputThread = nullptr;
	std::thread* m_drawThread = nullptr;
	

	ENetHost* m_client = nullptr;
	ENetPeer* m_server = nullptr;

	vector<string> m_outputLog;
	vector<char> m_inputQueue;

	void InitializeEnet();

	void PromptName();

	void DrawScreen();
	void KbListen();
	void StartThreads();

	void SendName();
	void HandleReceive(ENetEvent* e);
	void HandleDisconnect(ENetEvent* e);

	void SendGuess();
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

