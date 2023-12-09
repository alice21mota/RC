#ifndef MANAGERAUCTIONSFOLDER
#define MANAGERAUCTIONSFOLDER

#include <filesystem>
#include <sstream>
#include <fstream>

#include "filesystem.h"

#include <iostream>
using namespace std;

bool createAuctionsFolder();

bool createAuctionFolder(string auctionId);

// bool createAuctionStartFile(string auctionId);

// bool deleteAuctionEndFile(string auctionId);

bool createAuctionAssetFolder(string auctionId);


#endif