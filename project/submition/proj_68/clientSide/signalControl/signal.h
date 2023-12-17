#ifndef SIGNAL_H
#define SIGNAL_H


#include "../globalsClient/globals.h"
#include "../../common/validations.h"

using namespace std;

int setup_signal_handlers();

void exitUserApp(int sig);


#endif // SIGNAL_H 