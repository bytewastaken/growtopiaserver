#include <iostream>
#include <cstring>
#include "login.h"

using namespace std;

bool Login::Authenticate(string username, string password) {
	if(username == "a" && password == "a") {
		return true;
	} else {
		return false;
	}
}