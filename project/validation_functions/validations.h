#ifndef VALIDATIONS_H
#define VALIDATIONS_H

#include <string>

//Checks if the string is composed only of numbers
bool isNumeric(std::string str);

//Checks if the string is composed only of numbers and chars
bool isAlphanumeric(std::string str);

//Checks if UID is in correct format
bool isUID(std::string uid);

//Checks if password is in correct format
bool isPassword(std::string password);

//Checks if description is in correct format
bool isDescription(std::string description);

//Checks if string is a valid filename
bool isFileName(std::string fileName);

//Checks if it is a valid startValue
bool isStartValue(std::string startValue);

//Checks if it is a valid duration time
bool isDuration(std::string duration);

//Checks if file exists
bool fileExists(std::string asset_fname);

//Checks if file is of valid size
bool isFileSizeValid(std::size_t fileSize);

#endif // VALIDATIONS_H