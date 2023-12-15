#ifndef STATUS_H
#define STATUS_H


#include "../globalsClient/globals.h"
#include "../../common/validations.h"
#include "../../common/auxFuncs.h"


using namespace std;

string loginStatus(string response);

string logoutStatus(string response);

string unregisterStatus(string response);

void openStatus(string response);

void closeStatus(string response);

string myauctionsStatus(string response);

void bidStatus(string response);

string mybidsStatus(string response);

string listStatus(string response);

void show_assetStatus(string response);

string show_recordStatus(string response);

#endif // STATUS_H