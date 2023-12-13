#ifndef FUNCTIONSUDP
#define FUNCTIONSUDP

#include "utils.h"
#include "manageUsersFolder.h"
#include "filesystem.h"

#include <iostream>
using namespace std;

string login(string userId, string password);

string logout(string userId, string password);

string unregister(string userId, string password);

string getMyAuctions(string userId);

string getMyBids(string userId);

string listAuctions();

string showRecord(string auctionId);

#endif