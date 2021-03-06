#include <iostream>
#include <enet/enet.h>
#include <cstring>
#include <string>
#include <vector>
#include <sstream>
#include <utility>
#include "utils.h"
#include <fstream>
#include <dirent.h>
#include <fstream>

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

int Utils::FileSize(string fileName) {
	std::ifstream file(fileName.c_str(), std::ios::binary | std::ios::ate);
	return file.tellg();
}

void Utils::FileWriteToPointer(string fileName, unsigned char *buffer, int size) {
	std::ifstream file(fileName, std::ios::binary | std::ios::ate);
	file.seekg(0, std::ios::beg);
	file.read((char*)(buffer), size);
}

void Utils::DumpArray(unsigned char* data, int len) {
    for(int i = 0; i < len; i++) {
        printf("Index: %i Hex: %02x Data: %c\r\n", i, data[i], data[i]);
    }
}

string Utils::ToString(unsigned char *data, int length) {
	string result = "";
	for(int i = 0; i < length; i++) {
		result += data[i];
	}
	return result;
}

bool Utils::PathExists(string s) {
	DIR* dir = opendir(s.c_str());
	if(dir) {
		closedir(dir);
		return true;
	}
	return false;
}

bool Utils::FileExists(const char *fileName) {
	std::ifstream ifile(fileName);
	return (bool)ifile;
}

string Utils::AlphaNumeric(string name) {
	string newS;
	for (char c : name) newS+=(c >= 'A' && c <= 'Z') ? c-('A'-'a') : c;
	string ret;
	for (int i = 0; i < newS.length(); i++)
	{
		if (newS[i] == '`') i++; else ret += newS[i];
	}
	string ret2;
	for (char c : ret) if ((c >= 'a' && c <= 'z') || (c >= '0' && c <= '9')) ret2 += c;
	return ret2;
}

char *Utils::ReadFile(string fileName) {
	int pSize = this->FileSize(fileName);
	char *buffer = new char[pSize + 1];
	std::ifstream infile(fileName.c_str(), std::ifstream::binary);
	infile.read(buffer,pSize);
	infile.close();
	return buffer;
}

void Utils::WriteFile(string fileName, char *data, int length) {
	std::ofstream outfile(fileName.c_str(), std::ofstream::binary);
	outfile.write(data, length);
	delete[] data;
	outfile.close();
}