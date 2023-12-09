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

using namespace std;

string port = "58000";
string ip = "localhost";
string userID, password;
string fName, fData, tempAID, filePath;
//#define PORT "58001"
bool logged_in = false;

string sendUDP(string message) {
    message = message + "\n";

    // maybe global variables
    int fd, errcode;
    ssize_t n, recSize;
    socklen_t addrlen;
    char buffer[128];

    struct addrinfo hints, *res;
    struct sockaddr_in addr;
   

    fd = socket(AF_INET, SOCK_DGRAM, 0);
    if (fd == -1)
    {
        exit(1);
    }

    memset(&hints, 0, sizeof hints);
    hints.ai_family = AF_INET; // IPv4
    hints.ai_socktype = SOCK_DGRAM;

    errcode = getaddrinfo(ip.c_str(), port.c_str(), &hints, &res);
    if (errcode != 0)
    {
        exit(1);
    }

    n = sendto(fd, message.c_str(), message.length(), 0, res->ai_addr, res->ai_addrlen);
    if (n == -1)
    {
        exit(1);
    }

    /*n = recvfrom(fd, buffer, sizeof(buffer), 0, (struct sockaddr *)&addr, &addrlen);
    if (n == -1)
    {
        exit(1);
    }*/

    addrlen = sizeof(addr);
    char headerBuffer[sizeof(int)]; //Storing message size

    ssize_t headerBytes = recvfrom(fd, headerBuffer, sizeof(headerBuffer), MSG_PEEK, (struct sockaddr *)&addr, &addrlen);
    //cout << "headerBytes = " << headerBytes << endl;
    if (headerBytes == -1) {
        cerr << "Error receiving message size." << endl;
        close(fd);
        exit(1);
    }

    int messageSize;
    memcpy(&messageSize, headerBuffer, sizeof(int));

    char* fullMessage = new char[messageSize];
    ssize_t messageBytes = recvfrom(fd, fullMessage, messageSize, 0, (struct sockaddr *)&addr, &addrlen);
    if (messageBytes == -1) {
        cerr << "Error receiving message." << endl;
        //delete[] buffer;
        close(fd);
        exit(1);
    }

    //cout << "Data received from the server: " << fullMessage << endl;

    //cout << "Complete Response: " << fullResponse; // Print the complete response*/

    freeaddrinfo(res);
    close(fd);

    return fullMessage;
}

void sendFileChunks(int fd, string fileName) {
    size_t chunkSize = 1024;
    char buffer[chunkSize];

    // Open the file
    ifstream file(fileName, ios::binary);
    if (!file.is_open()) {
        cerr << "Error opening file." << endl;
        exit(1);
    }

    // Send the file in chunks
    while (!file.eof()) {
        file.read(buffer, chunkSize);

        // Check if anything was read
        if (file.gcount() > 0) {
            ssize_t n = write(fd, buffer, file.gcount());
            if (n == -1) {
                exit(1);
            }
        }
    }

    // Close the file
    file.close();
}

string sendTCP(string message, string fileName){
    cout << "GOT in TCP\n";
    if (fileName == ""){
        message = message + "\n";
    }

    int fd, errcode;
    ssize_t n;
    socklen_t addrlen;
    char buffer[128];

    struct addrinfo hints, *res;
    struct sockaddr_in addr;

    fd = socket(AF_INET, SOCK_STREAM, 0);
    if (fd == -1)
    {
        cout << "GOT in socket\n";
        exit(1);
    }
    

    memset(&hints, 0, sizeof hints);
    hints.ai_family = AF_INET; // IPv4
    hints.ai_socktype = SOCK_STREAM;

    errcode = getaddrinfo(ip.c_str(), port.c_str(), &hints, &res);
    if (errcode != 0)
    {
        cout << "GOT in addrinfo\n";
        exit(1);
    }

    n = connect(fd, res->ai_addr, res->ai_addrlen);
    if (n == -1)
    {
        cout << "GOT in connect\n";
        exit(1);
    }

    n = write(fd, message.c_str(), message.length());
    if (n == -1)
    {
        cout << "GOT in write\n";
        exit(1);
    }

    if (fileName != ""){
        
        sendFileChunks(fd, fileName);
        
        char newline = '\n';
        ssize_t n = write(fd, &newline, 1);
        if (n == -1) {
            cout << "GOT in send chunks write\n";
            exit(1);
        }
    }

    n = read(fd, buffer, 128);
    if (n == -1)
    {
        cout << "GOT in read\n";
        exit(1);
    }
    
    cout << "ALMOST OUT\n";
    freeaddrinfo(res);
    close(fd);

    return buffer;
}

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

string readCommands(){
    string command;
    getline(cin, command);
    cout << "Command is: " << command << "\n";
    return command;
}

void exit(){
    //TODO Check if user is logged in. If it is -> logout
    //If locally we have values for UID and passowrd, then the user is logged in.
    if (logged_in == true)
        cout << "Please logout before exiting the application\n";
    
    else {
        cout << "Exiting the program.\n" << endl;
        exit(0);
    }
}

string show_record(){
    //TODO IS IT SUPPOSE TO LOSE USER INFORMATION ON CLIENT SIDE?
    return "SRC";
}

//TODO Não esquecer de dar check do reply do server para cada command
void getCommand(string command){
    string whichCommand, request, status, result;
    //getline (command, whichCommand, " ");
    whichCommand = command.substr(0, command.find(" "));

    if (whichCommand == "login"){
        request = login(command);

        if ((request.substr(0, 3) == "LIN")){
            cout << request << "\n";
            result = sendUDP(request);
            status = result.substr(0, result.find('\n'));
            //cout << "Status is->" << status <<"\nABC\n";
            cout << "RESPONSE:";
            loginStatus(status);

        } else {
            cout << "Error: " << request << endl;
        }

    } else if (whichCommand == "logout") {
        request = logout();

        if (request.substr(0, 3) == "LOU"){
            cout << request << "\n";
            result = sendUDP(request);
            status = result.substr(0, result.find('\n'));
            cout << "Status is->" << status <<"\n";
            cout << "RESPONSE:";
            logoutStatus(status);
        } else {
            cout << "Error: " << request << endl;
        }

    } else if (whichCommand == "unregister") {
        request = unregister();

        if (request.substr(0, 3) == "UNR"){
            cout << request << "\n";
            result = sendUDP(request);
            status = result.substr(0, result.find('\n'));
            cout << "Status is->" << status <<"\n";
            cout << "RESPONSE:";
            unregisterStatus(status);

        } else {
            cout << "Error: " << request << endl;
        }

    } else if (whichCommand == "exit") {
        exit();
        //cout << "exit\n";

    } else if (whichCommand == "open") {
        request = open(command);

        if (request.substr(0, 3) == "OPA"){
            cout << request << "\n";
            cout << fName << " <- FILE NAME\n";
            cout << filePath << " <- FILE PATH\n";
            result = sendTCP(request, filePath);
            status = result.substr(0, result.find('\n'));
            cout << "Status is->" << status <<"\n";
            cout << "RESPONSE:";
            openStatus(status);

        } else {
            cout << "Error: " << request << endl;
        }

    } else if (whichCommand == "close") {
        request = close(command);

        if (request.substr(0, 3) == "CLS"){
            cout << request << "\n";
            result = sendTCP(request, "");
            status = result.substr(0, result.find('\n'));
            cout << "Status is->" << status <<"\n";
            cout << "RESPONSE:";
            closeStatus(status);

        }else {
            cout << "Error: " << request << endl;
        }

    } else if (whichCommand == "myauctions" || whichCommand == "ma") {
        request = myauctions();

        if (request.substr(0, 3) == "LMA"){
            cout << request << "\n";
            result = sendUDP(request);
            status = result.substr(0, result.find('\n'));
            cout << "Status is->" << status <<"\n";
            cout << "RESPONSE:";
            myauctionsStatus(status);

        } else cout << "Error: " << request << endl;

    } else if (whichCommand == "mybids" || whichCommand == "mb") {
        request = mybids();

        if (request.substr(0, 3) == "LMB"){
            cout << request << "\n";
            result = sendUDP(request);
            status = result.substr(0, result.find('\n'));
            cout << "Status is->" << status <<"\n";
            cout << "RESPONSE:";
            mybidsStatus(status);

        } else cout << "Error: " << request << endl;

    } else if (whichCommand == "list" || whichCommand == "l") {
        request = list(command);

        if (request.substr(0, 3) == "LST"){
            cout << request << "\n";
            result = sendUDP(request);
            status = result.substr(0, result.find('\n'));
            cout << "Status is->" << status <<"\n";
            cout << "RESPONSE: ";
            listStatus(status);

        } else cout << "Error: " << request << endl;

    } else if (whichCommand == "show_asset" || whichCommand == "sa") {
        // show_asset(command);
        cout << "show_asset\n";

    } else if (whichCommand == "bid" || whichCommand == "b") {
        request = bid(command);

        if (request.substr(0, 3) == "BID"){
            cout << request << "\n";
            result = sendTCP(request, "");
            status = result.substr(0, result.find('\n'));
            cout << "Status is->" << status <<"\n";
            cout << "RESPONSE:";
            bidStatus(status);

        }else {
            cout << "Error: " << request << endl;
        }
        
    } else if (whichCommand == "show_record" || whichCommand == "sr") {
        request = show_record();
        cout << request << "\n";
        //sendUDP(request);
        cout << "show_record\n";

    } else {
        cout << "Unknown command\n";
    }
}


int main(int argc, char *argv[])
{
    string result, user_command;

    getArgs(argc, argv);

    while (1){

        user_command = readCommands();
        getCommand(user_command);
    }
    cout << port << "\n" << ip << "\n";
    cout << "done\n";
    cout << "--------" << result << endl; // Debug
}