#include "Client.h"
#include "Packet.h"

#include <enet/enet.h>
#include <iostream>
#include <conio.h>
#include <vector>
#include <string>
#include <thread>

using std::cout; using std::cin;
using std::endl;
using std::vector; using std::string;

void Client::InitializeEnet()
{
	if (enet_initialize() < 0)
	{
		cout << "Failed to initialize enet." << endl;
		exit(EXIT_FAILURE);
	}

	atexit(enet_deinitialize);
}

void Client::PromptName()
{
	std::cout << endl << "Enter a name: ";
	getline(cin, m_name);
}

void Client::DrawScreen()
{
	int logSize = 0;
	int inputQueueSize = 0;
	bool drawing = true;

	while (drawing)
	{
		if ((m_outputLog.size() > logSize) ||
			m_inputQueue.size() != inputQueueSize)
		{
			system("cls");
			for (string message : m_outputLog)
				cout << message << endl;
			cout << endl;

			for (char c : m_inputQueue)
				cout << c;

			logSize = m_outputLog.size();
			inputQueueSize = m_inputQueue.size();
		}
	}
}

void Client::KbListen()
{
	bool listening = true;
	while (listening)
	{
		if (_kbhit())
		{
			const int kBackspace = 8;
			const int kEnter = 13;
			char keyPressed = _getch();

			if (keyPressed >= '0' && keyPressed <= '9')
			{
				m_inputQueue.push_back(keyPressed);
			}
			
			if (keyPressed == kBackspace && m_inputQueue.size() > 0)
			{
				m_inputQueue.pop_back();
			}

			if (keyPressed == kEnter)
			{
				SendGuess();
				m_inputQueue.erase(m_inputQueue.begin(), m_inputQueue.end());
			}
		}
	}
}

void Client::StartThreads()
{
	if (m_inputThread == nullptr)
		m_inputThread = new std::thread(&Client::KbListen, this);
	
	if (m_drawThread == nullptr)
		m_inputThread = new std::thread(&Client::DrawScreen, this);
}

void Client::SendName()
{
	Packet namePacket(PacketType::NAME, m_name.c_str());
	namePacket.SendToPeer(m_server);
}

void Client::HandleReceive(ENetEvent* e)
{
	Packet receivedPacket(e);
	if (receivedPacket.GetType() == PacketType::MESSAGE)
		DisplayMessage(receivedPacket.GetData());
}

void Client::HandleDisconnect(ENetEvent* e)
{
	m_outputLog.push_back("Disconnected from server.");
}

void Client::SendGuess()
{
	char* guess;
	// build the guess based off of the content of m_inputQueue
	guess = new char[m_inputQueue.size()];

	for (int i = 0; i < m_inputQueue.size(); i++)
		guess[i] = m_inputQueue[i];
	guess[m_inputQueue.size()] = '\0';

	Packet guessPacket(PacketType::GUESS, guess);
	guessPacket.SendToPeer(m_server);
}

void Client::DisplayMessage(char* message)
{
	m_outputLog.push_back(message);
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
		case ENET_EVENT_TYPE_RECEIVE:
			HandleReceive(&event);
			break;
		case ENET_EVENT_TYPE_DISCONNECT:
			HandleDisconnect(&event);
			running = false;
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
	PromptName();
	InitializeEnet();

	m_client = enet_host_create(NULL, 1, 2, 0, 0);
	if (!m_client) exit(EXIT_FAILURE);

	m_server = enet_host_connect(m_client, &m_serverAddress, 2, 0);
	if (!m_server) exit(EXIT_FAILURE);

	ENetEvent event;
	if (enet_host_service(m_client, &event, 10000) > 0 &&
		event.type == ENET_EVENT_TYPE_CONNECT)
	{
		m_outputLog.push_back("Connected!");
	}
	else
	{
		cout << "Unable to connect to server :( " << endl;
	}

	StartThreads();

	SendName();

	ClientLoop();

	enet_host_destroy(m_client);
}
