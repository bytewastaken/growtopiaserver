#include <iostream>
#include <enet/enet.h>
#include <cstring>
#include <string>
#include <vector>
#include <sstream>
#include <utility>
#include "utils.h"

using namespace std;

std::vector<std::string> Utils::Explode(std::string const &s, const char &delim)
{
    std::vector<std::string> result;
    istringstream iss(s);

    for (std::string token; std::getline(iss, token, delim);)
    {
        result.push_back(move(token));
    }

    return result;
}

void Utils::DumpArray(unsigned char* data, int len) {
    for(int i = 0; i < len; i++) {
        printf("Index: %i Hex: %02x Data: %c\r\n", i, data[i], data[i]);
    }
}