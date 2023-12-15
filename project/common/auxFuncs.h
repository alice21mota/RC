#ifndef AUXFUNCS_H
#define AUXFUNCS_H


#include "../clientSide/globalsClient/globals.h"

using namespace std;

size_t getFileSize(string asset_fname);

string fileSizeString(string asset_fname);

int setup_signal_handlers();

void exitUserApp(int sig);


#endif // AUXFUNCS_H 