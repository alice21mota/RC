#ifndef STATUS_H
#define STATUS_H


#include "../globalsClient/globals.h"
#include "../../common/validations.h"
#include "../../common/auxFuncs.h"


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

void show_assetStatus(string response);

void show_recordStatus(string response);

#endif // STATUS_H