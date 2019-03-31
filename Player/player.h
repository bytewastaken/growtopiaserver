#include <string>
#include <enet/enet.h>

using namespace std;

const string TYPE_LOCAL = "type|local\n";

const string TYPE_NON_LOCAL = "";

struct PlayerDB {
	bool isAdmin = 0;
	bool isDev = 0;
};

struct PlayerMoving {
	int packetType;
	int netID;
	float x;
	float y;
	int characterState;
	int plantingTree;
	float XSpeed;
	float YSpeed;
	int punchX;
	int punchY;

};

struct PlayerInfo {
	string username;
	string displayName;
	string userID;
	string currentWorld = "EXIT";
	bool isDev = false;
	bool isAdmin = false;
	int netID = 0;
	int x;
	int y;
};

class Player {
	public:
		void SpawnPlayer(ENetPeer *peer, string netID, int x, int y, string username, string type);
		bool isHere(ENetPeer* peer, ENetPeer* peer2);
		void RemovePlayer(ENetPeer *peer, string netID);
		void BroadcastOnPlayerEnter(ENetPeer *peer, ENetHost *users);
		void BroadcastOnPlayerLeave(ENetPeer *peer, ENetHost *users);
		void BroadcastLocation(ENetPeer *peer, ENetHost *users, PlayerMoving *PMov);
		void UpdateDB(string fileName, PlayerDB *db);
};
