#include <iostream>
#include <cstring>
#include <enet/enet.h>

using namespace std;

class Command
{
	public:
		void SendTextToAllUsersInWorld(ENetPeer *sender, ENetHost *users, string TextData);
		void ProccessCommand(ENetPeer *peer, ENetHost *users, string command);
};