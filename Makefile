COMPILER=g++

OUTPUT=server

INCLUDE_MODULES=-lenet -lpthread

INCLUDE_FILES=Player/player.cpp Login/login.cpp Packet/packet.cpp Utils/utils.cpp World/world.cpp

DISPLAY_TEXT=Compiling Files

module:
	$(COMPILER) server.cpp $(INCLUDE_FILES) -o $(OUTPUT) $(INCLUDE_MODULES)
all:
	@echo $(DISPLAY_TEXT)
