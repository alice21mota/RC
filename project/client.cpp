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
#include <regex>
#include "validation_functions/validations.h"

using namespace std;

string port = "58000";
string ip = "localhost";
string userID, password;
string fName, fData, tempAID;
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

//Gets file size in bytes
size_t getFileSize(string asset_fname) {
    ifstream file(asset_fname, ios::binary | ios::ate);
    size_t size = file.tellg();
    file.close();
    return size;
}

string fileSizeString(string asset_fname) {
    size_t size = getFileSize(asset_fname);
    ostringstream sizeStr;
    sizeStr << size;

    if (sizeStr.str().size() > 8){
        return "TOO MANY DIGITS";
    }

    return sizeStr.str();
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

    if (logged_in == false){

        if (iss >> whichCommand >> userID >> password && iss.eof()) {
            // Use userID and password as needed
            if (isUID(userID) && isPassword(password)){
                return "LIN " + userID + " " + password;
            }

            else {
                //TODO -> Passar o userID e password a ""?
                return "Incorrect login attempt";               // TODO DAR CHECK NESTES RETURNS. PASSAR SEND_UDP PARA AQUI??
            }

        } else {
            return "Incorrect login attempt";
        }

    } else {
        //cout << userID <<" "<< password << endl;
        return "A user is already logged in";
    }

}

void loginStatus(string status){
    
    //cout << "IN LOGIN \n";

    if (status == "RLI OK"){
        logged_in = true;
        cout << "successful login\n";
    }

    //TODO check if userId and password can be saved like this, when login is not performed
    else if (status == "RLI NOK"){
        cout << "incorrect login attempt\n";
    }
    
    //TODO check passing userID to ""
    else if (status == "RLI REG"){
        //userID = "";
        //password = "";
        logged_in = true;
        cout << "new user registered\n";
    }

    else cout << "INCORRECT RESPONSE\n";
}

string logout(){
    if (userID != "" && password != ""){
        return "LOU " + userID + " " + password;
    }

    else return "User not logged in";
    //TODO CHECK IF LOGOUT IS CORRECT
    //TODO IS IT SUPPOSE TO LOSE USER INFORMATION ON CLIENT SIDE?
}

void logoutStatus(string status){
    
    if (status == "RLO OK"){
        //userID = "";
        //password = "";
        logged_in = false;
        cout << "successful logout\n";
    }

    else if (status == "RLO NOK"){
        cout << "user not logged in\n";
    }
    
    else if (status == "RLO UNR"){
        cout << "unknown user\n";
    }

    else cout << "INCORRECT RESPONSE\n";
}

string unregister(){

    if (userID != "" && password != ""){

        if (isUID(userID) && isPassword(password)){
            return "UNR " + userID + " " + password;
        } else return "user not known";    

    } else return "user not known";

    /*if (logged_in = true)
        return "UNR " + userID + " " + password;*/

    //TODO IS IT SUPPOSE TO LOSE USER INFORMATION ON CLIENT SIDE?
}

void unregisterStatus(string status){
    //TODO ALTER THIS TO BE THE SAMEAS THE OTHERS
    if (status == "RUR OK"){
        //userID = "";
        //password = "";
        logged_in = false;
        cout << "successful unregister\n";
    }

    else if (status == "RUR NOK"){
        cout << "incorrect unregister attempt\n";
    }
    
    else if (status == "RUR UNR"){
        cout << "unknown user\n";
    }

    else cout << "INCORRECT RESPONSE\n";
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

string open(string command){
    // name asset_fname start_value timeactive
    // name = max 10 alphanumeric
    // asset_fname = name of image or document file
    // start_value = max 6 digits
    // timeactive = max 5 digits (seconds)

    string whichCommand, name, asset_fname, start_value, timeactive;
    string toReturn, fSize;// fName, fSize, fData;
    istringstream iss(command);

    if (userID != "" && password != ""){
        if (iss >> whichCommand >> name >> asset_fname >> start_value >> timeactive && iss.eof()) {
            
            if (isDescription(name) && isFileName(asset_fname) && isStartValue(start_value) && isDuration(timeactive)){
                
                //Check if the file exists
                if (fileExists(asset_fname)) {
                    
                    fName = asset_fname;
                    size_t fileSize = getFileSize(fName);
                    if (isFileSizeValid(fileSize)){
                        
                        //TODO MAYBE CHANGE THIS FUNC LATER
                        fSize = fileSizeString(fName);

                        if (fSize != "TOO MANY DIGITS"){

                            toReturn = "OPA " + userID + " " + password + " " + name + " " + start_value + " " +
                                    timeactive + " " + fName + " " + fSize + " ";

                            return toReturn;
                        }
                        return "File Size is too big"; //TODO CHANGE THIS LATER
                    }
                    return "Invalid File Size";
                } else
                    return "File not found";
            } else 
                return "Invalid fields";             
        } else 
            return "Command not as expected";
    }
    return "Not logged in User Application";
}

void openStatus(string response){
    istringstream iss(response);
    string command, status, aid;

    if (iss >> command >> status){// && iss.eof()){
        
        if (command == "ROA"){
            
            if (status == "NOK" && iss.eof()){
                cout << "auction could not be started\n";
            }

            else if (status == "NLG" && iss.eof()){
                cout << "user not logged in\n";
            }

            else if (status == "OK"){
                if (iss >> aid && iss.eof()){
                    if (aid.size() == 3 && isNumeric(aid)){
                        cout << "successful request, AID = " << aid << "\n";
                    }
                    else cout << "incorrect AID\n";
                    
                } else cout << "incorrect response\n";

            } else cout << "incorrect response\n";

        } else cout << "incorrect response\n";

    } else cout << "incorrect response\n";
}

string close(string command){
    
    string whichCommand, aid;    //, userID, password;
    istringstream iss(command);

    if ((userID != "" && password != "") || logged_in == true ){

        if (iss >> whichCommand >> aid && iss.eof()) {
            // TODO _ am i supposed to check if auser is logged in the user application?
            if (isNumeric(aid) && aid.size() == 3){
                tempAID = aid; //Used to return error to user if needed
                return "CLS " + userID + " " + password + " " + aid;
            }

            else {
                return "Incorrect AID";               // TODO DAR CHECK NESTES RETURNS. PASSAR SEND_UDP PARA AQUI??
            }

        } else {
            return "Incorrect command format";
        }

    } else {
        return "User not logged in User Appplication";
    }
}

void closeStatus(string response){
    istringstream iss(response);
    string command, status;

    if (iss >> command >> status && iss.eof()){

        if (command == "RCL"){
            
            if (status == "OK"){
                cout << "auction closed successfully" << endl;
            }

            else if (status == "NOK"){
                cout << "user or password incorrect" << endl;
            }

            else if (status == "NLG"){
                cout << "user not logged in" << endl;
            }

            else if (status == "EAU"){
                cout << "auction " << tempAID << " does not exist" << endl;
            }

            else if (status == "EOW"){
                cout << "user " << userID << " does not own " << "auction " << tempAID << endl;
            }

            else if (status == "END"){
                cout << "auction " << tempAID << " has already ended" << endl;
            }

            else cout << "incorrect response\n";

        } else cout << "incorrect response\n";
 
    } else cout << "incorrect response\n";
 
}

string myauctions(){
    //TODO IS IT SUPPOSE TO LOSE USER INFORMATION ON CLIENT SIDE?
    if ((userID != "" && password != "") || logged_in == true ){
        if (isUID(userID) && isPassword(password)){
            return "LMA " + userID;
        } else return "user not known"; 

    } else return "user not known";
}

void myauctionsStatus(string response){
    istringstream iss(response);
    string command, status, aid, state;

    if (iss >> command >> status){
       
        if (iss.eof()){

            if (command == "RMA"){

                if (status == "NOK"){
                    cout << "user " << userID << " does not have ongoing auctions" << endl;
                }

                else if (status == "NLG"){
                    cout << "user not logged in" << endl;
                }

                else cout << "!incorrect response" << endl;

            } else cout << "incorrect response" << endl;
        } 

        else if (command == "RMA" && status == "OK"){
                    
                while (iss >> aid >> state) {
                    /*if (iss.eof()){             //TODO KINDA POINTLESS BUT CHECK
                        cout << "Auction ID: "  << aid << ", State: " << state << endl;
                    }*/
                    cout << "Auction ID: "  << aid << ", State: " << state << endl;
                }

        } else cout << "incorrect response!" << endl; 
        
    } else cout << "incorrect response!!!" << endl;

}

string bid(string command){
    string whichCommand, aid, value;    //, userID, password;
    istringstream iss(command);

    if ((userID != "" && password != "") || logged_in == true ){

        if (iss >> whichCommand >> aid >> value && iss.eof()) {
            // TODO _ am i supposed to check if auser is logged in the user application?
            if (isNumeric(aid) && aid.size() == 3){
                tempAID = aid;

                if (isNumeric(value)){
                    return "BID " + userID + " " + password + " " + aid + " " + value;
                } else return "Invalid value";

            } else return "Invalid aid";

        } else return "Incorrect command format";

    } else return "User not logged in User Appplication";
}

void bidStatus(string response){
    istringstream iss(response);
    string command, status;
    if (iss >> command >> status && iss.eof()){

        if (command == "RBD"){
            
            if (status == "ACC"){
                cout << "Bid accepted" << endl;
            }

            else if (status == "NOK"){
                cout << "Auction " << tempAID << " is not active" << endl;
            }

            else if (status == "NLG"){
                cout << "User not logged in" << endl;
            }

            else if (status == "REF"){
                cout << "Bid refused. Larger bid placed previously" << endl;
            }

            else if (status == "ILG"){
                cout << "Cannot bid on auction hosted by yourself" << endl;
            }

            else cout << "incorrect response\n";

        } else cout << "incorrect response\n";
 
    } else cout << "incorrect response\n";
}

string mybids(){
    //TODO IS IT SUPPOSE TO LOSE USER INFORMATION ON CLIENT SIDE?
    if ((userID != "" && password != "") || logged_in == true ){
        if (isUID(userID) && isPassword(password)){
            return "LMB " + userID;
        } else return "user not known"; 
    } else return "user not known";
}

void mybidsStatus(string response){
    istringstream iss(response);
    string command, status, aid, state;

    if (iss >> command >> status){
       
        if (iss.eof()){

            if (command == "RMB"){

                if (status == "NOK"){
                    cout << "user " << userID << " does not have ongoing bids" << endl;
                }

                else if (status == "NLG"){
                    cout << "user not logged in" << endl;
                }

                else cout << "!incorrect response" << endl;

            } else cout << "incorrect response" << endl;
        } 

        else if (command == "RMB" && status == "OK"){
                    
                while (iss >> aid >> state) {
                    /*if (iss.eof()){             //TODO KINDA POINTLESS BUT CHECK
                        cout << "Auction ID: "  << aid << ", State: " << state << endl;
                    }*/
                    cout << "Auction ID: "  << aid << ", State: " << state << endl;
                }

        } else cout << "incorrect response!" << endl; 
        
    } else cout << "incorrect response!!!" << endl;

}

string list(string command){
    //TODO IS IT SUPPOSE TO LOSE USER INFORMATION ON CLIENT SIDE?
    string whichCommand;    //, userID, password;
    istringstream iss(command);
    if (iss >> whichCommand && iss.eof()) {
        return "LST";      
    } else return "Incorrect command format";
}

void listStatus(string response){
    istringstream iss(response);
    string command, status, aid, state;

    if (iss >> command >> status){
       
        if (iss.eof()){

            if (command == "RLS" && status == "NOK"){
                cout << "No auction has been started" << endl;

            } else cout << "incorrect response" << endl;
        } 

        else if (command == "RLS" && status == "OK"){
                    
                while (iss >> aid >> state) {
                    /*if (iss.eof()){             //TODO KINDA POINTLESS BUT CHECK
                        cout << "Auction ID: "  << aid << ", State: " << state << endl;
                    }*/
                    cout << "Auction ID: "  << aid << ", State: " << state << endl;
                }

        } else cout << "incorrect response" << endl; 
        
    } else cout << "incorrect response" << endl;
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
            result = sendTCP(request, fName);
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