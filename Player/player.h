#include <string>
#include <enet/enet.h>

using namespace std;

const string TYPE_LOCAL = "type|local\n";

const string TYPE_NON_LOCAL = "";

class Player {
	public:
		void SpawnPlayer(ENetPeer *peer, string type);
};