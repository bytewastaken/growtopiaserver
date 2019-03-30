#include <iostream>
#include "player.h"
#include <enet/enet.h>
#include "../Packet/packet.h"

using namespace std;

void Player::SpawnPlayer(ENetPeer *peer, string type) {
	Packet packet;
	string data = "spawn|avatar\nnetID|1\nuserID|1000293332\ncolrect|0|0|20|30\nposXY|50|20\nname|```5@ColdChip``\ncountry|ru\ninvis|0\nmstate|0\nsmstate|0\n" + type;
	PacketData *PData = packet.PacketEnd(packet.AppendString(packet.AppendString(packet.CreatePacket(), "OnSpawn"), data));
	packet.Send(peer, PData);
}