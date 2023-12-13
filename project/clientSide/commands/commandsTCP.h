#ifndef COMMANDSTCP_H
#define COMMANDSTCP_H

#include "../globalsClient/globals.h"
#include "../../common/validations.h"
#include "../../common/auxFuncs.h"

using namespace std;

string open(string command);

string close(string command);

string bid(string command);

string show_asset(string command);


#endif // COMMANDSTCP_H 