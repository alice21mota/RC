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

                    stringstream dataStream;
                    dataStream << iss.rdbuf();  // Read until the end of the stream
                    fData = dataStream.str();

                    // If a destination directory is specified, store the file there
                    if (!destinationDirectory.empty()) {
                        // Construct the full path for the file in the destination directory
                        string fullPath = destinationDirectory + "/" + fName;

                        // Open a file for writing in binary mode
                        ofstream file(fullPath, ios::binary);

                        if (file.is_open()) {
                            // Write the received data to the file
                            //cout << "CONTENT " << fData << endl;

                            file.write(fData.c_str(), fData.size());

                            // Close the file after writing
                            file.close();

                            cout << "File stored at: " << fullPath << endl;

                        } else cerr << "Error opening file for writing." << endl;

                    } else cout << "Destination directory not specified" << endl;


                    cout << "File " << fName << " of " << fSize << " bytes stored locally" << endl;


                } else cout << "Unknown response" << endl;

            } else cout << "Unknown response" << endl;

        } else cout << "Unknown response" << endl;

    } else cout << "Unknown response" << endl;
}
