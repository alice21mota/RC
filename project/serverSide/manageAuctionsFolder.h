#ifndef MANAGERAUCTIONSFOLDER
#define MANAGERAUCTIONSFOLDER

#include <filesystem>
#include <sstream>
#include <fstream>

#include "filesystem.h"
#include "utils.h"

#include <iostream>
using namespace std;

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

int getAuctionStartFullTime(string auctionId);

int getAuctionTimeactive(string auctionId);

int checkExpiredAuctions();

#endif