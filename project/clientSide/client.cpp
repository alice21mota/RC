#include "globalsClient/globals.h"

using namespace std;

string port = "58000";
string ip = "localhost";
string userID, password;
string fName, fData, tempAID, filePath;

bool logged_in = false;
bool shouldExit = false;
bool signalReceived = false; // Flag to indicate whether a signal was received

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

    // Check if the input line is empty
    if (command.empty()) {
        return command;
    }

    //cout << "Received command: " << command << endl;

    return command;
}

void exit() {

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
            status = closeStatus(result);

            cout << "CLOSE REQUEST: " << status << endl;

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
            status = show_assetStatus(result);

            cout << "AUCTION " << tempAID << "'S ASSET:" << endl << status << endl;

        }
        else cout << "Error: " << request << endl;

    }
    else if (whichCommand == "bid" || whichCommand == "b") {

        request = bid(command);

        if (request.substr(0, 3) == "BID") {

            result = sendTCP(request, "", "");
            status = bidStatus(result);

            cout << "BID REQUEST: " << status << endl;

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
    else if (!signalReceived) {}
    else cout << "Unknown command" << endl;
}

int main(int argc, char *argv[])
{
    // Set up signal handlers
    if (setup_signal_handlers() == EXIT_FAILURE) {
        cerr << "Error setting up signal handlers. Exiting." << endl;
        return EXIT_FAILURE;
    }

    string result, user_command;

    getArgs(argc, argv);

    while (!shouldExit) {
        cout << "Enter a command: " << endl;
        user_command = readCommands();

        if (signalReceived) {

            break;  // Exit the loop
        }

        getCommand(user_command);
        cin.clear();

    }

    return EXIT_SUCCESS;

}