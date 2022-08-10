#include "Packet.h"
#include <enet/enet.h>

Packet::Packet(char* serializedPacket)
{
    Deserialize(serializedPacket);
}

Packet::Packet(ENetEvent* e)
{
    char* serializedPacket;
    int length = e->packet->dataLength;
    serializedPacket = new char[e->packet->dataLength];
    memcpy_s(serializedPacket, length, e->packet->data, length);

    Deserialize(serializedPacket);
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
    serializedPacket[packetSize] = '\0';

    return serializedPacket;
}

void Packet::Deserialize(char* serializedPacket)
{
    char* data;
    int packetSize;
    
    m_type = PacketType(serializedPacket[0]);
    m_dataLength = int(serializedPacket[1]);
    
    delete m_data;
    m_data = new char[m_dataLength];

    memcpy_s(&m_data, m_dataLength, &serializedPacket[2], m_dataLength);
}

void Packet::SendToPeer(ENetPeer* p)
{
    ENetPacket* packetToSend = enet_packet_create(m_data, m_dataLength, ENET_PACKET_FLAG_RELIABLE);
    enet_peer_send(p, 0, packetToSend);
}

void Packet::Broadcast(ENetHost* server)
{
    ENetPacket* packetToSend = enet_packet_create(m_data, m_dataLength, ENET_PACKET_FLAG_RELIABLE);
    enet_host_broadcast(server, 0, packetToSend);
}
