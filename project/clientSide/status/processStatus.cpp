#include "processStatus.h"

void loginStatus(string status){

    if (status == "RLI OK"){
        logged_in = true;
        cout << "Successful login" << endl;
    }

    else if (status == "RLI NOK"){
        cout << "Incorrect login attempt" << endl;
    }
    
    else if (status == "RLI REG"){
        logged_in = true;
        cout << "New user registered" << endl;
    }

    else cout << "Unknown response" << endl;
}

void logoutStatus(string status){
    
    if (status == "RLO OK"){
        
        logged_in = false;
        cout << "Successful logout" << endl;
    }

    else if (status == "RLO NOK"){
        cout << "User not logged in" << endl;
    }
    
    else if (status == "RLO UNR"){
        cout << "Unknown user" << endl;
    }

    else cout << "Unknown response" << endl;
}

void unregisterStatus(string status){

    //TODO ALTER THIS TO BE THE SAME AS THE OTHERS
    if (status == "RUR OK"){
    
        logged_in = false;
        cout << "Successful unregister" << endl;

    }

    else if (status == "RUR NOK"){
        cout << "Incorrect unregister attempt" << endl;
    }
    
    else if (status == "RUR UNR"){
        cout << "Unknown user" << endl;
    }

    else cout << "Unknown response" << endl;
}

void openStatus(string response){

    istringstream iss(response);
    string command, status, aid;

    if (iss >> command >> status){
        
        if (command == "ROA"){
            
            if (status == "NOK" && iss.eof())
                cout << "Auction could not be started" << endl;
        
            else if (status == "NLG" && iss.eof())
                cout << "User not logged in" << endl;

            else if (status == "OK"){
                if (iss >> aid && iss.eof()){

                    if (isValidAID(aid))
                        cout << "Successful request, AID = " << aid << "" << endl;
                    
                    else cout << "Did not receive a valid AID" << endl;
                    
                } else cout << "Unknown response" << endl;

            } else cout << "Unknown response" << endl;

        } else cout << "Unknown response" << endl;

    } else cout << "Unknown response" << endl;
}

void closeStatus(string response){
    istringstream iss(response);
    string command, status;

    if (iss >> command >> status && iss.eof()){

        if (command == "RCL"){
            
            if (status == "OK")
                cout << "Auction closed successfully" << endl;

            else if (status == "NOK")
                cout << "User or password incorrect" << endl;

            else if (status == "NLG")
                cout << "User not logged in" << endl;

            else if (status == "EAU")
                cout << "Auction " << tempAID << " does not exist" << endl;

            else if (status == "EOW")
                cout << "User " << userID << " does not own auction " << tempAID << endl;

            else if (status == "END")
                cout << "Auction " << tempAID << " has already ended" << endl;

            else cout << "Unknown response" << endl;

        } else cout << "Unknown response" << endl;
 
    } else cout << "Unknown response" << endl;
 
}

void myauctionsStatus(string response){
    istringstream iss(response);
    string command, status, aid, state;

    if (iss >> command >> status){
       
        if (command == "RMA"){

            if (iss.eof()){

                if (status == "NOK"){
                    cout << "User " << userID << " does not have ongoing auctions" << endl;
                }

                else if (status == "NLG"){
                    cout << "User not logged in" << endl;
                }

                else cout << "Unknown response" << endl;

            } else if (status == "OK"){
                        
                while (iss >> aid >> state) {

                    if (isValidAID(aid) && isValidState(state))

                        cout << "Auction ID: "  << aid << ", State: " << state << endl;

                    if (iss.eof()) break;

                }

            } else cout << "Unknown response" << endl; 
            
        } else cout << "Unknown response" << endl; 
        
    } else cout << "Unknown response" << endl;
}

void bidStatus(string response){

    istringstream iss(response);
    string command, status;
    
    if (iss >> command >> status && iss.eof()){

        if (command == "RBD"){
            
            if (status == "ACC")
                cout << "Bid accepted" << endl;

            else if (status == "NOK")
                cout << "Auction " << tempAID << " is not active" << endl;

            else if (status == "NLG")
                cout << "User not logged in" << endl;

            else if (status == "REF")
                cout << "Bid refused. Larger bid placed previously" << endl;

            else if (status == "ILG")
                cout << "Cannot bid on auction hosted by yourself" << endl;

            else cout << "Unknown response" << endl;

        } else cout << "Unknown response" << endl;
 
    } else cout << "Unknown response" << endl;
}

void mybidsStatus(string response){

    istringstream iss(response);
    string command, status, aid, state;

    if (iss >> command >> status){
       
       if (command == "RMB"){

            if (iss.eof()){

                if (status == "NOK"){
                    cout << "User " << userID << " has not placed any bids" << endl;
                }

                else if (status == "NLG"){
                    cout << "User not logged in" << endl;
                }

                else cout << "Unknown response" << endl;

            } 

            else if (status == "OK"){
                        
                while (iss >> aid >> state) {
                    if (isValidAID(aid) && isValidState(state))
                        cout << "Auction ID: "  << aid << ", State: " << state << endl;
                    if (iss.eof()) break;
                }

            } else cout << "Unknown response" << endl; 
            
        } else cout << "Unknown response" << endl;
        
    } else cout << "Unknown response" << endl;

}

void listStatus(string response){
    
    istringstream iss(response);
    string command, status, aid, state;

    if (iss >> command >> status){
        
        if (command == "RLS"){

            if (iss.eof()){

                if (status == "NOK"){
                    cout << "No auction has been started" << endl;

                } else cout << "Unknown response" << endl;
            } 

            else if (status == "OK"){
                        
                while (iss >> aid >> state) {
                    if (isValidAID(aid) && isValidState(state))
                        cout << "Auction ID: "  << aid << ", State: " << state << endl;
                    if (iss.eof()) break;
                }

            } else cout << "Unknown response" << endl; 

        } else cout << "Unknown response" << endl;
       
    } else cout << "Unknown response" << endl;

}

void show_assetStatus(string response) {
    
    istringstream iss(response);
    string command, status, fName, fSize, fData, chara;

    if (iss >> command >> status) {

        if (command == "RSA") {

            if (status == "NOK"){

                cout << "Problems receiving file" << endl;

            }


            else if (status == "OK") {

                if (iss >> fName >> fSize) {

                    if (isFileName(fName) && isValidFileSize(fSize)){

                        vector<char> fData((istreambuf_iterator<char>(iss)), {});
                        
                        istringstream dataStream;
                        
                        dataStream >> command >> status >> fName >> fSize;

                        // Read until the end of the stream
                        dataStream.seekg(response.find(fSize) + fSize.size());
                        string remainingContent((istreambuf_iterator<char>(dataStream)), {});

                        string path = fName;

                        // Open a file for writing in binary mode
                        ofstream file(path, ios::binary);

                        if (file.is_open()) {
                            
                            // Write the received data to the file
                            file.write(fData.data() + 1, fData.size() - 2);

                            // Close the file after writing
                            file.close();

                            cout << "File " << fName << " of " << fSize << " bytes stored in: " << path << endl;

                        } else cout << "Error opening file for writing." << endl;

                    } else cout << "Invalid file name or size" << endl;
                    
                } else cout << "Unknown response" << endl;

            } else cout << "Unknown response" << endl;

        } else cout << "Unknown response" << endl;

    } else cout << "Unknown response" << endl;
}

void show_recordStatus(string response){
    
    istringstream iss(response);
    string command, status, aid, state;
    string host_UID, auction_name, asset_fName, start_value, start_date, start_time, timeactive;
    string identifier, bidder_UID, bid_value, bid_date, bid_time, bid_sec_time;
    string end_date, end_time, end_sec_time;

    if (iss >> command >> status){
        
        if (command == "RRC"){

            if (iss.eof()){

                if (status == "NOK"){

                    cout << "Auction " << tempAID << " does not exist" << endl;

                } else cout << "Unknown response" << endl;
            } 

            else if (status == "OK"){
                        
                if (iss >> host_UID >> auction_name >> asset_fName >> start_value >> start_date >> start_time >> timeactive) {

                    if (isUID(host_UID) && isDescription(auction_name) && isFileName(asset_fName) && isStartValue(start_value) && 
                        isValidDate(start_date) && isValidTime(start_time) && isValidSecTime(timeactive)){

                        cout << "Auction \"" << auction_name << "\" with file \"" << asset_fName << "\" started by user " << host_UID 
                            << " in " << start_date << " at " << start_time << ". Start Value was " << start_value << " and it was/has been active for " 
                            << timeactive << " seconds" << endl;

                        if (!iss.eof()){

                            cout << "Bids Information:" << endl;

                            if (iss >> identifier && identifier == "B"){

                                while (identifier == "B"){
                                    if (iss >> bidder_UID >> bid_value >> bid_date >> bid_time >> bid_sec_time){

                                        if (isUID(bidder_UID) && isBidValue(bid_value) && isValidDate(bid_date) && isValidTime(bid_time) && isValidSecTime(timeactive)){

                                            cout << "Bidder UID: " << bidder_UID << ", Bid Value: " << bid_value << ", Bid Date and Time: " 
                                                << bid_date << " at " << bid_time << ", Time since the auction started: " << bid_sec_time << " seconds" << endl;

                                        } else cout << "Invalid Bid information received" << endl;

                                    }
                                    if (iss >> identifier && !iss.eof()) continue;
                                }

                            } else cout << "No bids were/have been made" << endl;

                            if (!iss.eof()){

                                if (identifier == "E"){

                                    if (iss >> end_date >> end_time >> end_sec_time && iss.eof()){

                                        if (isValidDate(end_date) && isValidTime(end_time) && isValidSecTime(end_sec_time)){

                                            cout << "Auction ended in " << end_date << " at " << end_time 
                                                << ", " << end_sec_time << " seconds after it started" << endl;

                                        } else cout << "Invalid End information received" << endl;

                                    } else cout << "Unknown response" << endl;

                                } else cout << "Unknown response" << endl;

                            } else if (iss.eof()) 
                                cout << endl << "Auction has not ended yet" << endl;

                        } else cout << "Auction has not ended yet, and no bids have been made" << endl;

                    } else cout << "Invalid Auction information received";
                    
                } else cout << "Unknown response" << endl; 

            } else cout << "Unknown response" << endl; 

        } else cout << "Unknown response" << endl;
       
    } else cout << "Unknown response" << endl;

}
