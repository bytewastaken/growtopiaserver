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

int Utils::HexDec(char x) {
	switch (x)
	{
		case '0':
			return 0;
		case '1':
			return 1;
		case '2':
			return 2;
		case '3':
			return 3;
		case '4':
			return 4;
		case '5':
			return 5;
		case '6':
			return 6;
		case '7':
			return 7;
		case '8':
			return 8;
		case '9':
			return 9;
		case 'A':
			return 10;
		case 'B':
			return 11;
		case 'C':
			return 12;
		case 'D':
			return 13;
		case 'E':
			return 14;
		case 'F':
			return 15;
		default:
			break;
	}
}

void Utils::DumpArray(unsigned char* data, int len) {
    for(int i = 0; i < len; i++) {
        printf("Index: %i Hex: %02x Data: %c\r\n", i, data[i], data[i]);
    }
}