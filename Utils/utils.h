#include <cstring>
#include <string>
#include <vector>
#include <sstream>
#include <utility>

#define Array vector<string>

using namespace std;

class Utils {
	public:
		Array Explode(const string &delimiter, const string &str);
		void DumpArray(unsigned char* data, int len);
		int HexDec(char x);
		int FileSize(string fileName);
		void FileWriteToPointer(string fileName, unsigned char *buffer, int size);
		string ToString(unsigned char *data, int length);
		bool PathExists(string s);
		bool FileExists(const char *fileName);
		string AlphaNumeric(string name);
		char *ReadFile(string fileName);
		void WriteFile(string fileName, char *data, int length);
};