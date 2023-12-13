#ifndef SENDPROTOCOLS_H
#define SENDPROTOCOLS_H

#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <unistd.h>
#include <sstream>
#include <iostream>
#include <fstream>
#include <cstring>

#include "../globalsClient/globals.h"
#include "../../common/validations.h"

using namespace std;


string sendUDP(string message);

void sendFileChunks(int fd, string fileName);

//string receiveResponse(int fd);

string sendTCP(string message, string fileName, string comm);


#endif // SENDPROTOCOLS_H 