#include <iostream>
#include <enet/enet.h>
#include <cstring>
#include "packet.h"
#include "../Utils/utils.h"

using namespace std;

void Packet::getType() {

}

void Packet::Send(ENetPeer *peer, PacketData *data) {
	unsigned char *Packet = new unsigned char[sizeof(data->data) + 5];
	int zero = 0;
	memcpy(Packet, &data->type, 4);
	memcpy(Packet + 4, data->data, data->length);
	memcpy(Packet + 4 + data->length, &zero, 1);
	ENetPacket *packet = enet_packet_create(Packet, sizeof(Packet), ENET_PACKET_FLAG_RELIABLE);
	enet_peer_send(peer, 0, packet);
}

PacketData *Packet::CreateOnConnectPacket() {
	PacketData *data = new PacketData();
	data = this->MakeRawPacket(1, 0, 0);
	return data;
}

PacketData *Packet::Unpack(ENetPacket *packet) {
	Utils u;
	PacketData *data = new PacketData();
	data->type = *packet->data;
	data->data = packet->data + 4;
	data->length = packet->dataLength - 4;
	return data;
}

PacketData *Packet::MakeRawPacket(int num, unsigned char* data, int len) {
	unsigned char *packData = new unsigned char[len + 1];
	PacketData *PData = new PacketData();
	PData->type = num;
	if (data != NULL)
	{
		memcpy(packData, data, len);
	}
	char zero = 0;
	memcpy(packData, &zero, 1);
	PData->data = packData;
	PData->length = len + 1;
	return PData;
}