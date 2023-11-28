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
#include <string>
#include <cctype> 

using namespace std;

string port = "58000";
string ip = "localhost";
//#define PORT "58001"

// Checks if the string is composed only of numbers
bool isNumeric(string str) {
    return (all_of(str.begin(), str.end(), ::isdigit));
}

bool isAlphanumeric(string str) {
    return (all_of(str.begin(), str.end(), ::isalnum));
}

// Check if UID is in correct format
bool isUID(string uid){
    return (uid.size()==6 && isNumeric(uid));
}

bool isPassword(string password){
    return (password.size()==8 && isAlphanumeric(password));
}

string readCommands(){
    string command;
    getline(cin, command);
    cout << "Command is: " << command << "\n";
    return command;
}

void login(string command){

    // login UID password
    // UID = 6 numeros
    // password = 8 alphabumeric numbers

    string whichCommand, userID, password;
    istringstream iss(command);

    if (iss >> whichCommand >> userID >> password && iss.eof()) {
        // Use userID and password as needed
        if (isUID(userID) && isPassword(password)){
            cout << "User_ID: " << userID << endl;
            cout << "User_password: " << password << endl;
        }
        else {
            cout << "incorrect login attempt" << std::endl;
        }

    } else {
        cout << "incorrect login attempt" << std::endl;
    }

}

void exit(){
    //TODO Check if user is logged in. If it is -> logout
    cout << "Exiting the program." << endl;
    exit(0);
}

void getCommand(string command){
    string whichCommand;
    //getline (command, whichCommand, " ");
    whichCommand = command.substr(0, command.find(" "));
    cout << "command is: " << whichCommand << "\n";

    if (whichCommand == "login"){
        cout << "Going to login\n";
        login(command);
    } else if (whichCommand == "logout") {
        // logout();
        cout << "logout\n";
    } else if (whichCommand == "unregister") {
        // unregister(command);
        cout << "unregister\n";
    } else if (whichCommand == "exit") {
        exit();
        cout << "exit\n";
    } else if (whichCommand == "open") {
        // open(command);
        cout << "open\n";
    } else if (whichCommand == "close") {
        // close(command);
        cout << "close\n";
    } else if (whichCommand == "myauctions" || whichCommand == "ma") {
        // myauctions();
        cout << "myauctions\n";
    } else if (whichCommand == "mybids" || whichCommand == "mb") {
        // mybids();
        cout << "mybids\n";
    } else if (whichCommand == "list" || whichCommand == "l") {
        // list();
        cout << "list\n";
    } else if (whichCommand == "show_asset" || whichCommand == "sa") {
        // show_asset(command);
        cout << "show_asset\n";
    } else if (whichCommand == "bid" || whichCommand == "b") {
        // bid(command);
        cout << "bid\n";
    } else if (whichCommand == "show_record" || whichCommand == "sr") {
        // show_record();
        cout << "show_record\n";
    } else {
        cout << "Unknown command\n";
    }
}

string sendUDP(string message)
{
    message = message + "\n";

    // maybe global variables
    int fd, errcode;
    ssize_t n;
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

    addrlen = sizeof(addr);
    n = recvfrom(fd, buffer, sizeof(buffer), 0, (struct sockaddr *)&addr, &addrlen);
    if (n == -1)
    {
        exit(1);
    }

    cout << "UDP returned: " << buffer << endl; // Debug

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

int main(int argc, char *argv[])
{
    string result, user_command;

    getArgs(argc, argv);

    cout << port << ip << endl;

    while (1){

        user_command = readCommands();
        getCommand(user_command);
    }

    cout << "done\n";

    result = sendUDP("hello");
    cout << "--------" << result << endl; // Debug
}