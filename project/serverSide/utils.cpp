#include "utils.h"


bool userAlreadyExists(string userId) {
    string directoryPath = "USERS/" + userId;
    return filesystem::exists(directoryPath);
}

bool isRegisteredUser(string userId) {
    filesystem::path filePath = "USERS/" + userId + "/" + userId + "_pass.txt";
    return filesystem::exists(filePath);
}

bool isCorrectPassword(string userId, string passwordToTest) {
    filesystem::path filePath = "USERS/" + userId + "/" + userId + "_pass.txt";
    string corretPassword = readFromFile(filePath);
    // cout << "corretPassword " << corretPassword << endl; // Debug
    // cout << "passwordToTest " << passwordToTest << endl; // Debug
    return corretPassword == passwordToTest ? true : false;
}

bool isLoggedIn(string userId) {
    filesystem::path filePath = "USERS/" + userId + "/" + userId + "_login.txt";
    return filesystem::exists(filePath);
}

// FIXME :  not sure se pode ser só checkar se a pasta HOSTED
bool hasOngoingAuctions(string userId) {
    filesystem::path directoryPath = "USERS/" + userId + "/" + "HOSTED";
    return filesystem::exists(directoryPath);
}

bool isAuctionActive(string auctionId) {
    filesystem::path filePath = "AUCTIONS/" + auctionId + "/END_" + auctionId + ".txt";
    return !(filesystem::exists(filePath));
}


// FIXME :  not sure se pode ser só checkar se a pasta BIDDED
bool hasOngoingBids(string userId) {
    filesystem::path directoryPath = "USERS/" + userId + "/" + "BIDDED";
    return filesystem::exists(directoryPath);
}

// FIXME :  not sure se pode ser só checkar se a pasta AUCTIONS
bool existAuctions() {
    filesystem::path directoryPath = "AUCTIONS/";
    return filesystem::exists(directoryPath);
}

bool isExistingAuction(string auctionId) {
    filesystem::path directoryPath = "AUCTIONS/" + auctionId;
    return filesystem::exists(directoryPath);
}

// string getDate() {
//     time_t  fulltime;
//     struct tm *current_time;
//     char time_str[20];

//     time(&fulltime); // Get current time in seconds starting at 1970−...
//     current_time = gmtime(&fulltime); // Convert time to YYYY−MM−DD  HH:MM: SS .    current time points to a struct of type tm 
//     sprintf(time_str, "%4d-%02d-%02d %02d:%02d:%02d",
//         current_time->tm_year + 1900,
//         current_time->tm_mon + 1,
//         current_time->tm_mday,
//         current_time->tm_hour,
//         current_time->tm_min,
//         current_time->tm_sec
//     );
//     return time_str;
// }

time_t getSeconds() {
    time_t  fulltime;
    time(&fulltime); // Get current time in seconds starting at 1970−...
    return fulltime;
}

string secondsToDate(time_t  fulltime) {
    struct tm *current_time;
    char time_str[20];

    time(&fulltime); // Get current time in seconds starting at 1970−...
    current_time = gmtime(&fulltime); // Convert time to YYYY−MM−DD  HH:MM: SS .    current time points to a struct of type tm 
    sprintf(time_str, "%4d-%02d-%02d %02d:%02d:%02d",
        current_time->tm_year + 1900,
        current_time->tm_mon + 1,
        current_time->tm_mday,
        current_time->tm_hour,
        current_time->tm_min,
        current_time->tm_sec
    );
    return time_str;
}