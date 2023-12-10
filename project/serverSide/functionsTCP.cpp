#include "functionsTCP.h"

string open(string userId, string password, string name, string start_value, string timeactive, string Fname, string Fsize, string Fdata) {
    // cout << "entrei no OPEN" << endl; // Debug
    string status;
    string aid;

    if (!isLoggedIn(userId)) status = "NLG";    // FIXME what should return first
    else if (!isCorrectPassword(userId, password)) status = "NOK"; // FIXME what should return first
    else {
        if (!existAuctions()) createAuctionsFolder(); // FIXME: should i check this (?)
        if ((aid = createAuctionFolder()) == "-1") status = "NOK";
        else
        {
            cout << "aid  = " << aid << endl; // Debug
            if (!createAuctionAssetFolder(aid)) status = "NOK";
            else {
                if (!writeAsset(aid, Fname, Fsize, Fdata)) status = "NOK";
                else {
                    cout << "escreveu o file" << endl; // Debug
                    if (!createAuctionStartFile(aid, userId, name, start_value, timeactive, Fname)) status = "NOK";
                    else status = "OK " + aid;
                }

            }
        }
    }
    return "ROA " + status;
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
            string Fdata = readFromFile(filePath);
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
    if (!createAuctionEndFile(auctionId)) return command + "NOK";
    return command + "OK";
}