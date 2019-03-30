#include <cstring>
#include <string>
#include <vector>
#include <sstream>
#include <utility>

class Utils {
	public:
		std::vector<std::string> Explode(std::string const &s, const char &delim);
		void DumpArray(unsigned char* data, int len);
};