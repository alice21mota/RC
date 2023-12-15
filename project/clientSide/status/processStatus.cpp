#include "processStatus.h"

string loginStatus(string response) {

    string evalResponse;

    if (endsWithNewLine(response)) {
        evalResponse = response.substr(0, response.size() - 1);
    }
    else return "Unknown response";

    istringstream iss(evalResponse);
    string command, status;

    if (iss >> command >> status && iss.eof()) {

        if (command == "RLI") {
            if (status == "OK") {
                logged_in = true;
                return "Successful login";
            }
            else if (status == "NOK") {
                return "Incorrect login attempt";
            }
            else if (status == "REG") {
                logged_in = true;
                return "New user registered";
            }
            else return "Unknown response";
        }
        else return "Unknown response";
    }
    return "Unknown response";
}

string logoutStatus(string response) {

    string evalResponse;

    if (endsWithNewLine(response)) {
        evalResponse = response.substr(0, response.size() - 1);
    }
    else return "Unknown response";

    istringstream iss(evalResponse);
    string command, status;

    if (iss >> command >> status && iss.eof()) {

        if (command == "RLO") {

            if (status == "OK") {

                logged_in = false;
                return "Successful logout";

            }
            else if (status == "NOK") {

                return "User not logged in";
            }
            else if (status == "UNR") {

                return "Unknown user";
            }
            else return "Unknown response";
        }
        else return "Unknown response";
    }
    return "Unknown response";
}

string unregisterStatus(string response) {

    string evalResponse;

    if (endsWithNewLine(response)) {
        evalResponse = response.substr(0, response.size() - 1);
    }
    else return "Unknown response";

    istringstream iss(evalResponse);
    string command, status;

    if (iss >> command >> status && iss.eof()) {

        if (command == "RUR") {

            if (status == "OK") {

                logged_in = false;
                return "Successful unregister";

            }
            else if (status == "NOK") {

                return "Incorrect unregister attempt";
            }
            else if (status == "UNR") {

                return "Unknown user";
            }
            else return "Unknown response";
        }
        else return "Unknown response";
    }
    return "Unknown response";

}

string openStatus(string response) {

    string evalResponse;

    if (endsWithNewLine(response)) {
        evalResponse = response.substr(0, response.size() - 1);
    }
    else return "Unknown response";

    istringstream iss(evalResponse);
    string command, status, aid;

    if (iss >> command >> status) {
        if (command == "ROA") {

            if (status == "NOK" && iss.eof())
                return "Auction could not be started";

            else if (status == "NLG" && iss.eof())
                return "User not logged in";

            else if (status == "OK") {

                if (iss >> aid && iss.eof()) {

                    if (isValidAID(aid))
                        return "Successful request: AID = " + aid;

                    return "Did not receive a valid AID";

                }
                return "Unknown response";

            }
            return "Unknown response";

        }
        return "Unknown response";

    }
    return "Unknown response";
}

string closeStatus(string response) {

    string evalResponse;

    if (endsWithNewLine(response)) {
        evalResponse = response.substr(0, response.size() - 1);
    }
    else return "Unknown response";

    istringstream iss(evalResponse);
    string command, status;

    if (iss >> command >> status && iss.eof()) {

        if (command == "RCL") {

            if (status == "OK")
                return "Auction closed successfully";

            else if (status == "NOK")
                return "User or password incorrect";

            else if (status == "NLG")
                return "User not logged in";

            else if (status == "EAU")
                return "Auction " + tempAID + " does not exist";

            else if (status == "EOW")
                return "User " + userID + " does not own auction " + tempAID;

            else if (status == "END")
                return "Auction " + tempAID + " has already ended";

            else return "Unknown response";

        }
        return "Unknown response";

    }
    return "Unknown response";

}

string myauctionsStatus(string response) {
    string evalResponse;

    if (endsWithNewLine(response)) {
        evalResponse = response.substr(0, response.size() - 1);
    }
    else return "Unknown response\n";

    istringstream iss(evalResponse);
    string command, status, aid, state;

    if (iss >> command >> status) {

        if (command == "RMA") {

            if (iss.eof()) {

                if (status == "NOK") {

                    return "User " + userID + " does not have ongoing auctions\n";

                }

                else if (status == "NLG") {
                    return "User not logged in\n";
                }

                return "Unknown response\n";

            }
            else if (status == "OK") {

                string toReturn;

                while (iss >> aid >> state) {

                    if (isValidAID(aid) && isValidState(state))

                        toReturn += "Auction ID: " + aid + "\t State: " + state + "\n";

                        //cout << "Auction ID: " << aid << ", State: " << state << endl;

                    else return "Invalid Response\n";

                    if (iss.eof()) return toReturn;

                }

            }
            return "Unknown response\n";

        }
        return "Unknown response\n";

    }
    return "Unknown response\n";
}

string bidStatus(string response) {

    string evalResponse;

    if (endsWithNewLine(response)) {
        evalResponse = response.substr(0, response.size() - 1);
    }
    else return "Unknown response";

    istringstream iss(evalResponse);
    string command, status;

    if (iss >> command >> status && iss.eof()) {

        if (command == "RBD") {

            if (status == "ACC")
                return "Bid accepted";

            else if (status == "NOK")
                return "Auction " + tempAID + " is not active";

            else if (status == "NLG")
                return "User not logged in";

            else if (status == "REF")
                return "Bid refused. Larger bid placed previously";

            else if (status == "ILG")
                return "Cannot bid on auction hosted by yourself";

            return "Unknown response";

        }
        return "Unknown response";

    }
    return "Unknown response";
}

string mybidsStatus(string response) {

    string evalResponse;

    if (endsWithNewLine(response)) {
        evalResponse = response.substr(0, response.size() - 1);
    }
    else return "Unknown response\n";

    istringstream iss(evalResponse);
    string command, status, aid, state;

    if (iss >> command >> status) {

        if (command == "RMB") {

            if (iss.eof()) {

                if (status == "NOK") {

                    return "User " + userID + " has not placed any bids\n";

                }

                else if (status == "NLG") {
                    return "User not logged in\n";
                }

                return "Unknown response\n";

            }

            else if (status == "OK") {

                string toReturn;

                while (iss >> aid >> state) {
                    if (isValidAID(aid) && isValidState(state))

                        toReturn += "Auction ID: " + aid + "\t State: " + state + "\n";

                    else return "Invalid Response\n";

                    if (iss.eof()) return toReturn;
                }

            }
            return "Unknown response\n";

        }
        return "Unknown response\n";

    }
    return "Unknown response\n";

}

string listStatus(string response) {

    string evalResponse;

    if (endsWithNewLine(response)) {
        evalResponse = response.substr(0, response.size() - 1);
    }
    else return "Unknown response\n";

    istringstream iss(evalResponse);
    string command, status, aid, state;

    if (iss >> command >> status) {

        if (command == "RLS") {

            if (iss.eof()) {

                if (status == "NOK") {
                    return "No auction has been started yet";

                }
                return "Unknown response";
            }

            else if (status == "OK") {

                string toReturn;

                while (iss >> aid >> state) {

                    if (isValidAID(aid) && isValidState(state))

                        toReturn += "Auction ID: " + aid + "\t State: " + state + "\n";

                        //cout << "Auction ID: " << aid << ", State: " << state << endl;

                    else return "Invalid Response\n";

                    if (iss.eof()) return toReturn;

                }

            }
            return "Unknown response";

        }
        return "Unknown response";

    }
    return "Unknown response";

}

void show_assetStatus(string response) {

    istringstream iss(response);
    string command, status, fName, fSize, fData, chara;

    if (iss >> command >> status) {

        if (command == "RSA") {

            if (status == "NOK") {

                cout << "Problems receiving file" << endl;

            }


            else if (status == "OK") {

                if (iss >> fName >> fSize) {

                    if (isFileName(fName) && isValidFileSize(fSize)) {

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

                        }
                        else cout << "Error opening file for writing." << endl;

                    }
                    else cout << "Invalid file name or size" << endl;

                }
                else cout << "Unknown response" << endl;

            }
            else cout << "Unknown response" << endl;

        }
        else cout << "Unknown response" << endl;

    }
    else cout << "Unknown response" << endl;
}

string show_recordStatus(string response) {

    string evalResponse;

    if (endsWithNewLine(response)) {
        evalResponse = response.substr(0, response.size() - 1);
    }
    else return "Unknown response\n";

    istringstream iss(evalResponse);
    string command, status, aid, state;
    string host_UID, auction_name, asset_fName, start_value, start_date, start_time, timeactive;
    string identifier, bidder_UID, bid_value, bid_date, bid_time, bid_sec_time;
    string end_date, end_time, end_sec_time;

    if (iss >> command >> status) {

        if (command == "RRC") {

            if (iss.eof()) {

                if (status == "NOK") {

                    return "Auction " + tempAID + " does not exist\n";

                }
                else return "Unknown response\n";
            }

            else if (status == "OK") {

                string toReturn;

                if (iss >> host_UID >> auction_name >> asset_fName >> start_value >> start_date >> start_time >> timeactive) {

                    if (isUID(host_UID) && isDescription(auction_name) && isFileName(asset_fName) && isStartValue(start_value) &&
                        isValidDate(start_date) && isValidTime(start_time) && isValidSecTime(timeactive)) {

                        toReturn += "Auction \"" + auction_name + "\" with file \""
                            + asset_fName + "\" started by user "
                            + host_UID + " in "
                            + start_date + " at "
                            + start_time + ". Start Value was "
                            + start_value + " and it can be/was active for "
                            + timeactive + " seconds\n\n";

                        if (!iss.eof()) {

                            toReturn += "Bids Information:\n";

                            if (iss >> identifier && identifier == "B") {
                                int nBids = 0;
                                while (identifier == "B" && nBids <= 50) {
                                    if (iss >> bidder_UID >> bid_value >> bid_date >> bid_time >> bid_sec_time) {

                                        if (isUID(bidder_UID) && isBidValue(bid_value) && isValidDate(bid_date) && isValidTime(bid_time) && isValidSecTime(timeactive)) {

                                            toReturn += "Bidder UID: " + bidder_UID
                                                + "\t Bid Value: " + bid_value
                                                + "\t Bid Date and Time: " + bid_date
                                                + " at " + bid_time
                                                + "\t Time since the auction started: " + bid_sec_time
                                                + " seconds\n";

                                            nBids++;

                                        }
                                        else return "Invalid Bid information received\n";

                                    }
                                    if (iss.eof()) break;
                                    else iss >> identifier;
                                }

                            }
                            else toReturn += "No bids were/have been made\n";

                            if (!iss.eof()) {

                                if (identifier == "E") {

                                    if (iss >> end_date >> end_time >> end_sec_time && iss.eof()) {

                                        if (isValidDate(end_date) && isValidTime(end_time) && isValidSecTime(end_sec_time)) {

                                            toReturn += "\nAuction ended in " + end_date
                                                + " at " + end_time
                                                + ", " + end_sec_time
                                                + " seconds after it started\n";

                                            return toReturn;

                                        }
                                        return "Invalid End information received\n";

                                    }
                                    return "Unknown response\n";

                                }
                                return "Unknown response\n";

                            }
                            if (iss.eof()) {

                                toReturn += "\nAuction has not ended yet\n";
                                return toReturn;
                            }

                        }
                        else {
                            toReturn += "Auction has not ended yet, and no bids have been made\n";
                            return toReturn;
                        }
                    }
                    return "Invalid Auction information received\n";

                }
                return "Unknown response\n";

            }
            return "Unknown response\n";

        }
        return "Unknown response\n";

    }
    return "Unknown response\n";

}
