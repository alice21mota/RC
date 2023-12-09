
#include "validations.h"

bool isNumeric(string str) {
    return (all_of(str.begin(), str.end(), ::isdigit));
}

bool isAlphanumeric(string str) {
    return (all_of(str.begin(), str.end(), ::isalnum));
}


bool isUID(string uid){
    return (uid.size()==6 && isNumeric(uid));
}

bool isPassword(string password){
    return (password.size()== 8 && isAlphanumeric(password));
}

bool isDescription(string description){
    if (description.size() > 0 && description.size() <= 10){
        if (isAlphanumeric(description))
            return true;
        return false;
    }
    return false;
}

bool isFileName(string fileName){
    const regex pattern("^([0-9]{1,21}\\.)?[a-zA-Z0-9_-]{1,21}\\.[a-zA-Z0-9_-]{3}$");
    return regex_match(fileName, pattern);
}

bool isStartValue(string startValue){
    if (startValue.size() > 0 && startValue.size() <= 6){
        if (isNumeric(startValue))
            return true;
        return false;
    }
    return false;
}

bool isDuration(string duration){
    if (duration.size() > 0 && duration.size() <= 5){
        if (isNumeric(duration))
            return true;
        return false;
    }
    return false;
}

bool fileExists(string filePath) {
    //string filePath = "ASSETS/" + asset_fname;
    cout << "Checking for file: " << filePath << endl;
    return filesystem::exists(filePath);
}

bool isFileSizeValid(size_t fileSize){
    size_t maxSize = 10 * 1024 * 1024; //10MB in bytes
    return fileSize <= maxSize;
}