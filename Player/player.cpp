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

void Player::UpdateDB(string fileName, PlayerDB *db) {
	Utils util;
	char *data = new char[2];
	memcpy(data, &db->isAdmin, 1);
	memcpy(data, &db->isDev, 1);
	util.WriteFile(fileName, data, 2);
}