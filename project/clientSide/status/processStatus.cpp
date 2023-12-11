#include "processStatus.h"

void loginStatus(string status){

    if (status == "RLI OK"){
        logged_in = true;
        cout << "Successful login\n";
    }

    else if (status == "RLI NOK"){
        cout << "Incorrect login attempt\n";
    }
    
    else if (status == "RLI REG"){
        logged_in = true;
        cout << "New user registered\n";
    }

    else cout << "Unknown response\n";
}

void logoutStatus(string status){
    
    if (status == "RLO OK"){
        //TODO CHECK THIS
        //userID = "";
        //password = "";
        logged_in = false;
        cout << "Successful logout\n";
    }

    else if (status == "RLO NOK"){
        cout << "User not logged in\n";
    }
    
    else if (status == "RLO UNR"){
        cout << "Unknown user\n";
    }

    else cout << "Unknown response\n";
}

void unregisterStatus(string status){

    //TODO ALTER THIS TO BE THE SAME AS THE OTHERS
    if (status == "RUR OK"){
        //userID = "";
        //password = "";
        logged_in = false;
        cout << "Successful unregister\n";
    }

    else if (status == "RUR NOK"){
        cout << "Incorrect unregister attempt\n";
    }
    
    else if (status == "RUR UNR"){
        cout << "Unknown user\n";
    }

    else cout << "Unknown response\n";
}

void openStatus(string response){

    istringstream iss(response);
    string command, status, aid;

    if (iss >> command >> status){
        
        if (command == "ROA"){
            
            if (status == "NOK" && iss.eof())
                cout << "Auction could not be started\n";
        
            else if (status == "NLG" && iss.eof())
                cout << "User not logged in\n";

            else if (status == "OK"){
                if (iss >> aid && iss.eof()){

                    if (aid.size() == 3 && isNumeric(aid))
                        cout << "Successful request, AID = " << aid << "\n";
                    
                    else cout << "Did not receive a valid AID\n";
                    
                } else cout << "Unknown response\n";

            } else cout << "Unknown response\n";

        } else cout << "Unknown response\n";

    } else cout << "Unknown response\n";
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

            else cout << "Unknown response\n";

        } else cout << "Unknown response\n";
 
    } else cout << "Unknown response\n";
 
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

            else cout << "Unknown response\n";

        } else cout << "Unknown response\n";
 
    } else cout << "Unknown response\n";
}

void mybidsStatus(string response){

    istringstream iss(response);
    string command, status, aid, state;

    if (iss >> command >> status){
       
       if (command == "RMB"){

            if (iss.eof()){

                if (status == "NOK"){
                    cout << "User " << userID << " does not have ongoing bids" << endl;
                }

                else if (status == "NLG"){
                    cout << "User not logged in" << endl;
                }

                else cout << "Unknown response" << endl;

            } 

            else if (status == "OK"){
                        
                while (iss >> aid >> state) {
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
                    cout << "Auction ID: "  << aid << ", State: " << state << endl;
                    if (iss.eof()) break;
                }

            } else cout << "Unknown response" << endl; 

        } else cout << "Unknown response" << endl;
       
    } else cout << "Unknown response" << endl;

}

void show_assetStatus(string response, string destinationDirectory) {
    
    istringstream iss(response);
    string command, status, fName, fSize, fData;

    if (iss >> command >> status) {

        if (command == "RSA") {

            if (iss.eof()) {

                if (status == "NOK") {

                    cout << "Problems receiving file" << endl;

                } else cout << "Unknown response" << endl;

            } else if (status == "OK") {

                if (iss >> fName >> fSize) {
                    
                    vector<char> fData((istreambuf_iterator<char>(iss)), {});
                    
                    istringstream dataStream;
                    
                    dataStream >> command >> status >> fName >> fSize;

                     // Read until the end of the stream
                    dataStream.seekg(response.find(fSize) + fSize.size());
                    string remainingContent((istreambuf_iterator<char>(dataStream)), {});

                    // If a destination directory is specified, store the file there
                    if (!destinationDirectory.empty()) {

                        // Construct the full path for the file in the destination directory
                        string fullPath = destinationDirectory + "/" + fName;

                        // Open a file for writing in binary mode
                        ofstream file(fullPath, ios::binary);

                        if (file.is_open()) {
                            
                            // Write the received data to the file
                            file.write(fData.data() + 1, fData.size() - 2);

                            // Close the file after writing
                            file.close();

                            cout << "File " << fName << " of " << fSize << " bytes stored at: " << fullPath << endl;

                        } else cout << "Error opening file for writing." << endl;

                    } else cout << "Destination directory not specified" << endl;

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

                    //cout << host_UID << " " << auction_name << " " << asset_fName << " " << start_value << " " << start_date << " " << start_time << " " << timeactive << endl;
                    cout << "Auction \"" << auction_name << "\" with file \"" << asset_fName << "\" started by user " << host_UID 
                        << " in " << start_date << " at " << start_time << ". Start Value was " << start_value << " and it was/has been active for " 
                        << timeactive << " seconds" << endl;

                    if (!iss.eof()){

                        cout << "Bids Information:" << endl;

                        if (iss >> identifier && identifier == "B"){

                            while (identifier == "B"){
                                if (iss >> bidder_UID >> bid_value >> bid_date >> bid_time >> bid_sec_time){

                                    cout << "Bidder UID: " << bidder_UID << ", Bid Value: " << bid_value << ", Bid Date and Time: " 
                                        << bid_date << " at " << bid_time << ", Time since the auction started: " << bid_sec_time << " seconds" << endl;
                                    //cout << bidder_UID << " " << bid_value << " " << bid_date << " " << bid_time << " " << bid_sec_time << endl; 
                                }
                                if (iss >> identifier && !iss.eof()) continue;
                            }

                        } else cout << "No bids were/have been made" << endl;

                        if (!iss.eof()){

                            if (identifier == "E"){

                                if (iss >> end_date >> end_time >> end_sec_time && iss.eof()){

                                    cout << "Auction ended in " << end_date << " at " << end_time 
                                        << ", " << end_sec_time << " seconds after it started" << endl;

                                    //cout << end_date << " " << end_time << " " << end_sec_time << endl;
                                } else cout << "Unknown response" << endl;

                            } else cout << "Unknown response" << endl;

                        } else if (iss.eof()) cout << endl << "Auction has not ended yet" << endl;

                    } else cout << "Auction has not ended yet, and no bids have been made" << endl;
                    
                } else cout << "Unknown response" << endl; 

            } else cout << "Unknown response" << endl; 

        } else cout << "Unknown response" << endl;
       
    } else cout << "Unknown response" << endl;

}
