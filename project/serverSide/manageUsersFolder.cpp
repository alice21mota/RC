#include "manageUsersFolder.h"

void createUsersFolder() {
    filesystem::path directoryPath("USERS");
    createFolder(directoryPath);
}

void createUserFolder(string userId) {
    filesystem::path directoryPath("USERS/" + userId);
    createFolder(directoryPath);
}

bool createUserPasswordFile(string userId, string password) {
    filesystem::path filePath = "USERS/" + userId + "/" + userId + "_pass.txt";
    if (createFile(filePath, password) != -1) return true;
    return false;
}

bool deleteUserPasswordFile(string userId) { // TODO: deal with the errors
    filesystem::path filePath = "USERS/" + userId + "/" + userId + "_pass.txt";
    return filesystem::remove(filePath);
}

void createUserLoginFile(string userId) {
    filesystem::path filePath = "USERS/" + userId + "/" + userId + "_login.txt";
    createFile(filePath);
}

bool deleteUserLoginFile(string userId) { // TODO: deal with the errors
    filesystem::path filePath = "USERS/" + userId + "/" + userId + "_login.txt";
    if (filesystem::remove(filePath) != -1)
        return true;
    else return false;
}

void createHostedFolder(string userId) {
    filesystem::path directoryPath("USERS/" + userId + "/HOSTED/");
    createFolder(directoryPath);
}

void createBiddedFolder(string userId) {
    filesystem::path directoryPath("USERS/" + userId + "/BIDDED/");
    createFolder(directoryPath);
}
