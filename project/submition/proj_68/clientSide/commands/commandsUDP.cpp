#include "commandsUDP.h"

string login(string command){

    // login UID password
    // UID = 6 numeros
    // password = 8 alphabumeric numbers

    string whichCommand;
    istringstream iss(command);

    if (logged_in == false){

        if (iss >> whichCommand >> userID >> password && iss.eof()) {
            
            if (isUID(userID) && isPassword(password)){
                return "LIN " + userID + " " + password;
            }

            else return "Incorrect login attempt";               

        } else return "Incorrect login attempt";

    } else return "A user is already logged in";

}

string logout(){

    if (logged_in == true){
        return "LOU " + userID + " " + password;
    }

    else return "User not logged in";
    
}

string unregister(){

    if (logged_in == true){

        if (isUID(userID) && isPassword(password)){

            return "UNR " + userID + " " + password;
            
        } else return "User not known";    

    } else return "User not logged in";

}

string myauctions(){

    if (logged_in == true ){
        
        if (isUID(userID) && isPassword(password)){

            return "LMA " + userID;

        } else return "User not known"; 

    } else return "User not logged in";

}

string mybids(){
    
    if (logged_in == true ){
        
        if (isUID(userID) && isPassword(password)){
            
            return "LMB " + userID;

        } else return "User not known"; 

    } else return "User not logged in";
}

string list(string command){
    
    string whichCommand;
    istringstream iss(command);

    if (iss >> whichCommand && iss.eof()) {

        return "LST";     

    } else return "Incorrect command format";
}

string show_record(string command){
    
    string whichCommand, aid;
    istringstream iss(command);

    if (iss >> whichCommand >> aid && iss.eof()){

        if (isValidAID(aid)){
            
            tempAID = aid;

            return "SRC " + aid;
        
        } else return "Invalid AID";

    } else return "Invalid command";
    
}