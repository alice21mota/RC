#ifndef GLOBALS_H
#define GLOBALS_H


#include <sys/types.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <sys/ioctl.h>
#include <netdb.h>
#include <time.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <signal.h>
#include <iostream>
#include <sstream>
#include <fstream>
#include <vector>
#include <string>
#include <cctype>
#include <algorithm>
#include <csignal>
#include <cstdlib>
#include <cerrno>

#include "../../common/validations.h"
#include "../../common/auxFuncs.h"

#define TCP_READ_TIMEOUT_SECONDS (1) // 1 minute
#define TCP_WRITE_TIMEOUT_SECONDS (1)  // 20 minutes
#define UDP_READ_TIMEOUT_SECONDS (1) // 1 minute
#define UDP_WRITE_TIMEOUT_SECONDS (1)  // 20 minutes

using namespace std;

extern string userID;
extern string password;
extern string fName;
extern string fData;
extern string tempAID;
extern string filePath;
extern string port;
extern string ip;
extern bool logged_in;
extern bool shouldExit;
extern bool signalReceived; // Flag to indicate whether a signal was received



#endif // GLOBALS_H