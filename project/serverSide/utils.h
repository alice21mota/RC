#ifndef UTILS
#define UTILS

#include <filesystem>
#include "filesystem.h"

#include <iostream>
using namespace std;

/**
 * Checks if the user already exists
 * by verifying if the folder structure for that user has already been created.
 */
bool userAlreadyExists(string userId);


/**
 * Checks if the user is registered
 * by verifying if they have a password file.
 */
bool isRegisteredUser(string userId);

/***
 * Checks if the provided password matches the stored password for the given user.
 */
bool isCorrectPassword(string userId, string passwordToTest);

/**
 * Checks if the user is logged in
 * by verifying if they have a login file.
 */
bool isLoggedIn(string userId);

/**
 * Checks if the user has ongoing auctions
 * by verifying if their HOSTED folder exists.
 */
bool hasOngoingAuctions(string userId);

/**
 * Checks if the user has ongoing bids
 * by verifying if their BIDDED folder exists.
 */
bool hasOngoingBids(string userId);

/**
 * Checks if the given auction is still active
 * by verifying if their END file does not exist.
 */
bool isAuctionActive(string auctionId);

/**
 * Checks if there exist some auctions
 * by verifying if the AUCTIONS folder already exists.
 */
bool existAuctions();

#endif