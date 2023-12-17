#include "manageUsersFolder.h"

// bool createUsersFolder() {
//     filesystem::path directoryPath("USERS");
//     return createFolder(directoryPath);
// }

bool createUserFolder(string userId) {
    filesystem::path directoryPath("USERS/" + userId);
    return createFolder(directoryPath);
}

bool createUserPasswordFile(string userId, string password) {
    filesystem::path filePath = "USERS/" + userId + "/" + userId + "_pass.txt";
    if (createFile(filePath, password)) return true;
    return false;
}

bool deleteUserPasswordFile(string userId) {
    filesystem::path filePath = "USERS/" + userId + "/" + userId + "_pass.txt";
    return filesystem::remove(filePath);
}

bool createUserLoginFile(string userId) {
    filesystem::path filePath = "USERS/" + userId + "/" + userId + "_login.txt";
    return createFile(filePath);
}

bool deleteUserLoginFile(string userId) {
    filesystem::path filePath = "USERS/" + userId + "/" + userId + "_login.txt";
    return filesystem::remove(filePath);
}

bool createUserHostedFolder(string userId) {
    filesystem::path directoryPath("USERS/" + userId + "/HOSTED/");
    return createFolder(directoryPath);
}

bool createUserHostedFile(string userId, string auctionId) {
    if (!hasOngoingAuctions(userId))
        if (!createUserHostedFolder(userId))
            return false;

    filesystem::path filePath = "USERS/" + userId + "/HOSTED/" + auctionId + ".txt";
    return createFile(filePath);
}

bool createUserBiddedFolder(string userId) {
    filesystem::path directoryPath("USERS/" + userId + "/BIDDED/");
    return createFolder(directoryPath);
}

bool createUserBiddedFile(string userId, string auctionId) {
    if (!hasOngoingBids(userId))
        if (!createUserBiddedFolder(userId))
            return false;

    filesystem::path filePath = "USERS/" + userId + "/BIDDED/" + auctionId + ".txt";
    return createFile(filePath);
}