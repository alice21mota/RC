#include "functionsUDP.h"

/**
 * Cria todas as diretorias e ficheiros necessários para guardar as informações do utilizador.
*/
void createUser(string userId, string password) {
    // cout << "creating the folder struct" << endl; // Debug
    createUserFolder(userId);
    // createHostedFolder(userId);
    // createBiddedFolder(userId);
    createUserPasswordFile(userId, password);
    createUserLoginFile(userId);
    // cout << "User folder struct created" << endl; // Debug
}

string login(string userId, string password) {
    string status;
    // cout << "login do user " << userId << "com a password " << password << endl; // Debug

    // TODO: check if user and password are allowed (char number etc)

    if (isRegisteredUser(userId)) {
        if (!isCorrectPassword(userId, password)) { status = "NOK"; }
        else {
            createUserLoginFile(userId);
            status = "OK";
        }
    }
    else {
        createUser(userId, password);
        status = "REG";
    }
    return "RLI " + status;
}

string logout(string userId, string password) {
    string status;

    if (isLoggedIn(userId)) {
        if (!isCorrectPassword(userId, password)) status = "NOK"; // incorrect password
        else if (deleteUserLoginFile(userId)) status = "OK"; // unregister user
        else status = "NOK";// something gone wrong deleting the file
    }
    else if (!isRegisteredUser(userId)) status = "UNR"; // not registered
    else status = "NOK"; // not loggedin

    return "RLO " + status;
}

string unregister(string userId, string password) {
    string status;

    if (!isRegisteredUser(userId)) status = "UNR"; // not registered
    else if (isLoggedIn(userId)) {
        if (!isCorrectPassword(userId, password)) status = "NOK";   // incorrect password
        else {
            if (deleteUserLoginFile(userId) && deleteUserPasswordFile(userId)) status = "OK";    // unregister user
            else status = "NOK"; // something gone wrong deleting the files
        }
    }
    else status = "NOK"; // not loggedin
    return "RUR " + status;
}


string getMyAuctions(string userId) {
    string status;
    if (!hasOngoingAuctions(userId)) status = "NOK"; // FIXME check what sould return if is an *unlogged* user *without* bids
    else if (!isLoggedIn(userId)) status = "NLG";
    else {
        status = "OK";
        filesystem::path directoryPath("USERS/" + userId + "/HOSTED/");
        vector<string> auctions = getSortedFilesFromDirectory(directoryPath);

        auctions = removeExtensionFromVector(auctions);

        int nAuctions = auctions.size();
        for (int i = 0;i < nAuctions;i++) {
            status += " " + auctions[i] + " " + (isAuctionActive(auctions[i]) ? "1" : "0");
        }
    }
    return "RMA " + status;
}


string getMyBids(string userId) {
    string status;
    if (!hasOngoingBids(userId)) status = "NOK";    // FIXME check what sould return if is an *unlogged* user *without* bids
    else if (!isLoggedIn(userId)) status = "NLG";
    else {
        status = "OK";
        filesystem::path directoryPath("USERS/" + userId + "/BIDDED/");
        vector<string> bids = getSortedFilesFromDirectory(directoryPath);

        bids = removeExtensionFromVector(bids);

        int nBids = bids.size();
        for (int i = 0;i < nBids;i++) {
            status += " " + bids[i] + " " + (isAuctionActive(bids[i]) ? "1" : "0");
        }
    }
    return "RMB " + status;
}

string listAuctions() {
    string status;
    if (!existAuctions()) status = "NOK";    // FIXME check what sould return if is an *unlogged* user *without* bids
    else {
        status = "OK";
        filesystem::path directoryPath("AUCTIONS/");
        vector<string> auctions = getSortedFilesFromDirectory(directoryPath);

        auctions = removeExtensionFromVector(auctions);

        int nAuctions = auctions.size();
        for (int i = 0;i < nAuctions;i++) {
            status += " " + auctions[i] + " " + (isAuctionActive(auctions[i]) ? "1" : "0");
        }
    }
    return "RLS " + status;
}

string auctionToString(string auctionId) {
    filesystem::path startFilePath("AUCTIONS/" + auctionId + "/START_" + auctionId + ".txt");
    string infos = readFromFile(startFilePath);

    string timeactive = to_string(getAuctionTimeactive(auctionId));
    int indexTimeactive = infos.find(timeactive);
    infos = infos.erase(indexTimeactive, timeactive.length() + 2);
    infos += timeactive;

    string startFulltime = to_string(getAuctionStartFullTime(auctionId));
    int indexStartFulltime = infos.find(startFulltime);
    infos = infos.erase(indexStartFulltime, startFulltime.length());

    // remove all ,
    int index = infos.find(',');
    while (index != -1) {
        infos = infos.erase(index, 1);
        index = infos.find(',');
    }

    return infos;
}

string getBidsString(string auctionId) {
    string final = "";
    filesystem::path directotyPath = "AUCTIONS/" + auctionId + "/BIDS/";
    vector<string> bids = getSortedFilesFromDirectory(directotyPath);
    int nBids = bids.size();
    for (int i = 0;i < nBids; i++) {
        filesystem::path bidFilePath = directotyPath / bids[i];
        final += " B " + readFromFile(bidFilePath);
    }
    return final;
}

string showRecord(string auctionId) {
    string command = "RRC ";
    string finalString;
    if (!isExistingAuction(auctionId)) return command + "NOK";

    finalString = auctionToString(auctionId);

    if (hasAnyBid(auctionId)) {
        finalString += getBidsString(auctionId);
    }

    if (!isAuctionActive(auctionId)) {
        filesystem::path endFilePath = "AUCTIONS/" + auctionId + "/END_" + auctionId + ".txt";
        finalString += " E " + readFromFile(endFilePath);
    }

    return command + "OK " + finalString;
}