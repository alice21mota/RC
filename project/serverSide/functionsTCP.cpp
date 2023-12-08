#include "functionsTCP.h"

int getAID() {
    filesystem::path directoryPath = "AUCTIONS/";
    string lastAID = getSortedFilesFromDirectory(directoryPath).back();
    return stoi(lastAID) + 1;
}

string open(string userId, string password, string name, string start_value, string timeactive, string Fname, string Fsize, string Fdata) {
    string status;

    if (!isLoggedIn(userId)) status = "NLG";    // FIXME what should return first
    else if (!isCorrectPassword(userId, password)) status = "NOK"; // FIXME what should return first
    else {
        string aid = to_string(getAID());

        filesystem::path assetFilePath = "AUCTIONS/" + aid + "/ASSET/" + Fname;

        if (!writeFile(assetFilePath, Fsize, Fdata)) status = "NOK";
        else {
            filesystem::path startFilePath = "AUCTIONS/" + aid + "START_" + aid + ".txt";

            time_t start_fulltime = getSeconds();
            string start_datetime = secondsToDate(start_fulltime);

            string contentStartFile = userId + ", " + name + ", " + Fname + ", " + start_value + ", " + timeactive + ", " + start_datetime + ", " + to_string(start_fulltime);
            if (!createFile(startFilePath, contentStartFile)) status = "NOK";
            else status = "OK";
        }
    }
    return "ROA " + status;
}
