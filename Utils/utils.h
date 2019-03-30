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
};