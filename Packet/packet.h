struct PacketData {
	int type;
	unsigned char *data;

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