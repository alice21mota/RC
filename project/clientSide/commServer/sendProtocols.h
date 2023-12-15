#ifndef SENDPROTOCOLS_H
#define SENDPROTOCOLS_H

#include "../globalsClient/globals.h"
#include "../../common/validations.h"

using namespace std;

int openTcpSocket();

int openUdpSocket();

string sendUDP(string message);

void sendFileChunks(int fd, string fileName);

string sendTCP(string message, string fileName, string comm);


#endif // SENDPROTOCOLS_H 