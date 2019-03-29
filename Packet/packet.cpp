#include <iostream>
#include <enet/enet.h>
#include <cstring>
#include "packet.h"

using namespace std;

void Packet::getType() {

}

void Packet::Send(ENetPeer *peer, PacketData *data) {
	ENetPacket *packet = enet_packet_create(data->data, sizeof(data->data), ENET_PACKET_FLAG_RELIABLE);
	enet_peer_send(peer, 0, packet);
}

PacketData *Packet::CreateOnConnectPacket() {
	PacketData *data = new PacketData();
	data->data = this->MakeRawPacket(1, 0, 0);
	return data;
}

unsigned char *Packet::MakeRawPacket(int num, unsigned char* data, int len) {
	unsigned char *PacketData = new unsigned char[len + 5];
	memcpy(PacketData, &num, 4);
	if (data != NULL)
	{
		memcpy(PacketData+4, data, len);
	}
	char zero = 0;
	memcpy(PacketData + 4 + len, &zero, 1);
	return PacketData;
}