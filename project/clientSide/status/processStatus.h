#ifndef STATUS_H
#define STATUS_H


#include "../globalsClient/globals.h"

using namespace std;

string loginStatus(string response);

string logoutStatus(string response);

string unregisterStatus(string response);

string openStatus(string response);

string closeStatus(string response);

string myauctionsStatus(string response);

string bidStatus(string response);

string mybidsStatus(string response);

string listStatus(string response);

string show_assetStatus(string response);

string show_recordStatus(string response);

#endif // STATUS_H