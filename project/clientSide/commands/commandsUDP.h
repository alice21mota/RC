#ifndef COMMANDSUDP_H
#define COMMANDSUDP_H

#include <string>
#include <regex>
#include <filesystem>
#include <iostream>
#include <sstream>
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



#endif // COMMANDSUDP_H 