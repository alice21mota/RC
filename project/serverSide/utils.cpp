#include "utils.h"


bool userAlreadyExists(string userId) {
    string directoryPath = "USERS/" + userId;
    return filesystem::exists(directoryPath);
}

bool isRegistedUser(string userId) {
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
bool hasOngoingAudictions(string userId) {
    // TODO
    return false;
}

bool isAudictionActive(string audictionId) {
    filesystem::path filePath = "AUCTIONS/" + audictionId + "/END_" + audictionId + ".txt";
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