#include "sendProtocols.h"

string sendUDP(string message) {
    message = message + "\n";

    int fd, errcode;
    ssize_t n, recSize;
    socklen_t addrlen;
    char buffer[128];

    struct addrinfo hints, *res;
    struct sockaddr_in addr;

    fd = socket(AF_INET, SOCK_DGRAM, 0);
    if (fd == -1)
    {
        cerr << "Error creating socket." << endl;
        return "ERROR";
    }

    memset(&hints, 0, sizeof hints);
    hints.ai_family = AF_INET; // IPv4
    hints.ai_socktype = SOCK_DGRAM;

    errcode = getaddrinfo(ip.c_str(), port.c_str(), &hints, &res);
    if (errcode != 0)
    {
        cerr << "Error getting address information." << endl;
        close(fd);
        return "ERROR";
    }

    n = sendto(fd, message.c_str(), message.length(), 0, res->ai_addr, res->ai_addrlen);
    if (n == -1)
    {
        cerr << "Error sending message." << endl;
        close(fd);
        return "ERROR";
    }

    addrlen = sizeof(addr);
    char headerBuffer[sizeof(int)]; //Storing message size

    ssize_t headerBytes = recvfrom(fd, headerBuffer, sizeof(headerBuffer), MSG_PEEK, (struct sockaddr *)&addr, &addrlen);

    if (headerBytes == -1) {

        cerr << "Error receiving message size." << endl;
        close(fd);
        return "ERROR";
    }

    int messageSize;
    memcpy(&messageSize, headerBuffer, sizeof(int));

    char* fullMessage = new char[messageSize];
    ssize_t messageBytes = recvfrom(fd, fullMessage, messageSize, 0, (struct sockaddr *)&addr, &addrlen);

    if (messageBytes == -1) {

        cerr << "Error receiving message." << endl;
        close(fd);
        return "ERROR";

    }

    freeaddrinfo(res);
    close(fd);

    return fullMessage;
}

void sendFileChunks(int fd, string fileName) {
    size_t chunkSize = 1024;
    char buffer[chunkSize];

    // Open the file
    ifstream file(fileName, ios::binary);

    if (!file.is_open()) {
        cerr << "Error opening file." << endl;
        return;
    }

    // Send the file in chunks
    while (!file.eof()) {

        file.read(buffer, chunkSize);

        // Check if anything was read
        if (file.gcount() > 0) {
            ssize_t n = write(fd, buffer, file.gcount());

            if (n == -1) {

                cerr << "Error writing file chunk." << endl;
                return;

            }
        }
    }

    // Close the file
    file.close();
}

string sendTCP(string message, string fileName, string comm) {
    if (fileName == "") {
        message = message + "\n";
    }

    int fd, errcode;
    ssize_t n, bytesRead, matchedBytes;
    size_t fileSize;
    socklen_t addrlen;
    char buffer[128];
    char newline = '\n';
    string finalBuffer, fSize;


    struct addrinfo hints, *res;
    struct sockaddr_in addr;

    fd = socket(AF_INET, SOCK_STREAM, 0);

    if (fd == -1)
    {
        cerr << "Error creating socket." << endl;
        return "ERROR";
    }

    memset(&hints, 0, sizeof hints);
    hints.ai_family = AF_INET; // IPv4
    hints.ai_socktype = SOCK_STREAM;

    errcode = getaddrinfo(ip.c_str(), port.c_str(), &hints, &res);

    if (errcode != 0)
    {
        cerr << "Error getting address information." << endl;
        close(fd);
        return "ERROR";
    }

    n = connect(fd, res->ai_addr, res->ai_addrlen);

    if (n == -1)
    {
        cerr << "Error connecting to the server." << endl;
        close(fd);
        return "ERROR";
    }

    // Send the message
    n = write(fd, message.c_str(), message.length());
    if (n == -1) {
        cerr << "Error writing message." << endl;
        close(fd);
        return "ERROR";
    }

    if (fileName != "") {
        sendFileChunks(fd, fileName);

        // Sending a newline to indicate the end of file
        ssize_t n = write(fd, &newline, 1);
        if (n == -1) {
            cerr << "Error sending file." << endl;
            close(fd);
            return "ERROR";
        }
    }

    //If it is receiving a file, 
    if (comm == "rec") {

        n = read(fd, buffer, sizeof(buffer) - 1);


        if (n == -1) {

            cerr << "Error reading response." << endl;
            close(fd);
            return "ERROR";

        }

        if (n == 8) {
            freeaddrinfo(res);
            close(fd);
            cout << buffer << endl;
            return buffer;
        }

        else {

            bytesRead = n;
            finalBuffer.append(buffer, n);

            n = read(fd, buffer, sizeof(buffer) - 1); //In order to get all the needed fields
            finalBuffer.append(buffer, n);
            bytesRead += n;

            //pattern of the beggining of the response, up until file Size 
            regex pattern(R"(RSA OK ([A-Za-z0-9_.-]{1,20}\.[A-Za-z]{3}) (\d{1,8}))");
            smatch match; //Used to match with the patern

            if (regex_search(finalBuffer, match, pattern)) {

                if (match.size() == 3) {
                    fSize = match[2];

                    //+2 to make up for an extra two " "
                    matchedBytes = match.position(0) + match.length(0) + 2;

                    bytesRead -= matchedBytes;

                }
                else {
                    cerr << "Unexpected number of matches." << endl;
                    return "ERROR";
                }
            }
            else {
                cerr << "Failed to match pattern." << endl;
                return "ERROR";
            }

            //TODO CHECK THIS
            try {
                fileSize = stoll(fSize);
            }
            catch (const invalid_argument& e) {
                cerr << "Invalid argument: " << e.what() << endl;
            }
            catch (const out_of_range& e) {
                cerr << "Out of range: " << e.what() << endl;
            }


            while (bytesRead < fileSize) {

                n = read(fd, buffer, min(sizeof(buffer), fileSize - bytesRead));

                if (n == -1) {

                    cerr << "Error reading response." << endl;
                    close(fd);
                    return "ERROR";

                }

                finalBuffer.append(buffer, n);
                bytesRead += n;

            }

            if (n == -1) {

                cerr << "Error reading response." << endl;
                close(fd);
                return "ERROR";

            }
        }
    }
    else {

        while ((n = read(fd, buffer, sizeof(buffer) - 1)) > 0) {
            //TODO CHECK THIS 
            //buffer[n] = '\0';  // Null-terminate buffer

            finalBuffer.append(buffer, n);
        }

        if (n == -1) {

            cerr << "Error reading response." << endl;
            close(fd);
            return "ERROR";

        }
    }

    freeaddrinfo(res);
    close(fd);

    return finalBuffer;

}

