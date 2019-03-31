#include <iostream>
#include "player.h"
#include <enet/enet.h>
#include "../Packet/packet.h"

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