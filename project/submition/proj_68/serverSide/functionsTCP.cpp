#include "functionsTCP.h"

string open(string userId, string password, string name, string start_value, string timeactive, string Fname, string Fsize, string Fdata) {
    // cout << "entrei no OPEN" << endl; // Debug
    string aid;
    string command = "ROA ";

    if (!isLoggedIn(userId)) return command + "NLG";
    if (!isCorrectPassword(userId, password)) return command + "NOK";

    if (!existAuctions())
        if (!createAuctionsFolder()) return command + "NOK";
    // cout << "já há o auctions folder"; // Debug

    if ((aid = createAuctionFolder()) == "-1") return command + "NOK";
    // cout << "aid  = " << aid << endl; // Debug

    if (!createAuction(aid, userId, name, start_value, timeactive, Fname, Fsize, Fdata)) return command + "NOK";
    return command + "OK " + aid;

}

bool createAuction(string auctionId, string userId, string name, string start_value, string timeactive, string Fname, string Fsize, string Fdata) {
    if (!createAuctionAssetFolder(auctionId)) return false;
    if (!createAuctionStartFile(auctionId, userId, name, start_value, timeactive, Fname)) return false;
    if (!writeAsset(auctionId, Fname, Fsize, Fdata)) return false;
    if (!createUserHostedFile(userId, auctionId)) return false;
    return true;
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
    if (!isCorrectPassword(userId, password)) return command + "NOK";
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