#include "functionsTCP.h"

string open(string userId, string password, string name, string start_value, string timeactive, string Fname, string Fsize, string Fdata) {
    cout << "entrei no OPEN" << endl; // Debug
    int auctionId;
    string command = "ROA ";

    if (!isLoggedIn(userId)) return command + "NLG";    // FIXME what should return first
    if (!isCorrectPassword(userId, password)) return command + "NOK"; // FIXME what should return first

    if (!existAuctions())
        if (!createAuctionsFolder()) return command + "NOK"; // FIXME: should i check this (?)
    cout << "já há o auctions folder";

    for (int i = 0;i < 10;i++) {
        auctionId = createAuction(userId, name, start_value, timeactive, Fname, Fsize, Fdata);
        if (auctionId > 0) return command + "OK " + to_string(auctionId);
        cout << "-------------------------------------\n--------------------------------------------\n";
    }

    return command + "NOK";

}

int createAuction(string userId, string name, string start_value, string timeactive, string Fname, string Fsize, string Fdata) {
    string auctionId;
    if ((auctionId = createAuctionFolder()) == "-1") return -1;
    cout << "auctionId  = " << auctionId << endl; // Debug
    if (!createAuctionAssetFolder(auctionId)) return -1;
    if (!writeAsset(auctionId, Fname, Fsize, Fdata)) return -1;
    if (!createAuctionStartFile(auctionId, userId, name, start_value, timeactive, Fname)) return -1;
    if (!createUserHostedFile(userId, auctionId)) return -1;
    return stoi(auctionId);
}

string showAsset(string auctionId) {
    string status;
    if (!isExistingAuction(auctionId)) status = "NOK";
    else
    {
        filesystem::path directoryPath = "AUCTIONS/" + auctionId + "/ASSET/";
        vector<string> files = getSortedFilesFromDirectory(directoryPath);
        if (files.size() != 1) status = "NOK";
        else {
            string Fname = files[0];
            filesystem::path filePath = directoryPath / Fname;
            string Fdata = getFileData(filePath);
            if (Fdata == "-1") status = "NOK";
            else {
                int Fsize = Fdata.length();
                status = "OK " + Fname + " " + to_string(Fsize) + " " + Fdata;
            }
        }
    }
    return "RSA " + status;
}

string closeAuction(string userId, string password, string auctionId) {
    string command = "RCL ";
    if (!isLoggedIn(userId)) return command + "NLG";
    if (!isExistingAuction(auctionId)) return command + "EAU";
    if (!isOwner(userId, auctionId)) return command + "EOW";
    if (!isAuctionActive(auctionId)) return command + "END";
    if (!isCorrectPassword(userId, password)) return command + "NOK"; //FIXME: não está no enunciado
    if (!createAuctionEndFile(auctionId)) return command + "NOK";
    return command + "OK";
}

int getLastBid(string auctionId) {
    filesystem::path directoryPath("AUCTIONS/" + auctionId + "/BIDS/");
    if (!hasAnyBid(auctionId)) return 0;
    vector<string> bids = getSortedFilesFromDirectory(directoryPath);
    if (bids.size() < 1) return -1;
    return stoi(removeExtension(bids.back()));
}

bool createBid(string auctionId, int bid, string userId) {
    if (!createAuctionBidFile(auctionId, bid, userId)) return false;
    return createUserBiddedFile(userId, auctionId);
}

string addBid(string userId, string password, string auctionId, int bid) {
    string command = "RBD ";
    int lastBid;
    if (!isAuctionActive(auctionId)) return command + "NOK";
    if (!isLoggedIn(userId)) return command + "NLG";
    if (isOwner(userId, auctionId)) return command + "ILG";
    if (!isCorrectPassword(userId, password)) return command + "NOK";
    if ((lastBid = getLastBid(auctionId)) == -1) return command + "NOK";
    if (bid < lastBid) return command + "REF";
    if (!createBid(auctionId, bid, userId)) return command + "NOK";
    return command + "ACC";
}