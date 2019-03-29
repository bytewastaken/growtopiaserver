struct ServerData {
	ENetAddress address;
	ENetHost *server;
	ENetEvent event;
	ENetPeer *peer;
};


class Server {
	public:
		void onConnect(ENetPeer *peer);
		void onReceive();
		void onDisconnect();
};