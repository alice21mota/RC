#ifndef MANAGERAUCTIONSFOLDER
#define MANAGERAUCTIONSFOLDER

#include <filesystem>
#include <sstream>
#include <fstream>

#include "filesystem.h"
#include "utils.h"

#include <iostream>
using namespace std;

const int FULLTIME_NCHARS = 10;
const int USERID_NCHARS = 6;

string getAID();

bool createAuctionsFolder();

/**
 * @returns auctionId
*/
string createAuctionFolder();

bool createAuctionAssetFolder(string auctionId);

bool writeAsset(string auctionId, string filePath, string fileSize, string fileData);

bool createAuctionStartFile(string auctionId, string userId, string name, string start_value, string timeactive, string Fname);

bool createAuctionEndFile(string auctionId);

bool createBidsFolder(string auctionId);

bool createBidFile(string auctionId, int bid_value);

int getAuctionStartFullTime(string auctionId);

int getAuctionTimeactive(string auctionId);

string getAuctionOwner(string auctionId);

int checkExpiredAuctions();

#endif