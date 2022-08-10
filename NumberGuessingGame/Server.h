#pragma once
#include <enet/enet.h>




class Server
{
	int m_winningNumber = 0;
	ENetAddress m_address;

	void InitialieEnet();

	void HandleConnect(ENetEvent* e);
	void HandleReceive(ENetEvent* e);
	void HandleDisconnect(ENetEvent* e);
	
	void HandleGuess(ENetPeer* p, int guess);
	void RespondIncorrectGuess(ENetPeer* p);
	void RespondCorrectGuess(ENetPeer* p);

	void RandomizeNumber();
	void NewGame();

	void ServerLoop();
public:
	Server();

	void SetAddress(char* address, int port);
	void StartServer();
};

