#include <string>
#include "../Player/player.h"

using namespace std;

const int TYPE_1 = 1;

const int TYPE_2 = 2;

const int TYPE_3 = 3;

const int TYPE_4 = 4;

const string PacketHeader = "0400000001000000FFFFFFFF00000000080000000000000000000000000000000000000000000000000000000000000000000000000000000000000000";

const string ItemsDatPacketHeader = "0400000010000000FFFFFFFF000000000800000000000000000000000000000000000000000000000000000000000000000000000000000000000000";

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
		PacketData *UnpackText(ENetPacket *packet);
		PacketData *UnpackBinary(ENetPacket *packet);
		PacketData *ProbeType(ENetPacket *packet);
		PacketData *CreatePacket();
		PacketData *AppendString(PacketData *p, string str);
		PacketData *PacketEnd(PacketData *p);
		void OnConsoleMessage(ENetPeer *peer, string data);
		void Disconnect(ENetPeer *peer);
		void SendOnLoginPacket(ENetPeer *peer);
		void SendItemsDat(ENetPeer *peer);
		PacketData *AppendInt(PacketData *p, int val);
		PlayerMoving *UnpackPlayerMoving(PacketData *data);
		PacketData *PackPlayerMoving(PlayerMoving *data);
		PacketData *AppendIntx(PacketData *p, int val);
		void OnTalkBubble(ENetPeer *peer, int netID, string data);
	private:
		PacketData *MakeRawPacket(int num, unsigned char* data, int len);
};