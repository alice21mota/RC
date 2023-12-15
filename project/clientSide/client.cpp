#include <sys/types.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <time.h>
#include <fcntl.h>
#include <sys/socket.h>
#include <sys/ioctl.h>
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

#include "../common/validations.h"
#include "../common/auxFuncs.h"
#include "status/processStatus.h"
#include "globalsClient/globals.h"
#include "commands/commandsUDP.h"
#include "commands/commandsTCP.h"
#include "commServer/sendProtocols.h"

using namespace std;

string port = "58000";
string ip = "localhost";
string userID, password;
string fName, fData, tempAID, filePath;

bool logged_in = false;

void getArgs(int argc, char *argv[])
{
    for (int i = 1; i < argc; i += 2)
    {
        if (!strcmp(argv[i], "-n"))
            ip = argv[i + 1];
        if (!strcmp(argv[i], "-p"))
            port = argv[i + 1];
    }
}

string readCommands() {
    string command;
    getline(cin, command);
    //cout << "Command is: " << command << endl;
    return command;
}

void exit() {
    //TODO Check if user is logged in. If it is -> logout
    //If locally we have values for UID and passowrd, then the user is logged in.
    if (logged_in == true)
        cout << "Please logout before exiting the application" << endl;

    else {
        cout << "Exiting the program." << endl;
        exit(0);
    }
}

void getCommand(string command) {

    string whichCommand, request, status, result;
    whichCommand = command.substr(0, command.find(" "));

    if (whichCommand == "login") {

        request = login(command);

        if ((request.substr(0, 3) == "LIN")) {

            result = sendUDP(request);
            status = loginStatus(result);

            cout << "LOGIN: " << status << endl;

        }
        else cout << "Error: " << request << endl;

    }
    else if (whichCommand == "logout") {

        request = logout();

        if (request.substr(0, 3) == "LOU") {

            result = sendUDP(request);
            status = logoutStatus(result);

            cout << "LOGOUT: " << status << endl;

        }
        else cout << "Error: " << request << endl;

    }
    else if (whichCommand == "unregister") {

        request = unregister();

        if (request.substr(0, 3) == "UNR") {

            result = sendUDP(request);
            status = unregisterStatus(result);

            cout << "UNREGISTER: " << status << endl;

        }
        else cout << "Error: " << request << endl;

    }
    else if (whichCommand == "exit") {

        exit();

    }
    else if (whichCommand == "open") {

        request = open(command);

        if (request.substr(0, 3) == "OPA") {

            result = sendTCP(request, fName, "");
            status = openStatus(result);

            cout << "OPEN REQUEST: " << status << endl;

        }
        else cout << "Error: " << request << endl;

    }
    else if (whichCommand == "close") {

        request = close(command);

        if (request.substr(0, 3) == "CLS") {

            result = sendTCP(request, "", "");
            status = result.substr(0, result.find('\n'));

            cout << "RESPONSE: ";
            closeStatus(status);

        }
        else cout << "Error: " << request << endl;

    }
    else if (whichCommand == "myauctions" || whichCommand == "ma") {

        request = myauctions();

        if (request.substr(0, 3) == "LMA") {

            result = sendUDP(request);
            status = myauctionsStatus(result);

            cout << "MY AUCTIONS:" << endl << status;

        }
        else cout << "Error: " << request << endl;

    }
    else if (whichCommand == "mybids" || whichCommand == "mb") {

        request = mybids();

        if (request.substr(0, 3) == "LMB") {

            result = sendUDP(request);
            status = mybidsStatus(result);

            cout << "MY BIDS: " << endl << status;

        }
        else cout << "Error: " << request << endl;

    }
    else if (whichCommand == "list" || whichCommand == "l") {

        request = list(command);

        if (request.substr(0, 3) == "LST") {

            result = sendUDP(request);
            status = listStatus(result);

            cout << "LIST OF AUCTIONS: " << endl << status;

        }
        else cout << "Error: " << request << endl;

    }
    else if (whichCommand == "show_asset" || whichCommand == "sa") {

        request = show_asset(command);

        if (request.substr(0, 3) == "SAS") {

            // rec indicates that this command may be receiving a file
            result = sendTCP(request, "", "rec");

            cout << "RESPONSE: ";
            show_assetStatus(result);

        }
        else cout << "Error: " << request << endl;

    }
    else if (whichCommand == "bid" || whichCommand == "b") {

        request = bid(command);

        if (request.substr(0, 3) == "BID") {

            result = sendTCP(request, "", "");
            status = result.substr(0, result.find('\n'));

            cout << "RESPONSE: ";
            bidStatus(status);

        }
        else cout << "Error: " << request << endl;

    }
    else if (whichCommand == "show_record" || whichCommand == "sr") {

        request = show_record(command);

        if (request.substr(0, 3) == "SRC") {

            result = sendUDP(request);
            status = show_recordStatus(result);

            cout << "AUCTION " << tempAID << " - RECORD: " << endl << status;
            
        }
        else cout << "Error: " << request << endl;

    }
    else cout << "Unknown command\n";
}

int main(int argc, char *argv[])
{
    string result, user_command;

    getArgs(argc, argv);

    while (1) {

        user_command = readCommands();
        getCommand(user_command);
    }

}