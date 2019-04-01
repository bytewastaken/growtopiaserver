#include <iostream>
#include <enet/enet.h>
#include <cstring>
#include <string>
#include "world.h"
#include "../Packet/packet.h"
#include <enet/enet.h>
#include "../Utils/utils.h"

using namespace std;

void World::SendWorld(ENetPeer *peer, string name) {
	Packet packet;
	Utils util;
	string asdf = "0400000004A7379237BB2509E8E0EC04F8720B050000000000000000FBBB0000010000007D920100FDFDFDFD04000000040000000000000000000000070000000000"; // 0400000004A7379237BB2509E8E0EC04F8720B050000000000000000FBBB0000010000007D920100FDFDFDFD04000000040000000000000000000000080000000000000000000000000000000000000000000000000000000000000048133A0500000000BEBB0000070000000000
	string worldName = "LOL";
	int xSize = 100;
	int ySize = 50;
	int square = xSize*ySize;
	int16_t nameLen = worldName.length();
	int payloadLen = asdf.length() / 2;
	int dataLen = payloadLen + 2 + nameLen + 12 + (square * 8) + 4;
	int allocMem = payloadLen + 2 + nameLen + 12 + (square * 8) + 4 + 16000;
	unsigned char* data = new unsigned char[allocMem];
	for (int i = 0; i < asdf.length(); i += 2)
	{
		char x = util.HexDec(asdf[i]);
		x = x << 4;
		x += util.HexDec(asdf[i + 1]);
		memcpy(data + (i / 2), &x, 1);
	}
	int zero = 0;
	int16_t item = 0;
	int smth = 0;
	for (int i = 0; i < square * 8; i += 4) memcpy(data + payloadLen + i + 14 + nameLen, &zero, 4);
	for (int i = 0; i < square * 8; i += 8) memcpy(data + payloadLen + i + 14 + nameLen, &item, 2);
	memcpy(data + payloadLen, &nameLen, 2);
	memcpy(data + payloadLen + 2, worldName.c_str(), nameLen);
	memcpy(data + payloadLen + 2 + nameLen, &xSize, 4);
	memcpy(data + payloadLen + 6 + nameLen, &ySize, 4);
	memcpy(data + payloadLen + 10 + nameLen, &square, 4);
	unsigned char* blockPtr = data + payloadLen + 14 + nameLen;
	string worldFile = "Database/Worlds/lol.bin";
	if(util.FileExists(worldFile.c_str())) {
		char *ReadWorldData = new char[square * 8];
		ReadWorldData = util.ReadFile(worldFile);
		for (int i = 0; i < square; i++) {
			memcpy(blockPtr, ReadWorldData, (i * 8));
		}
		delete ReadWorldData;
	} else {
		for (int i = 0; i < square; i++) {
			if ((square - i) <= 400) {
				int16_t block = 8;
				memcpy(blockPtr, &block, 2);
				int type = 0x00000000;
				type |= 0;
				memcpy(blockPtr + 4, &type, 4);
				/*if (worldInfo->items[i].foreground % 2)
				{
					blockPtr += 6;
				}*/
			} else {
				memcpy(blockPtr, &zero, 2);
			}
			memcpy(blockPtr + 2, &zero, 2);
			blockPtr += 8;
		}

		char *WDataBinary = new char[square * 8];

		memcpy(WDataBinary, data + payloadLen + 14 + nameLen, square * 8);

		util.WriteFile(worldFile, WDataBinary, square * 8);

		delete WDataBinary;
	}

	memcpy(data + dataLen - 4, &smth, 4);
	WorldData *wData = new WorldData();
	wData->data = data;
	wData->length = dataLen;

	PacketData *PData = new PacketData();
	PData->data = data;
	PData->length = dataLen;
	packet.Send(peer, PData);

}

void World::ApplyTileChange(int x, int y, int16_t tile) {
	Utils util;
	string worldFile = "Database/Worlds/lol.bin";
	int FileSize = util.FileSize(worldFile.c_str());
	char *ReadWorldData = new char[FileSize];
	ReadWorldData = util.ReadFile(worldFile);
	int index = ((100 * y) + x) * 8;
	int zero = 0;
	int16_t tileSet = 0;
	memcpy(&tileSet, ReadWorldData + index, 2);
	if(tileSet > 0) {
		if(tileSet == 8) {

		} else {
			memcpy(ReadWorldData + index, &zero, 4);
			memcpy(ReadWorldData + index + 4, &zero, 4);
		}
	} else {
		if(tile == 8) {

		} else {
			memcpy(ReadWorldData + index, &tile, 2);
			memcpy(ReadWorldData + index + 2, &zero, 2);
			memcpy(ReadWorldData + index + 4, &zero, 4);
		}
	}
	util.WriteFile(worldFile, ReadWorldData, FileSize);
}