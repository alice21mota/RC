#ifndef UTILS
#define UTILS

#include <filesystem>
#include "filesystem.h"


#include <iostream>
using namespace std;

/**
 * Checks if the userAlreadyExists
 * aka if the folder structure from that user already was created
*/
bool userAlreadyExists(string userId);


/**
 * Checks if the user is registed
 * aka if has a password
*/
bool isRegistedUser(string userId);

bool isCorrectPassword(string userId, string passwordToTest);

bool isLoggedIn(string userId);

bool hasOngoingAudictions(string userId);

bool isAudictionActive(string audictionId);

bool hasOngoingBids(string userId);

bool existAuctions();

#endif