#pragma once
#include <enet/enet.h>

enum class PacketType : char
{
	MESSAGE = 'm',
	GUESS = 'g',
};

class Packet
{
	PacketType m_type;
	char m_dataLength = 0;
	char* m_data = nullptr;

public:
	Packet() :
		m_type(PacketType::GUESS),
		m_dataLength(0),
		m_data(nullptr)
	{};

	Packet(PacketType type, const char* data) :
		m_type(type),
		m_dataLength(strlen(data) + 1)
	{
		m_data = new char[m_dataLength];
		memcpy_s(&m_data, sizeof(m_data), &data, sizeof(data));
	};

	Packet(PacketType type, char* data) :
		m_type(type),
		m_dataLength(strlen(data) + 1),
		m_data(data)
	{};

	Packet(char* serializedPacket);
	Packet(ENetEvent* e);

	void SetType(PacketType type) { m_type = type; }
	PacketType GetType() { return m_type; }

	void setData(char* data)
	{
		m_dataLength = strlen(m_data);
		m_data = data;
	}

	char* GetData() { return m_data; }

	char* Serialize();
	void Deserialize(char* serializedPacket);
	void SendToPeer(ENetPeer* p);
	void Broadcast(ENetHost* server);
};