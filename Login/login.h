#include <string>

using namespace std;

const string LOGIN_ACCEPTED = "poiuytrectg6543343";

const string LOGIN_NOT_ACCEPTED = "DDRDRDRF5R43E3W";

const string LOGIN_DATABASE_NON_EXIST = "R5R43E3EF5F";

struct LoginResults {

	bool success = false;
	bool isAdmin = false;
	bool isDev = false;

};

class Login {
	public:
		LoginResults *Authenticate(string username, string password);
};