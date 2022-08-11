#include "Packet.h"
#include <enet/enet.h>
#include <string>

using std::string;

Packet::Packet(char* packetData)
{
    PacketData pd = *reinterpret_cast<PacketData*>(packetData);

    m_type = PacketType(pd.type);
    m_dataLength = pd.dataLength;
    m_data = new char[m_dataLength];
    memcpy_s(m_data, m_dataLength, pd.data, m_dataLength);
}

Packet::Packet(ENetEvent* e)
{
    PacketData pd = *reinterpret_cast<PacketData*>(e->packet->data);

    m_type = PacketType(pd.type);
    m_dataLength = pd.dataLength;
    m_data = new char[m_dataLength];
    memcpy_s(m_data, m_dataLength, pd.data, m_dataLength);
}

void Packet::SendToPeer(ENetPeer* p)
{
    PacketData pd;
    pd.type = m_type;
    pd.dataLength = m_dataLength;

    memcpy_s(pd.data, m_dataLength, m_data, m_dataLength);

    ENetPacket* packetToSend = enet_packet_create(reinterpret_cast<void*>(&pd), sizeof(pd), ENET_PACKET_FLAG_RELIABLE);
    enet_peer_send(p, 0, packetToSend);
}

void Packet::Broadcast(ENetHost* server)
{
    PacketData pd;
    pd.type = m_type;
    pd.dataLength = m_dataLength;

    memcpy_s(pd.data, m_dataLength, m_data, m_dataLength);

    ENetPacket* packetToSend = enet_packet_create(reinterpret_cast<void*>(&pd), sizeof(pd), ENET_PACKET_FLAG_RELIABLE);
    enet_host_broadcast(server, 0, packetToSend);
}
