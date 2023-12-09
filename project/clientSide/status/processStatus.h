#ifndef STATUS_H
#define STATUS_H

#include <string>
#include <iostream>
#include <sstream>
#include <regex>
#include <filesystem> 
#include "../globalsClient/globals.h"
#include "../../common/validations.h"


using namespace std;

void loginStatus(string status);

void logoutStatus(string status);

void unregisterStatus(string status);

void openStatus(string response);

void closeStatus(string response);

void myauctionsStatus(string response);

void bidStatus(string response);

void mybidsStatus(string response);

void listStatus(string response);

#endif // STATUS_H