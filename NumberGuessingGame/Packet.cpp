#include "Packet.h"
#include <enet/enet.h>


Packet::Packet()
{
}

char* Packet::Serialize()
{
    char* serializedPacket;
    int packetSize;

    packetSize = sizeof(m_type) +
        sizeof(m_dataLength) +
        strlen(m_data);

    serializedPacket = new char[packetSize+1];

    memcpy_s(&serializedPacket+0, 1, &m_type, 1);
    memcpy_s(&serializedPacket+1, 1, &m_dataLength, 1);
    memcpy_s(&serializedPacket+2, m_dataLength, &m_data, m_dataLength);

    return serializedPacket;
}

void Packet::Deserialize(char* serializedPacket)
{
}

void Packet::SendToPeer(ENetPeer* p)
{
}

void Packet::Broadcast()
{
}
