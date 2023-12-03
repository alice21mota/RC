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
string userID, password;
//#define PORT "58001"


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

string login(string command){

    // login UID password
    // UID = 6 numeros
    // password = 8 alphabumeric numbers

    string whichCommand;    //, userID, password;
    istringstream iss(command);

    if (iss >> whichCommand >> userID >> password && iss.eof()) {
        // Use userID and password as needed
        if (isUID(userID) && isPassword(password)){
            cout << "User_ID: " << userID << endl;
            cout << "User_password: " << password << endl;
            
        }
        else {
            return "incorrect login";               // TODO DAR CHECK NESTES RETURNS. PASSAR SEND_UDP PARA AQUI??
        }

    } else {
        return "incorrect login";
    }

    return "LIN " + userID + " " + password;
}

string logout(){
    //TODO CHECK IF LOGOUT IS CORRECT
    //TODO IS IT SUPPOSE TO LOSE USER INFORMATION ON CLIENT SIDE?
    userID = "";
    password = "";
    return "LOU " + userID + " " + password + "\n";
}

string unregister(){
    //TODO IS IT SUPPOSE TO LOSE USER INFORMATION ON CLIENT SIDE?
    return "UNR " + userID + " " + password + "\n";
}

void exit(){
    //TODO Check if user is logged in. If it is -> logout
    //If locally we have values for UID and passowrd, then the user is logged in.
    if (userID != "" && password != "")
        cout << "Please logout before exiting the application\n";
    
    else {
        cout << "Exiting the program.\n" << endl;
        exit(0);
    }
}

string myauctions(){
    //TODO IS IT SUPPOSE TO LOSE USER INFORMATION ON CLIENT SIDE?
    return "LMA " + userID;
}

string mybids(){
    //TODO IS IT SUPPOSE TO LOSE USER INFORMATION ON CLIENT SIDE?
    return "LMB " + userID;
}

string list(){
    //TODO IS IT SUPPOSE TO LOSE USER INFORMATION ON CLIENT SIDE?
    return "LST";
}

string show_record(){
    //TODO IS IT SUPPOSE TO LOSE USER INFORMATION ON CLIENT SIDE?
    return "SRC";
}


void getCommand(string command){
    string whichCommand, request;
    //getline (command, whichCommand, " ");
    whichCommand = command.substr(0, command.find(" "));
    cout << "command is: " << whichCommand << "\n";

    if (whichCommand == "login"){
        request = login(command);

        if (request != "Incorrect Login Attempt"){
            cout << request << "\n";
            //sendUDP(request);
            cout << "Sucess login\n";

        } else {
            cout << "INCORRECT LOGIN\n";
        }


    } else if (whichCommand == "logout") {
        request = logout();
        cout << request << "\n";
        //sendUDP(request);
        cout << "logout\n";

    } else if (whichCommand == "unregister") {
        request = unregister();
        cout << request << "\n";
        //sendUDP(request);
        //logout();
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
        request = myauctions();
        cout << request << "\n";
        //sendUDP(request);
        cout << "myauctions\n";

    } else if (whichCommand == "mybids" || whichCommand == "mb") {
        request = mybids();
        cout << request << "\n";
        //sendUDP(request);
        cout << "mybids\n";

    } else if (whichCommand == "list" || whichCommand == "l") {
        request = list();
        cout << request << "\n";
        //sendUDP(request);
        cout << "list\n";

    } else if (whichCommand == "show_asset" || whichCommand == "sa") {
        // show_asset(command);
        cout << "show_asset\n";

    } else if (whichCommand == "bid" || whichCommand == "b") {
        // bid(command);
        cout << "bid\n";
        
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

    cout << port << ip << endl;

    while (1){

        user_command = readCommands();
        getCommand(user_command);
    }

    cout << "done\n";

    result = sendUDP("hello");
    cout << "--------" << result << endl; // Debug
}