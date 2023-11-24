#include <iostream>
#include <sstream>
#include <string>
#include <cctype> 

#define PORT "58001"

using namespace std;

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

int main() {
    string user_command;

    while (1){

        user_command = readCommands();
        getCommand(user_command);
    }

    cout << "done\n";

}