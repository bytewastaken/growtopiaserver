#include <iostream>
#include <enet/enet.h>
#include <cstring>
#include <string>
#include "world.h"

using namespace std;

WorldData *World::GetWorld(string name) {
	WorldData *w = new WorldData();
	return w;
}