struct PacketData {

	unsigned char *data;

};

class Packet {
	public:
		void getType();
		void Send(ENetPeer *peer, PacketData *data);
		PacketData *CreateOnConnectPacket();
	private:
		unsigned char *MakeRawPacket(int num, unsigned char* data, int len);
};