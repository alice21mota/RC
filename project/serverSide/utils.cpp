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

// TODO :  se puder ser, é só checkar se a pasta HOSTED
bool hasOngoingAuctions(string userId) {
    // TODO
    return false;
}

bool isAuctionActive(string auctionId) {
    filesystem::path filePath = "AUCTIONS/" + auctionId + "/END_" + auctionId + ".txt";
    return !(filesystem::exists(filePath));
}


// TODO :  se puder ser, é só checkar se a pasta BIDDED
bool hasOngoingBids(string userId) {
    // TODO
    return false;
}

// TODO :  se puder ser, é só checkar se a pasta BIDDED
bool existAuctions() {
// TODO
    return false;
}