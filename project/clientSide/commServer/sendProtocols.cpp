#include "sendProtocols.h"

string sendUDP(string message) {
    message = message + "\n";

    // maybe global variables
    int fd, errcode;
    ssize_t n, recSize;
    socklen_t addrlen;
    char buffer[128];

    struct addrinfo hints, *res;
    struct sockaddr_in addr;


    fd = socket(AF_INET, SOCK_DGRAM, 0);
    if (fd == -1)
    {
        exit(1);
    }

    memset(&hints, 0, sizeof hints);
    hints.ai_family = AF_INET; // IPv4
    hints.ai_socktype = SOCK_DGRAM;

    errcode = getaddrinfo(ip.c_str(), port.c_str(), &hints, &res);
    if (errcode != 0)
    {
        exit(1);
    }

    n = sendto(fd, message.c_str(), message.length(), 0, res->ai_addr, res->ai_addrlen);
    if (n == -1)
    {
        exit(1);
    }

    /*n = recvfrom(fd, buffer, sizeof(buffer), 0, (struct sockaddr *)&addr, &addrlen);
    if (n == -1)
    {
        exit(1);
    }*/

    addrlen = sizeof(addr);
    char headerBuffer[sizeof(int)]; //Storing message size

    ssize_t headerBytes = recvfrom(fd, headerBuffer, sizeof(headerBuffer), MSG_PEEK, (struct sockaddr *)&addr, &addrlen);
    //cout << "headerBytes = " << headerBytes << endl;
    if (headerBytes == -1) {
        cerr << "Error receiving message size." << endl;
        close(fd);
        exit(1);
    }

    int messageSize;
    memcpy(&messageSize, headerBuffer, sizeof(int));

    char* fullMessage = new char[messageSize];
    ssize_t messageBytes = recvfrom(fd, fullMessage, messageSize, 0, (struct sockaddr *)&addr, &addrlen);
    if (messageBytes == -1) {
        cerr << "Error receiving message." << endl;
        //delete[] buffer;
        close(fd);
        exit(1);
    }

    //cout << "Data received from the server: " << fullMessage << endl;

    //cout << "Complete Response: " << fullResponse; // Print the complete response*/

    freeaddrinfo(res);
    close(fd);

    return fullMessage;
}

void sendFileChunks(int fd, string fileName) {
    size_t chunkSize = 80;
    char buffer[chunkSize];

    // Open the file
    ifstream file(fileName, ios::binary);
    if (!file.is_open()) {
        cerr << "Error opening file." << endl;
        exit(1);
    }

    // Send the file in chunks
    while (!file.eof()) {
        file.read(buffer, chunkSize);

        // Check if anything was read
        if (file.gcount() > 0) {
            ssize_t n = write(fd, buffer, file.gcount());
            if (n == -1) {
                exit(1);
            }
        }
    }

    // Close the file
    file.close();
}

string sendTCP(string message, string fileName) {
    cout << "GOT in TCP\n";
    if (fileName == "") {
        message = message + "\n";
    }

    int fd, errcode;
    ssize_t n;
    socklen_t addrlen;
    char buffer[128];

    struct addrinfo hints, *res;
    struct sockaddr_in addr;

    fd = socket(AF_INET, SOCK_STREAM, 0);
    if (fd == -1)
    {
        cout << "GOT in socket\n";
        exit(1);
    }


    memset(&hints, 0, sizeof hints);
    hints.ai_family = AF_INET; // IPv4
    hints.ai_socktype = SOCK_STREAM;

    errcode = getaddrinfo(ip.c_str(), port.c_str(), &hints, &res);
    if (errcode != 0)
    {
        cout << "GOT in addrinfo\n";
        exit(1);
    }

    n = connect(fd, res->ai_addr, res->ai_addrlen);
    if (n == -1)
    {
        cout << "GOT in connect\n";
        exit(1);
    }

    n = write(fd, message.c_str(), message.length());
    if (n == -1)
    {
        cout << "GOT in write\n";
        exit(1);
    }

    if (fileName != "") {

        sendFileChunks(fd, fileName);

        char newline = '\n';
        ssize_t n = write(fd, &newline, 1);
        if (n == -1) {
            cout << "GOT in send chunks write\n";
            exit(1);
        }
    }

    n = read(fd, buffer, 128);
    if (n == -1)
    {
        cout << "GOT in read\n";
        exit(1);
    }

    cout << "ALMOST OUT\n";
    freeaddrinfo(res);
    close(fd);

    return buffer;
}
