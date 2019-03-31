/*

@author ColdChip

*/

#include <enet/enet.h>
#include <iostream>
#include <cstdint>
#include <cstring>
#include <thread>
#include <vector>
#include <stdint.h>
#include <string>
#include <fstream>
#include <utility>
#include "server.h"
#include "Packet/packet.h" // It includes player.h too
#include "Utils/utils.h"
#include "Login/login.h"
#include "World/world.h"
#include "Command/command.h"

using namespace std;

string initString = "\r\nColdChip - Growtopia Server\r\n===========================";

ServerData *host = new ServerData();

Server ServerEvent;

Packet p;

Utils Util;

Login login;

World world;

Player player;

Command command;

int CID = 1;

void Server::onConnect(ServerData *host) {
	PacketData *data = p.CreateOnConnectPacket();
	p.Send(host->peer, data);
	host->event.peer->data = new PlayerInfo();
}

void Server::onReceive(ServerData *host, ENetPacket *packet) {
	ENetPeer *peer = host->event.peer;
	PacketData *DataType = p.ProbeType(packet);
	switch(DataType->type) {
		case TYPE_2: {
			PacketData *data = p.UnpackText(packet);
			string PData = (char*)data->data;
			Array PDataArray = Util.Explode("\n", PData);
			if(PData.find("tankIDName") == 0) {
				string username = Util.Explode("|",PDataArray[0])[1];
				string password = Util.Explode("|",PDataArray[1])[1];
				LoginResults *loginResult = login.Authenticate(username, password);
				if(loginResult->success == true) {
					if(loginResult->isAdmin == true) {
						((PlayerInfo*)(peer->data))->isAdmin = true;
						((PlayerInfo*)(peer->data))->displayName = "`5@" + username + "`w";
					} else if(loginResult->isDev == true) {
						((PlayerInfo*)(peer->data))->isDev = true;
						((PlayerInfo*)(peer->data))->displayName = "`6@" + username + "`w";
					} else {
						((PlayerInfo*)(peer->data))->displayName = username;
					}
					((PlayerInfo*)(peer->data))->username = username;
					p.OnConsoleMessage(peer, "`2Login Successful");
					p.SendOnLoginPacket(peer);
				} else {
					p.OnConsoleMessage(peer, "`4Login Error");
					p.Disconnect(peer);
				}
				delete loginResult;
			} else if(PData.find("action|refresh_item_data") == 0) {
				p.SendItemsDat(peer);
			} else if(PData.find("action|enter_game") == 0){
				p.OnConsoleMessage(peer, "`2Welcome to ColdChip's Private Server!");
				PacketData *WorldOffersData = p.PacketEnd(p.AppendString(p.AppendString(p.CreatePacket(), "OnRequestWorldSelectMenu"), "default|LOL\nadd_button|Showing: `wWorlds``|_catselect_|0.6|3529161471|\nadd_floater|LOL|0|0.55|3529161471\n"));
				p.Send(peer, WorldOffersData);
			} else if(PData.find("action|input") == 0){
				string TextData = Util.Explode("|",PDataArray[1])[2];
				command.SendTextToAllUsersInWorld(peer, host->server, TextData);
			} else {
				p.OnConsoleMessage(peer, "`4Invalid packet. ");
			}
			delete data;
		}
		break;

		case TYPE_3: {
			PacketData *data = p.UnpackText(packet);
			string PData = (char*)data->data;
			if(PData.find("action|join_request") == 0) {
				world.SendWorld(peer, "lol");
				((PlayerInfo*)(peer->data))->currentWorld = "lol";
				((PlayerInfo*)(peer->data))->netID = CID;
				CID++;
				string MyNetID = std::to_string(((PlayerInfo*)(peer->data))->netID);
				player.SpawnPlayer(peer, MyNetID, 100, 200, ((PlayerInfo*)(peer->data))->displayName, TYPE_LOCAL);
				player.BroadcastOnPlayerEnter(peer, host->server);
			} else if(PData.find("action|quit_to_exit") == 0) {
				player.BroadcastOnPlayerLeave(peer, host->server);
				((PlayerInfo*)(peer->data))->currentWorld = "EXIT";
			} else if(PData.find("action|quit") == 0) {
				enet_peer_disconnect_later(peer, 0);
			}
		}
		break;

		case TYPE_4: {
			PacketData *data = p.UnpackBinary(packet);
			PlayerMoving *PMov = p.UnpackPlayerMoving(data);
			if(PMov->packetType == 0) {
				((PlayerInfo*)(peer->data))->x = PMov->x;
				((PlayerInfo*)(peer->data))->y = PMov->y;
				player.BroadcastLocation(peer, host->server, PMov);
			}
			delete PMov;
		} 
		break;
	}
	//Util.DumpArray(data->data, data->length);
}

void Server::onDisconnect(ServerData *host) {
	ENetPeer *peer = host->event.peer;
	player.BroadcastOnPlayerLeave(peer, host->server);
	delete (char*)host->event.peer->data;
}

void Status() {
	while(true) {
		usleep(250 * 1000);
		int i = 0;
		ENetPeer *currentPeer;
		for(currentPeer = host->server->peers; currentPeer < &host->server->peers[host->server->peerCount]; ++currentPeer) {
			if(currentPeer->state == ENET_PEER_STATE_CONNECTED) {
				i++;
			}
		}
		if(i > 1) {
			printf("\r  [ %i Users are online. ]", i);
		} else {
			printf("\r  [ %i User is online.   ]", i);
		}
	}
}

int main ()
{
	setbuf(stdout, NULL);
	std::thread stats(Status);
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
				ServerEvent.onConnect(host);
			}
			if(host->event.type == ENET_EVENT_TYPE_RECEIVE) {
				ServerEvent.onReceive(host, host->event.packet);
			}
			if(host->event.type == ENET_EVENT_TYPE_DISCONNECT) {
				ServerEvent.onDisconnect(host);
			}

		}
	}

	enet_host_destroy(host->server);
    return 0;
}
