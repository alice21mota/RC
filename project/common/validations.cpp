
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
    const regex pattern("^([0-9]{1,20}\\.)?[a-zA-Z0-9_-]{1,20}\\.[a-zA-Z0-9]{3}$");
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

bool isValidState(string state){
    if (state == "0" || state == "1")
        return true;
    return false;
}

bool fileExists(string filePath) {
    return filesystem::exists(filePath);
}

bool isFileSizeValid(size_t fileSize){
    size_t maxSize = 10 * 1024 * 1024; //10MB in bytes
    return fileSize <= maxSize;
}

bool isValidFileSize(string fileSizeStr) {
    try {
        // Convert the string to an integer
        int fileSize = stoi(fileSizeStr);

        // Check if the file size is non-negative and within the allowed range
        int maxSize = 10 * 1024 * 1024; // 10MB in bytes
        return fileSize >= 0 && fileSize <= maxSize;

    } catch (const invalid_argument& e) {
        // Handle the case where the conversion fails
        cerr << "Error converting file " << endl;
        return false;

    } catch (const out_of_range& e) {
        // Handle the case where the converted value is out of the range of int
        cerr << "Value out of range for int" << endl;
        return false;
    }
}

bool isValidAID(string aid){
    return (isNumeric(aid) && aid.size() == 3);
}

bool isValidDate(string date) {
   
    // Define the regular expression for the date format "YYYY-MM-DD"
    regex dateRegex(R"(\d{4}-\d{2}-\d{2})");
    
    return regex_match(date, dateRegex);

}

bool isValidTime(string time){
    
    regex timeRegex(R"(\d{2}:\d{2}:\d{2})");

    // Use std::regex_match to check if the input string matches the pattern
    return regex_match(time, timeRegex);
}

bool isValidSecTime(string sec_time){
    return (isNumeric(sec_time) && sec_time.size() <= 5);
}

bool isBidValue(string value){
    return (isNumeric(value) && value.size() <= 6);
}