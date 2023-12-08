#include "functionsTCP.h"

string getAID() {
    filesystem::path directoryPath = "AUCTIONS/";
    vector<string> filenames = getSortedFilesFromDirectory(directoryPath);
    if (filenames.size() == 0) return "001";
    string lastAID = getSortedFilesFromDirectory(directoryPath).back();
    int newId = stoi(lastAID) + 1;
    ostringstream oss;
    oss << setw(3) << std::setfill('0') << newId;

    return oss.str();
}

string open(string userId, string password, string name, string start_value, string timeactive, string Fname, string Fsize, string Fdata) {
    string status;

    if (!isLoggedIn(userId)) status = "NLG";    // FIXME what should return first
    else if (!isCorrectPassword(userId, password)) status = "NOK"; // FIXME what should return first
    else {
        string aid = getAID();

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
