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
	delete(data->data);
}

void Packet::OnConsoleMessage(ENetPeer *peer, string data) {
	PacketData *sendData = this->PacketEnd(this->AppendString(this->AppendString(this->CreatePacket(), "OnConsoleMessage"), data));
	this->Send(peer, sendData);
}

void Packet::OnTalkBubble(ENetPeer *peer, int netID, string data) {
	PacketData *sendData = this->PacketEnd(this->AppendIntx(this->AppendString(this->AppendIntx(this->AppendString(this->CreatePacket(), "OnTalkBubble"), netID), data), 0));
	this->Send(peer, sendData);
}

void Packet::Disconnect(ENetPeer *peer) {
	enet_peer_disconnect_later(peer, 0);
}

void Packet::SendOnLoginPacket(ENetPeer *peer) {
	PacketData *sendData = this->PacketEnd(this->AppendString(this->AppendString(this->AppendString(this->AppendString(this->AppendInt(this->AppendString(this->CreatePacket(), "OnSuperMainStartAcceptLogonHrdxs47254722215a"), 1109100565), "ubistatic-a.akamaihd.net"), "0098/CDNContent3/cache/"), "lol.com"), "proto=42|choosemusic=audio/mp3/tsirhc.mp3|active_holiday=0|"));
	this->Send(peer, sendData);
}

void Packet::SendItemsDat(ENetPeer *peer) {
	Utils util;
	int itemsDatSize = util.FileSize("items.dat");
	unsigned char *itemsDat = new unsigned char[60 + itemsDatSize];
	for (int i = 0; i < ItemsDatPacketHeader.length(); i += 2)
	{
		char x = util.HexDec(ItemsDatPacketHeader[i]);
		x = x << 4;
		x += util.HexDec(ItemsDatPacketHeader[i + 1]);
		memcpy(itemsDat + (i / 2), &x, 1);
		if (ItemsDatPacketHeader.length() > 60 * 2) throw 0;
	}
	memcpy(itemsDat + 56, &itemsDatSize, 4);
	util.FileWriteToPointer("items.dat", itemsDat + 60, itemsDatSize);
	PacketData *sendData = new PacketData();
	sendData->data = itemsDat;
	sendData->length = 60 + itemsDatSize;
	this->Send(peer, sendData);
}

PacketData *Packet::CreateOnConnectPacket() {
	PacketData *data = new PacketData();
	data = this->MakeRawPacket(1, 0, 0);
	return data;
}

PacketData *Packet::UnpackText(ENetPacket *packet) {
	Utils u;
	PacketData *data = new PacketData();
	data->type = *packet->data;
	data->data = packet->data + 4;
	data->length = packet->dataLength - 4;
	int zero = 0;
	memcpy(packet->data + packet->dataLength - 1, &zero, 1);
	return data;
}

PacketData *Packet::UnpackBinary(ENetPacket *packet) {
	Utils u;
	PacketData *data = new PacketData();
	if(packet->dataLength >= 0x3C) {
		data->type = *packet->data;
		data->data = packet->data + 4;
		data->length = packet->dataLength - 4;
		int zero = 0;
		memcpy(packet->data + 56, &zero, 4);
	}
	return data;
}

PacketData *Packet::ProbeType(ENetPacket *packet) {
	Utils u;
	PacketData *data = new PacketData();
	data->type = *packet->data;
	data->data = 0;
	data->length = 0;
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
	for (int i = 0; i < PacketHeader.length(); i += 2)
	{
		char x = util.HexDec(PacketHeader[i]);
		x = x << 4;
		x += util.HexDec(PacketHeader[i + 1]);
		memcpy(data + (i / 2), &x, 1);
		if (PacketHeader.length() > 61 * 2) throw 0;
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

PacketData *Packet::AppendInt(PacketData *p, int val) {
	//p.data[56] += 1;
	unsigned char* n = new unsigned char[p->length + 2 + 4];
	memcpy(n, p->data, p->length);
	delete p->data;
	p->data = n;
	n[p->length] = p->indexes;
	n[p->length + 1] = 9;
	memcpy(n + p->length + 2, &val, 4);
	p->length = p->length + 2 + 4;
	p->indexes++;
	return p;
}

PacketData *Packet::AppendIntx(PacketData *p, int val) {
	//p.data[56] += 1;
	unsigned char* n = new unsigned char[p->length + 2 + 4];
	memcpy(n, p->data, p->length);
	delete p->data;
	p->data = n;
	n[p->length] = p->indexes;
	n[p->length + 1] = 5;
	memcpy(n + p->length + 2, &val, 4);
	p->length = p->length + 2 + 4;
	p->indexes++;
	return p;
}

PlayerMoving *Packet::UnpackPlayerMoving(PacketData *data) {
	PlayerMoving* dataStruct = new PlayerMoving;
	memcpy(&dataStruct->packetType, data->data, 4);
	memcpy(&dataStruct->netID, data->data + 4, 4);
	memcpy(&dataStruct->characterState, data->data + 12, 4);
	memcpy(&dataStruct->plantingTree, data->data + 20, 4);
	memcpy(&dataStruct->x, data->data + 24, 4);
	memcpy(&dataStruct->y, data->data + 28, 4);
	memcpy(&dataStruct->XSpeed, data->data + 32, 4);
	memcpy(&dataStruct->YSpeed, data->data + 36, 4);
	memcpy(&dataStruct->punchX, data->data + 44, 4);
	memcpy(&dataStruct->punchY, data->data + 48, 4);
	return dataStruct;
}

PacketData *Packet::PackPlayerMoving(PlayerMoving *dataStruct) {
	unsigned char* data = new unsigned char[61];
	PacketData *PData = new PacketData();
	for (int i = 0; i < 61; i++)
	{
		data[i] = 0;
	}
	int four = 4;
	memcpy(data, &four, 4);
	memcpy(data + 4, &dataStruct->packetType, 4);
	memcpy(data + 8, &dataStruct->netID, 4);
	memcpy(data + 16, &dataStruct->characterState, 4);
	memcpy(data + 24, &dataStruct->plantingTree, 4);
	memcpy(data + 28, &dataStruct->x, 4);
	memcpy(data + 32, &dataStruct->y, 4);
	memcpy(data + 36, &dataStruct->XSpeed, 4);
	memcpy(data + 40, &dataStruct->YSpeed, 4);
	memcpy(data + 48, &dataStruct->punchX, 4);
	memcpy(data + 52, &dataStruct->punchY, 4);
	PData->data = data;
	PData->length = 61;
	return PData;
}