#include <iostream>
#include <cstring>
#include "command.h"
#include <enet/enet.h>
#include "../Packet/packet.h"

using namespace std;

void Command::SendTextToAllUsersInWorld(ENetPeer *sender, ENetHost *users, string TextData) {
	Player player;
	Packet p;
	ENetPeer *currentPeer;
	for(currentPeer = users->peers; currentPeer < &users->peers[users->peerCount]; ++currentPeer) {
		if(currentPeer->state == ENET_PEER_STATE_CONNECTED) {
			if(player.isHere(sender, currentPeer) == true) {
				string SenderUsername = ((PlayerInfo*)(sender->data))->displayName;
				p.OnConsoleMessage(currentPeer, "<" + SenderUsername + "> " + TextData);
				p.OnTalkBubble(currentPeer, ((PlayerInfo*)(sender->data))->netID, TextData);
			}
		}
	}
}

void Command::ProccessCommand(ENetPeer *peer, ENetHost *users, string command) {
	Packet p;
	if(command == "/fuckyou") {
		p.OnBanMessage(peer);
	} else if(command == "/restart") {
		p.OnRestartMessage(peer);
	} else {
		this->SendTextToAllUsersInWorld(peer, users, command);
	}

}