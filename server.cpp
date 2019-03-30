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
#include "Utils/utils.h"
#include "Login/login.h"
#include "World/world.h"
#include "Player/player.h"

using namespace std;

string initString = "\r\nColdChip - Growtopia Server\r\n===========================";

ServerData *host = new ServerData();

Server ServerEvent;

Packet p;

Utils Util;

Login login;

World world;

Player player;

void Server::onConnect(ENetPeer *peer) {
	PacketData *data = p.CreateOnConnectPacket();
	p.Send(peer, data);
}

void Server::onReceive(ENetPeer *peer, ENetPacket *packet) {
	PacketData *data = p.Unpack(packet);
	switch(data->type) {
		case TYPE_2: {
			string PData = (char*)data->data;
			Array PDataArray = Util.Explode("\n", PData);
			if(PData.find("tankIDName") == 0) {
				string username = Util.Explode("|",PDataArray[0])[1];
				string password = Util.Explode("|",PDataArray[1])[1];
				bool LoginResults = login.Authenticate(username, password);
				if(LoginResults == true) {
					p.OnConsoleMessage(peer, "`2Login Successful");
					p.SendOnLoginPacket(peer);
				} else {
					p.OnConsoleMessage(peer, "`4Login Error");
					p.Disconnect(peer);
				}
			} else if(PData.find("action|refresh_item_data") == 0) {
				p.SendItemsDat(peer);
			} else if(PData.find("action|enter_game") == 0){
				p.OnConsoleMessage(peer, "`2Welcome to ColdChip's Private Server!");
				PacketData *data = p.PacketEnd(p.AppendString(p.AppendString(p.CreatePacket(), "OnRequestWorldSelectMenu"), "default|LOL\nadd_button|Showing: `wWorlds``|_catselect_|0.6|3529161471|\nadd_floater|LOL|0|0.55|3529161471\n"));
				p.Send(peer, data);
			}
		}
		break;

		case TYPE_3: {
			string PData = (char*)data->data;
			if(PData.find("action|join_request") == 0) {
				world.SendWorld(peer, "lol");
				player.SpawnPlayer(peer, TYPE_LOCAL);
			}
		}
		break;
	}
	//Util.DumpArray(data->data, data->length);
}

void Server::onDisconnect(ENetPeer *peer) {

}

int main ()
{
	setbuf(stdout, NULL);
	cout << (initString) << endl;
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
