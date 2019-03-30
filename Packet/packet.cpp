#include <iostream>
#include <enet/enet.h>
#include <cstring>
#include <string>
#include "packet.h"
#include "../Utils/utils.h"

using namespace std;

void Packet::getType() {

}

void Packet::Send(ENetPeer *peer, PacketData *data) {
	ENetPacket *packet = enet_packet_create(data->data, data->length, ENET_PACKET_FLAG_RELIABLE);
	enet_peer_send(peer, 0, packet);
}

void Packet::OnConsoleMessage(ENetPeer *peer, string data) {
	PacketData *sendData = this->PacketEnd(this->AppendString(this->AppendString(this->CreatePacket(), "OnConsoleMessage"), data));
	this->Send(peer, sendData);
}

void Packet::Disconnect(ENetPeer *peer) {
	enet_peer_disconnect_later(peer, 0);
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
	memcpy(packData, &num, 4);
	if (data != NULL)
	{
		memcpy(packData + 4, data, len);
	}
	char zero = 0;
	memcpy(packData + 4 + len, &zero, 1);
	PData->data = packData;
	PData->length = len + 5;
	return PData;
}

PacketData *Packet::CreatePacket() {
	Utils util;
	unsigned char* data = new unsigned char[61];
	string asdf = "0400000001000000FFFFFFFF00000000080000000000000000000000000000000000000000000000000000000000000000000000000000000000000000";
	for (int i = 0; i < asdf.length(); i += 2)
	{
		char x = util.HexDec(asdf[i]);
		x = x << 4;
		x += util.HexDec(asdf[i + 1]);
		memcpy(data + (i / 2), &x, 1);
		if (asdf.length() > 61 * 2) throw 0;
	}
	PacketData *packet = new PacketData();
	packet->data = data;
	packet->length = 61;
	packet->indexes = 0;
	return packet;
}

PacketData *Packet::AppendString(PacketData *p, string str) {
	//p.data[56] += 1;
	unsigned char* n = new unsigned char[p->length + 2 + str.length() + 4];
	memcpy(n, p->data, p->length);
	delete p->data;
	p->data = n;
	n[p->length] = p->indexes;
	n[p->length + 1] = 2;
	int sLen = str.length();
	memcpy(n + p->length + 2, &sLen, 4);
	memcpy(n + p->length + 6, str.c_str(), sLen);
	p->length = p->length + 2 + str.length() + 4;
	p->indexes++;
	return p;
}

PacketData *Packet::PacketEnd(PacketData *p) {
	unsigned char* n = new unsigned char[p->length + 1];
	memcpy(n, p->data, p->length);
	delete p->data;
	p->data = n;
	char zero = 0;
	memcpy(p->data + p->length, &zero, 1);
	p->length += 1;
	//*(int*)(p.data + 52) = p.len;
	*(int*)(p->data + 56) = p->indexes;//p.len-60;//p.indexes;
	*(unsigned char*)(p->data + 60) = p->indexes;
	//*(p.data + 57) = p.indexes;
	return p;
}