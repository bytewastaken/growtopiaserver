#include <string>

using namespace std;

const int TYPE_1 = 1;

const int TYPE_2 = 2;

const int TYPE_3 = 3;

const int TYPE_4 = 4;

struct PacketData {
	int type;
	unsigned char *data;
	int length;
	int indexes = 0;
};

class Packet {
	public:
		void getType();
		void Send(ENetPeer *peer, PacketData *data);
		PacketData *CreateOnConnectPacket();
		PacketData *Unpack(ENetPacket *packet);
		PacketData *CreatePacket();
		PacketData *AppendString(PacketData *p, string str);
		PacketData *PacketEnd(PacketData *p);
		void OnConsoleMessage(ENetPeer *peer, string data);
		void Disconnect(ENetPeer *peer);
	private:
		PacketData *MakeRawPacket(int num, unsigned char* data, int len);
};