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