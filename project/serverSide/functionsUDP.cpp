#include "functionsUDP.h"

/**
 * Cria todas as diretorias e ficheiros necessários para guardar as informações do utilizador.
*/
bool createUser(string userId, string password) {
    filesystem::path directoryPath("USERS/" + userId);
    if (!filesystem::exists(directoryPath))
        if (!createUserFolder(userId))
            return false;
    return
        createUserPasswordFile(userId, password) &&
        createUserLoginFile(userId);
}

string login(string userId, string password) {
    string status;
    // cout << "login do user " << userId << "com a password " << password << endl; // Debug

    // TODO: check if user and password are allowed (char number etc)

    if (isRegisteredUser(userId)) {
        if (!isCorrectPassword(userId, password)) { status = "NOK"; }
        else {
            if (createUserLoginFile(userId)) { status = "OK"; }
            else { status = "NOK"; }
        }
    }
    else {
        if (createUser(userId, password)) { status = "REG"; }
        else { status = "NOK"; }
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
        if (auctions.size() < 1) return "-1";

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
        if (bids.size() < 1) return "-1";

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
        if (auctions.size() < 1) return "-1";

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

    if (infos == "-1") return "-1";

    string timeactive = to_string(getAuctionTimeactive(auctionId));
    if (timeactive == "-1") return "-1";

    int indexTimeactive = infos.find(timeactive);
    infos = infos.erase(indexTimeactive, timeactive.length() + 2);
    infos += timeactive;

    string startFulltime = to_string(getAuctionStartFullTime(auctionId));
    if (startFulltime == "-1") return "-1";

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
    string bid;
    filesystem::path directotyPath = "AUCTIONS/" + auctionId + "/BIDS/";
    vector<string> bids = getSortedFilesFromDirectory(directotyPath);
    int nBids = min((int)bids.size(), 50);
    for (int i = 0;i < nBids; i++) {
        filesystem::path bidFilePath = directotyPath / bids[i];
        bid = "";
        bid = readFromFile(bidFilePath);
        if (bid == "-1") return final;
        final += " B " + bid;
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