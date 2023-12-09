#ifndef COMMANDSTCP_H
#define COMMANDSTCP_H

#include <string>
#include <regex>
#include <filesystem>
#include <iostream>
#include <sstream>
#include "../globalsClient/globals.h"
#include "../../common/validations.h"
#include "../../common/auxFuncs.h"

using namespace std;

string open(string command);
string close(string command);
string bid(string command);



#endif // COMMANDSTCP_H 