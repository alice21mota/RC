#include "commandsTCP.h"

string open(string command){
    // name asset_fname start_value timeactive
    // name = max 10 alphanumeric
    // asset_fname = name of image or document file
    // start_value = max 6 digits
    // timeactive = max 5 digits (seconds)

    string whichCommand, name, asset_fname, start_value, timeactive;
    string toReturn, fSize;
    istringstream iss(command);

    if (userID != "" && password != ""){

        if (iss >> whichCommand >> name >> asset_fname >> start_value >> timeactive && iss.eof()) {
            
            if (isDescription(name) && isFileName(asset_fname) && isStartValue(start_value) && isDuration(timeactive)){
                
                filePath = "ASSETS/" + asset_fname;

                //Check if the file exists
                if (fileExists(filePath)) {
                    
                    fName = asset_fname;
                    size_t fileSize = getFileSize(filePath);
                    if (isFileSizeValid(fileSize)){
                        
                        fSize = fileSizeString(filePath);

                        if (fSize != "Too many digits"){

                            toReturn = "OPA " + userID + " " + password + " " + name + " " + start_value + " " +
                                    timeactive + " " + fName + " " + fSize + " ";

                            return toReturn;

                        } else return "File size is too big";

                    } else return "Invalid File Size";

                } else return "File not found";

            } else return "Invalid fields";   

        } else return "Command not as expected";

    } else return "Not logged in User Application";
}

string close(string command){
    
    string whichCommand, aid;    
    istringstream iss(command);

    if ((userID != "" && password != "") || logged_in == true ){

        if (iss >> whichCommand >> aid && iss.eof()) {
            
            if (isNumeric(aid) && aid.size() == 3){

                tempAID = aid; //Used to return error to user if needed
                return "CLS " + userID + " " + password + " " + aid;

            } else return "Incorrect AID";               

        } else return "Incorrect command format";

    } else return "User not logged in User Appplication";
}

string bid(string command){

    string whichCommand, aid, value;
    istringstream iss(command);

    if ((userID != "" && password != "") || logged_in == true ){

        if (iss >> whichCommand >> aid >> value && iss.eof()) {
            
            if (isNumeric(aid) && aid.size() == 3){
                tempAID = aid;

                if (isNumeric(value)){

                    return "BID " + userID + " " + password + " " + aid + " " + value;
                
                } else return "Invalid value";

            } else return "Invalid AID";

        } else return "Incorrect command format";

    } else return "User not logged in User Appplication";
}