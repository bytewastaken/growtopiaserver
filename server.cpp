/*

@author ColdChip

*/

#include <enet/enet.h>
#include <iostream>
#include <cstdint>
#include <cstring>
#include <vector>
#include <stdint.h>
#include <string>
#include <fstream>
#include <utility>
#include "server.h"
#include "Packet/packet.h"

using namespace std;

ServerData *host = new ServerData();

Server ServerEvent;

Packet p;

void Server::DumpArray(unsigned char* data, int len) {
    for(int i = 0; i < len; i++) {
        printf("Index: %i Hex: %02x Data: %c\r\n", i, data[i], data[i]);
    }
}

void Server::onConnect(ENetPeer *peer) {
	PacketData *data = p.CreateOnConnectPacket();
	p.Send(peer, data);
}

void Server::onReceive(ENetPeer *peer, ENetPacket *packet) {
	PacketData *data = p.Unpack(packet);
	//this->DumpArray(packet->data, packet->dataLength);
}

void Server::onDisconnect(ENetPeer *peer) {

}

int main ()
{
	setbuf(stdout, NULL);
	enet_address_set_host (&host->address, "0.0.0.0");
	host->address.port = 10003;
	host->server = enet_host_create (&host->address, 3200, 2, 0, 0);
	host->server->checksum = enet_crc32;
	enet_host_compress_with_range_coder(host->server);

	unsigned char *toPeerIDUnpacked = new unsigned char[4];
	while(true) {
		if(enet_host_service(host->server, &host->event, 100) > 0) {
			host->peer = host->event.peer;
 			if(host->event.type == ENET_EVENT_TYPE_CONNECT) {
				ServerEvent.onConnect(host->peer);
			}
			if(host->event.type == ENET_EVENT_TYPE_RECEIVE) {
				ServerEvent.onReceive(host->peer, host->event.packet);
			}
			if(host->event.type == ENET_EVENT_TYPE_DISCONNECT) {
				ServerEvent.onDisconnect(host->peer);
			}

		}
	}

	enet_host_destroy(host->server);
    return 0;
}
