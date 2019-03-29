COMPILER=g++

OUTPUT=server

INCLUDE_MODULES=enet

INCLUDE_FILES=Player/player.cpp Login/login.cpp Packet/packet.cpp

module:
	$(COMPILER) server.cpp $(INCLUDE_FILES) -o $(OUTPUT) -l $(INCLUDE_MODULES)
clean:
