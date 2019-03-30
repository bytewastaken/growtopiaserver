const int TYPE_1 = 1;

const int TYPE_2 = 2;

const int TYPE_3 = 3;

const int TYPE_4 = 4;

struct PacketData {
	int type;
	unsigned char *data;
	int length;
};

class Packet {
	public:
		void getType();
		void Send(ENetPeer *peer, PacketData *data);
		PacketData *CreateOnConnectPacket();
		PacketData *Unpack(ENetPacket *packet);
	private:
		PacketData *MakeRawPacket(int num, unsigned char* data, int len);
};