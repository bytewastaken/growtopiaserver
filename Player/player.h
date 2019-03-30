#include <string>
#include <enet/enet.h>

using namespace std;

const string TYPE_LOCAL = "type|local\n";

const string TYPE_NON_LOCAL = "";

class Player {
	public:
		void SpawnPlayer(ENetPeer *peer, string netID, string username, string type);
		bool isHere(ENetPeer* peer, ENetPeer* peer2);
};

struct PlayerInfo {
	string username;
	string userID;
	string currentWorld;
	int netID = 0;
	int x;
	int y;
};