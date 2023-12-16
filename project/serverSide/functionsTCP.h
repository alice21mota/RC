#ifndef FUNCTIONSTCP
#define FUNCTIONSTCP

#include "utils.h"
#include "filesystem.h"
#include "manageUsersFolder.h"
#include "manageAuctionsFolder.h"

// #include <string>

#include <iostream>
using namespace std;

string open(string userId, string password, string name, string start_value, string timeactive, string Fname, string Fsize, string Fdata);

bool createAuction(string auctionId, string userId, string name, string start_value, string timeactive, string Fname, string Fsize, string Fdata);

string showAsset(string auctionId);

string closeAuction(string userId, string password, string auctionId);

bool createBid(string auctionId, int bid, string userId);

string addBid(string userId, string password, string auctionId, int big);

#endif