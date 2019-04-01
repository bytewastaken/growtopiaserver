#include <iostream>
#include <string>
#include <enet/enet.h>
#include "../Packet/packet.h"
#include "../Utils/utils.h"

using namespace std;

void Player::SpawnPlayer(ENetPeer *peer, string netID, int x, int y, string username, string type) {
	Packet packet;
	string data = "spawn|avatar\nnetID|" + netID + "\nuserID|1000293332\ncolrect|0|0|20|30\nposXY|" + std::to_string(x) + "|" + std::to_string(y) + "\nname|``" + username + "``\ncountry|ru\ninvis|0\nmstate|0\nsmstate|0\n" + type;
	PacketData *PData = packet.PacketEnd(packet.AppendString(packet.AppendString(packet.CreatePacket(), "OnSpawn"), data));
	packet.Send(peer, PData);
}

void Player::RemovePlayer(ENetPeer *peer, string netID) {
	Packet packet;
	string data = "netID|" + netID + "\n";
	PacketData *PData = packet.PacketEnd(packet.AppendString(packet.AppendString(packet.CreatePacket(), "OnRemove"), data));
	packet.Send(peer, PData);
}

bool Player::isHere(ENetPeer *peer, ENetPeer *peer2) {
	return ((PlayerInfo*)(peer->data))->currentWorld == ((PlayerInfo*)(peer2->data))->currentWorld;
}

void Player::BroadcastOnPlayerEnter(ENetPeer *peer, ENetHost *users) {
	ENetPeer *currentPeer;
	for(currentPeer = users->peers; currentPeer < &users->peers[users->peerCount]; ++currentPeer) {
		if(peer != currentPeer && currentPeer->state == ENET_PEER_STATE_CONNECTED) {
			if(this->isHere(peer, currentPeer) == true) {
				string currentNetID = std::to_string(((PlayerInfo*)(currentPeer->data))->netID);
				this->SpawnPlayer(peer, currentNetID, ((PlayerInfo*)(currentPeer->data))->x, ((PlayerInfo*)(currentPeer->data))->y, ((PlayerInfo*)(currentPeer->data))->displayName, TYPE_NON_LOCAL);
				this->SpawnPlayer(currentPeer, std::to_string(((PlayerInfo*)(peer->data))->netID), ((PlayerInfo*)(currentPeer->data))->x, ((PlayerInfo*)(currentPeer->data))->y, ((PlayerInfo*)(peer->data))->displayName, TYPE_NON_LOCAL);
			}
		}
	}
}

void Player::BroadcastOnPlayerLeave(ENetPeer *peer, ENetHost *users) {
	Packet p;
	string currentNetID = std::to_string(((PlayerInfo*)(peer->data))->netID);
	ENetPeer *currentPeer;
	for(currentPeer = users->peers; currentPeer < &users->peers[users->peerCount]; ++currentPeer) {
		if(currentPeer->state == ENET_PEER_STATE_CONNECTED) {
			if(this->isHere(peer, currentPeer) == true) {
				this->RemovePlayer(currentPeer, currentNetID);
				p.OnConsoleMessage(currentPeer, "`5<`w" + ((PlayerInfo*)(peer->data))->displayName + " `5left, x others here>");
				p.OnTalkBubble(currentPeer, ((PlayerInfo*)(peer->data))->netID, "`5<`w" + ((PlayerInfo*)(peer->data))->displayName + " `5left, x others here>");
			}
		}
	}
}

void Player::BroadcastLocation(ENetPeer *peer, ENetHost *users, PlayerMoving *PMov) {
	Packet p;
	ENetPeer *currentPeer;
	for(currentPeer = users->peers; currentPeer < &users->peers[users->peerCount]; ++currentPeer) {
		if(peer != currentPeer && currentPeer->state == ENET_PEER_STATE_CONNECTED) {
			if(this->isHere(peer, currentPeer) == true) {
				PMov->netID = ((PlayerInfo*)(peer->data))->netID;
				PacketData *PData = p.PackPlayerMoving(PMov);
				p.Send(currentPeer, PData);
			}
		}
	}
}

void Player::BroadcastTileChange(ENetPeer *peer, ENetHost *users, PlayerMoving *PMov) {
	Packet p;
	ENetPeer *currentPeer;
	for(currentPeer = users->peers; currentPeer < &users->peers[users->peerCount]; ++currentPeer) {
		if(currentPeer->state == ENET_PEER_STATE_CONNECTED) {
			if(this->isHere(peer, currentPeer) == true) {
				PMov->netID = ((PlayerInfo*)(peer->data))->netID;
				PacketData *PData = p.PackPlayerMoving(PMov);
				p.Send(currentPeer, PData);
			}
		}
	}
}

void Player::UpdateDB(string fileName, PlayerDB *db) {
	Utils util;
	char *data = new char[2];
	memcpy(data, &db->isAdmin, 1);
	memcpy(data, &db->isDev, 1);
	util.WriteFile(fileName, data, 2);
}

void Player::SendInventory(ENetPeer *peer) {
	Utils util;
	Packet packet;
	string asdf2 = "0400000009A7379237BB2509E8E0EC04F8720B050000000000000000FBBB0000010000007D920100FDFDFDFD04000000040000000000000000000000000000000000";
	int inventoryLen = 200;
	int packetLen = (asdf2.length() / 2) + (inventoryLen * 4) + 4;
	unsigned char* data2 = new unsigned char[packetLen];
	for (int i = 0; i < asdf2.length(); i += 2)
	{
		char x = util.HexDec(asdf2[i]);
		x = x << 4;
		x += util.HexDec(asdf2[i + 1]);
		memcpy(data2 + (i / 2), &x, 1);
	}
	// int endianInvVal = _byteswap_ulong(inventoryLen);
	int endianInvVal = __builtin_bswap32(inventoryLen);
	memcpy(data2 + (asdf2.length() / 2) - 4, &endianInvVal, 4);
	// endianInvVal = _byteswap_ulong(inventory.inventorySize);
	endianInvVal = __builtin_bswap32(inventoryLen);
	memcpy(data2 + (asdf2.length() / 2) - 8, &endianInvVal, 4);
	int val = 0;
	for (int i = 0; i < inventoryLen; i++)
	{
		val = 0;
		val |= i * 2;
		val |= 200 << 16;
		val &= 0x00FFFFFF;
		val |= 0x00 << 24;
		memcpy(data2 + (i*4) + (asdf2.length() / 2), &val, 4);
	}
	PacketData *sendData = new PacketData();
	sendData->data = data2;
	sendData->length = packetLen;
	packet.Send(peer, sendData);
	delete data2;
}