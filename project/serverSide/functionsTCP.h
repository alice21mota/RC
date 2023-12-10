#ifndef FUNCTIONSTCP
#define FUNCTIONSTCP

#include "utils.h"
#include "filesystem.h"
#include "manageAuctionsFolder.h"

// #include <string>

#include <iostream>
using namespace std;

string open(string userId, string password, string name, string start_value, string timeactive, string Fname, string Fsize, string Fdata);

string showAsset(string auctionId);

string closeAuction(string userId, string password, string auctionId);

string addBid(string userId, string password, string auctionId, int big);

#endif