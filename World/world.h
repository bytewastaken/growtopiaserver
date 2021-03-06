#include <string>
#include <enet/enet.h>

using namespace std;

struct WorldData {
	unsigned char *data;
	int length;
};

class World {
	public:
		bool SendWorld(ENetPeer *peer, string name);
		void ApplyTileChange(int x, int y, int16_t tile, string world);
};