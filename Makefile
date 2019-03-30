COMPILER=g++

OUTPUT=server

INCLUDE_MODULES=enet

INCLUDE_FILES=Player/player.cpp Login/login.cpp Packet/packet.cpp Utils/utils.cpp

DISPLAY_TEXT=Compiling Files

module:
	$(COMPILER) server.cpp $(INCLUDE_FILES) -o $(OUTPUT) -l $(INCLUDE_MODULES)
all:
	@echo $(DISPLAY_TEXT)
