#ifndef COMMANDSUDP_H
#define COMMANDSUDP_H


#include "../globalsClient/globals.h"
#include "../../common/validations.h"

using namespace std;

string login(string command);

string logout();

string unregister();

string open(string command);

string myauctions();

string mybids();

string list(string command);

string show_record(string command);


#endif // COMMANDSUDP_H 