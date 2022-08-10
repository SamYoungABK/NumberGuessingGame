#pragma once
#include <enet/enet.h>
#include <string>

using std::string;

class Server
{
	int m_winningNumber = 0;
	bool m_numberGuessed = false;
	ENetAddress m_address;
	ENetHost* m_server = nullptr;

	struct PeerData
	{
		string name;
		ENetAddress address;
		int guessCount = 0;
	};

	void InitialieEnet();

	void HandleConnect(ENetEvent* e);
	void HandleReceive(ENetEvent* e);
	void HandleDisconnect(ENetEvent* e);
	
	void HandleGuess(ENetPeer* p, int guess);
	void RespondIncorrectGuess(ENetPeer* p);
	void RespondCorrectGuess(ENetPeer* p);

	int RandomizeNumber(int lowRange, int highRange);
	void NewGame();
	void ServerGameTick();

	void ServerLoop();
public:
	Server();

	void SetAddress(char* address, int port);
	void StartServer();
};

