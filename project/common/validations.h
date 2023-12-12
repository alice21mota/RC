#ifndef VALIDATIONS_H
#define VALIDATIONS_H

#include <string>
#include <regex>
#include <filesystem>
#include <iostream>

using namespace std;

//Checks if the string is composed only of numbers
bool isNumeric(string str);

//Checks if the string is composed only of numbers and chars
bool isAlphanumeric(string str);

//Checks if UID is in correct format
bool isUID(string uid);

//Checks if password is in correct format
bool isPassword(string password);

//Checks if description is in correct format
bool isDescription(string description);

//Checks if string is a valid filename
bool isFileName(string fileName);

//Checks if it is a valid startValue
bool isStartValue(string startValue);

//Checks if it is a valid duration time
bool isDuration(string duration);

//Checks if file exists
bool fileExists(string asset_fname);

//Checks if file is of valid size
bool isFileSizeValid(size_t fileSize);

//Checks if state if valid
bool isValidState(string state);

//Checks if string represents a valid file Size
bool isValidFileSize(string fileSizeStr);

//Checks is string is a valid aid
bool isValidAID(string aid);

//Checks if date is in valid format
bool isValidDate(string date);

//Checks if time is in valid format
bool isValidTime(string time);

//Checks if it is valid sec_time
bool isValidSecTime(string sec_time);

//Checks if it is a valid bid value
bool isBidValue(string value);

#endif // VALIDATIONS_H