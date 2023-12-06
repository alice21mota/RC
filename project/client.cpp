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

using namespace std;

string port = "58000";
string ip = "localhost";
string userID, password;
string fName, fData;
//#define PORT "58001"
bool logged_in = false;


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

    //cout << "UDP returned: " << buffer << endl; // Debug

    freeaddrinfo(res);
    close(fd);

    return buffer;
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
    message = message + "\n";

    int fd, errcode;
    ssize_t n;
    socklen_t addrlen;
    char buffer[128];

    struct addrinfo hints, *res;
    struct sockaddr_in addr;

    fd = socket(AF_INET, SOCK_STREAM, 0);
    if (fd == -1)
    {
        exit(1);
    }

    memset(&hints, 0, sizeof hints);
    hints.ai_family = AF_INET; // IPv4
    hints.ai_socktype = SOCK_STREAM;

    errcode = getaddrinfo(ip.c_str(), port.c_str(), &hints, &res);
    if (errcode != 0)
    {
        exit(1);
    }

    n = connect(fd, res->ai_addr, res->ai_addrlen);
    if (n == -1)
    {
        exit(1);
    }

    n = write(fd, message.c_str(), message.length());
    if (n == -1)
    {
        exit(1);
    }

    if (fileName != ""){
        
        sendFileChunks(fd, fileName);
    }

    n = read(fd, buffer, 128);
    if (n == -1)
    {
        exit(1);
    }
    
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
    return (password.size()== 8 && isAlphanumeric(password));
}

bool isDescription(string description){
    if (description.size() > 0 && description.size() <= 10){
        if (isAlphanumeric(description))
            return true;
        return false;
    }
    return false;
}

bool isFileName(string fileName){
    const std::regex pattern("^([0-9]{1,21}\\.)?[a-zA-Z0-9_-]{1,21}\\.[a-zA-Z0-9_-]{3}$");

    return regex_match(fileName, pattern);
}

bool isStartValue(string startValue){
    if (startValue.size() > 0 && startValue.size() <= 6){
        if (isNumeric(startValue))
            return true;
        return false;
    }
    return false;
}

bool isDuration(string duration){
    if (duration.size() > 0 && duration.size() <= 5){
        if (isNumeric(duration))
            return true;
        return false;
    }
    return false;
}

// Does file exists?
bool fileExists(string asset_fname){
    return filesystem::exists(asset_fname);
}

// Reads the contents of the selected file
/*string getFileContents(string asset_fname) {
    ifstream file(asset_fname, ios::binary);
    if (!file) {
        cerr << "Error opening file: " << asset_fname << endl;
        exit(1);
    }

    ostringstream contents;
    contents << file.rdbuf();
    file.close();

    return contents.str();
}*/

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

    if (iss >> whichCommand >> userID >> password && iss.eof()) {
        // Use userID and password as needed
        if (isUID(userID) && isPassword(password)){
            return "LIN " + userID + " " + password;
        }

        else {
            //TODO -> Passar o userID e password a ""?
            return "INCORRECT LOGIN ATTEMPT";               // TODO DAR CHECK NESTES RETURNS. PASSAR SEND_UDP PARA AQUI??
        }

    } else {
        return "INCORRECT LOGIN ATTEMPT";
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

    else return "USER NOT LOGGED IN";
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
        } else return "USER NOT KNOWN";    
    }

    /*if (logged_in = true)
        return "UNR " + userID + " " + password;*/

    //TODO IS IT SUPPOSE TO LOSE USER INFORMATION ON CLIENT SIDE?
    else return "USER NOT KNOWN";
}

void unregisterStatus(string status){
    
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
            cout << "W>Rnadhf\n";
            if (isDescription(name) && isStartValue(start_value) && isDuration(timeactive)){
                cout << "asdadads\n";
                
                //Check if the file exists
                if (fileExists(asset_fname)) {

                    fName = asset_fname;
                    //fData = getFileContents(fName);
                    fSize = fileSizeString(fName);

                    toReturn = "OPA " + userID + " " + password + " " + name + " " + start_value + " " +
                            timeactive + " " + fName + " " + fSize; // + " " + fData;
                    return toReturn;
                }

                //TODO FILE SIZE (Fsize)
                //TODO CONTENTS OF THE FILE (Fdata)
                else {
                    return "FILE NOT FOUND";
                }
            }

            else {
                //TODO -> Passar o userID e password a ""?
                return "INCORRECT OPEN";               // TODO DAR CHECK NESTES RETURNS. PASSAR SEND_UDP PARA AQUI??
            }

        } else {
            cout << "sdadadasasdadasd\n";
            return "INCORRECT OPEN";
        }
        
    }
    return "NOT LOGGED IN USER APPLICATION";

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

//TODO Não esquecer de dar check do reply do server para cada command
void getCommand(string command){
    string whichCommand, request, status, result;
    //getline (command, whichCommand, " ");
    whichCommand = command.substr(0, command.find(" "));

    if (whichCommand == "login"){
        request = login(command);

        if (request != "INCORRECT LOGIN ATTEMPT"){
            cout << request << "\n";
            result = sendUDP(request);
            status = result.substr(0, result.find('\n'));
            //cout << "Status is->" << status <<"\nABC\n";
            cout << "RESPONSE:";
            loginStatus(status);

        } else {
            cout << "INCORRECT LOGIN\n";
        }

    } else if (whichCommand == "logout") {
        request = logout();

        if (request != "USER NOT LOGGED IN"){
            cout << request << "\n";
            result = sendUDP(request);
            status = result.substr(0, result.find('\n'));
            cout << "Status is->" << status <<"\n";
            cout << "RESPONSE:";
            logoutStatus(status);
        } else {
            cout << "INCORRECT LOGOUT\n";
        }

    } else if (whichCommand == "unregister") {
        request = unregister();

        if (request != "USER NOT KNOWN"){
            cout << request << "\n";
            result = sendUDP(request);
            status = result.substr(0, result.find('\n'));
            cout << "Status is->" << status <<"\n";
            cout << "RESPONSE:";
            unregisterStatus(status);

        } else {
            cout << "INCORRECT UNREGISTER\n";
        }

    } else if (whichCommand == "exit") {
        exit();
        //cout << "exit\n";

    } else if (whichCommand == "open") {
        request = open(command);

        if (request != "INCORRECT OPEN" && request != "FILE NOT FOUND" && request != "NOT LOGGED IN USER APPLICATION"){
            cout << request << "\n";
            cout << fName << " <- FILE NAME\n";
            result = sendTCP(request, fName);
            status = result.substr(0, result.find('\n'));
            cout << "Status is->" << status <<"\n";
            //cout << "RESPONSE:";

        } else {
            cout << "FAIL OPEN\n";
        }

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

    //cout << port << ip << endl;

    while (1){

        user_command = readCommands();
        getCommand(user_command);
    }
    cout << port << "\n" << ip << "\n";
    cout << "done\n";
    //result = sendTCP("CLS 102500 passpass 123");
    /*bool Exists = fileExists("TESTE REDES.pdf");
    string contents = getFileContents("TESTE REDES.pdf");
    size_t size = getFileSize("TESTE REDES.pdf");
    string sizesize = fileSizeString("TESTE REDES.pdf");
    cout << Exists << " false is 0 ?\n";
    cout << contents << " <- contents; size -> " << size << "\n" << sizesize;*/
    /*bool name = isFileName("IST-1.png");
    cout << "IST -> " << name << "\n";
    bool name1 = isFileName("ABCD.tt");
    cout << "ABCD -> " << name1 << "\n";
    bool name2 = isFileName("monet_0-1121212121212.png");
    cout << "Monet -> " << name2 << "\n";
    bool name3 = isFileName("monet_01");
    cout << "wrong1 -> " << name3 << "\n";
    bool name4 = isFileName(".png");
    cout << "wrong2 -> " << name4 << "\n";*/
    cout << "--------" << result << endl; // Debug
}