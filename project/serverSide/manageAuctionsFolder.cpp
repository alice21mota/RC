#include "manageAuctionsFolder.h"

string getAID() {
    filesystem::path directoryPath = "AUCTIONS/";
    vector<string> filenames = getSortedFilesFromDirectory(directoryPath);
    if (filenames.size() == 0) return "001";
    string lastAID = filenames.back();
    int newId = stoi(lastAID) + 1;
    if (newId > 999) return "-1";

    ostringstream oss;
    oss << setw(3) << setfill('0') << newId;
    return oss.str();
}

bool createAuctionsFolder() {
    filesystem::path directoryPath("AUCTIONS/");
    return createFolder(directoryPath);
}

string createAuctionFolder() {
    string auctionsId = getAID();
    if (auctionsId == "-1") return "-1";
    filesystem::path directoryPath("AUCTIONS/" + auctionsId);
    if (!createFolder(directoryPath)) return "-1";
    return auctionsId;
}

bool createAuctionAssetFolder(string auctionsId) {
    filesystem::path directoryPath("AUCTIONS/" + auctionsId + "/ASSET/");
    return createFolder(directoryPath);
}

bool writeAsset(string auctionId, string fileName, string fileSize, string fileData) {
    filesystem::path assetFilePath = "AUCTIONS/" + auctionId + "/ASSET/" + fileName;
    return writeFile(assetFilePath, fileSize, fileData);
}

bool createAuctionStartFile(string auctionId, string userId, string name, string start_value, string timeactive, string Fname) {
    filesystem::path startFilePath = "AUCTIONS/" + auctionId + "/START_" + auctionId + ".txt";

    time_t start_fulltime = getSeconds();
    string start_datetime = secondsToDate(start_fulltime);

    string contentStartFile =
        userId + ", " +
        name + ", " +
        Fname + ", " +
        start_value + ", " +
        timeactive + ", " +
        start_datetime + ", " +
        to_string(start_fulltime) + "\n";
    return createFile(startFilePath, contentStartFile);
}

bool createAuctionEndFile(string auctionId) {
    filesystem::path endFilePath = "AUCTIONS/" + auctionId + "/END_" + auctionId + ".txt";

    int timeactive = getAuctionTimeactive(auctionId);
    if (timeactive == -1) return false;

    int startFulltime = getAuctionStartFullTime(auctionId);
    if (startFulltime == -1) return false;

    long int now = getSeconds();

    int end_time;
    int end_sec_time;
    if (now > startFulltime + timeactive) {
        end_time = startFulltime + timeactive;
        end_sec_time = timeactive;
    }
    else {
        end_time = now;
        end_sec_time = now - startFulltime;
    }

    string end_datetime = secondsToDate((time_t)end_time);

    string contentEndFile = end_datetime + " " + to_string(end_sec_time);
    return createFile(endFilePath, contentEndFile);
}

bool createAuctionBidsFolder(string auctionId) {
    filesystem::path directoryPath("AUCTIONS/" + auctionId + "/BIDS/");
    return createFolder(directoryPath);
}

bool createAuctionBidFile(string auctionId, int bid_value, string userId) {
    if (!hasAnyBid(auctionId))
        if (!createAuctionBidsFolder(auctionId)) return false;

    filesystem::path bidFilePath = "AUCTIONS/" + auctionId + "/BIDS/" + to_string(bid_value) + ".txt";

    time_t bid_fulltime = getSeconds();
    string bid_datetime = secondsToDate(bid_fulltime);

    int auctionStartFullTime = getAuctionStartFullTime(auctionId);
    if (auctionStartFullTime == -1) return false;

    int bid_sec_time = bid_fulltime - auctionStartFullTime;

    string content = userId + " " + to_string(bid_value) + " " + bid_datetime + " " + to_string(bid_sec_time);
    return createFile(bidFilePath, content);
}

int getAuctionStartFullTime(string auctionId) {
    cout << "getAuctionStartFullTime " << auctionId << endl; // Debug
    filesystem::path startFilePath("AUCTIONS/" + auctionId + "/START_" + auctionId + ".txt");
    string infos = readFromFile(startFilePath);
    if (infos == "-1") return -1;
    int infosLen = infos.length();
    string start_fulltime = infos.substr(infosLen - FULLTIME_NCHARS, FULLTIME_NCHARS);
    return stoi(start_fulltime);
}

int getAuctionTimeactive(string auctionId) {
    cout << "getAuctionTimeactive " << auctionId << endl; // Debug
    filesystem::path startFilePath("AUCTIONS/" + auctionId + "/START_" + auctionId + ".txt");
    string infos = readFromFile(startFilePath);
    if (infos == "-1") return -1;

    int startPosition = 0;
    for (int i = 0; i < 4;i++) {
        startPosition = infos.find(", ", startPosition) + 2;
    }
    int endPosition = infos.find(", ", startPosition);
    string timeactive = infos.substr(startPosition, endPosition - startPosition);

    return stoi(timeactive);
}

string getAuctionOwner(string auctionId) {
    cout << "getAuctionOwner " << auctionId << endl; // Debug
    filesystem::path startFilePath("AUCTIONS/" + auctionId + "/START_" + auctionId + ".txt");
    string infos = readFromFile(startFilePath);
    if (infos == "-1") return "-1";
    return infos.substr(0, USERID_NCHARS);
}

int checkExpiredAuctions() {
    if (!existAuctions()) return 0;
    int sumExpired = 0;
    filesystem::path directoryPath("AUCTIONS/");
    vector<string> auctionsIds = getSortedFilesFromDirectory(directoryPath);
    int nAuctions = auctionsIds.size();
    for (int i = 0;i < nAuctions;i++) {
        string auctionId = auctionsIds[i];
        if (isAuctionActive(auctionId)) {
            int timeactive = getAuctionTimeactive(auctionId);
            if (timeactive == -1) return -1;

            int startFulltime = getAuctionStartFullTime(auctionId);
            if (startFulltime == -1) return -1;

            long int now = getSeconds();

            if (now > startFulltime + timeactive) {    // FIXME: deal with all of -1
                if (!createAuctionEndFile(auctionId)) return -1;
                sumExpired++;
            }
        }
    }
    return sumExpired;
}
