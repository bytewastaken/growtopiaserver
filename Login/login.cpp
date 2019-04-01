#include <iostream>
#include <cstring>
#include "login.h"
#include "../Utils/utils.h"
#include "../Hash/picosha.h"
#include "../Player/player.h"

using namespace std;

string LoginPath = "Database/Users/";

LoginResults *Login::Authenticate(string username, string password) {
	Utils util;
	Player player;
	LoginResults *result = new LoginResults();
	username = util.AlphaNumeric(username);
	string hash = username + "_" + hash_sha256(username + password).substr(0, 16);
	if(util.FileExists((LoginPath + hash).c_str())) {
		if(util.FileSize(LoginPath + hash) < 2) {
			PlayerDB *db = new PlayerDB();
			player.UpdateDB(LoginPath + hash, db);
		}
		char *UserData;
		UserData = util.ReadFile(LoginPath + hash);
		memcpy(&result->isAdmin, UserData, 1);
		memcpy(&result->isDev, UserData + 1, 1);
		result->success = true;
	} else {
		result->success = false;
	}
	return result;
}