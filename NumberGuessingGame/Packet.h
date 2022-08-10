#pragma once
#include <enet/enet.h>

enum class PacketType : char
{
	GUESS = 'g',
};

class Packet
{
	PacketType m_type;
	char m_dataLength = 0;
	char* m_data = nullptr;

public:
	Packet();

	void SetType(PacketType type) { m_type = type; }
	PacketType GetType() { return m_type; }

	void setData(char* data)
	{
		m_dataLength = strlen(m_data);
		m_data = data;
	}

	char* Serialize();
	void Deserialize(char* serializedPacket);
	void SendToPeer(ENetPeer* p);
	void Broadcast();
};