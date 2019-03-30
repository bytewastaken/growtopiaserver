struct ServerData {
	ENetAddress address;
	ENetHost *server;
	ENetEvent event;
	ENetPeer *peer;
};


class Server {
	public:
		void onConnect(ServerData *host);
		void onReceive(ServerData *host, ENetPacket *packet);
		void onDisconnect(ServerData *host);
};