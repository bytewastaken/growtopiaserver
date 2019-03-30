struct ServerData {
	ENetAddress address;
	ENetHost *server;
	ENetEvent event;
	ENetPeer *peer;
};


class Server {
	public:
		void onConnect(ENetPeer *peer);
		void onReceive(ENetPeer *peer, ENetPacket *packet);
		void onDisconnect(ENetPeer *peer);
		void DumpArray(unsigned char* data, int len);
};