#include <iostream>
#include <enet/enet.h>
#include <cstring>
#include <string>
#include <vector>
#include <sstream>
#include <utility>
#include "utils.h"

using namespace std;

Array Utils::Explode(const string &delimiter, const string &str)
{
	Array arr;

	int strleng = str.length();
	int delleng = delimiter.length();
	if (delleng == 0)
		return arr;//no change

	int i = 0;
	int k = 0;
	while (i<strleng)
	{
		int j = 0;
		while (i + j<strleng && j<delleng && str[i + j] == delimiter[j])
			j++;
		if (j == delleng)//found delimiter
		{
			arr.push_back(str.substr(k, i - k));
			i += delleng;
			k = i;
		}
		else
		{
			i++;
		}
	}
	arr.push_back(str.substr(k, i - k));
	return arr;
}

void Utils::DumpArray(unsigned char* data, int len) {
    for(int i = 0; i < len; i++) {
        printf("Index: %i Hex: %02x Data: %c\r\n", i, data[i], data[i]);
    }
}