// #include <sys/types.h>
// #include <netinet/in.h>
// #include <arpa/inet.h>
// #include <time.h>
// #include <fcntl.h>
// #include <sys/socket.h>
// #include <sys/ioctl.h>
// #include <stdlib.h>
// #include <errno.h>
// #include <signal.h>

#include <regex>

#include "server.h"
#include <sys/wait.h>

string myPort = "58000";
bool verbose = false;

char in_str[128];
fd_set inputs, testfds; // fd_set -> mascara. Corresponde a descritores.
struct timeval timeout;
int i, out_fds, n, errcode, ret;
char prt_str[90];
char buffer[128];

// socket variables
struct addrinfo hints, *res;
struct sockaddr_in udp_useraddr, tcp_useraddr;
socklen_t addrlen;
int ufd, tfd, new_tfd = -1;
char host[NI_MAXHOST], service[NI_MAXSERV];

void getArgs(int argc, char *argv[])
{
    for (int i = 1; i < argc; i += 2)
    {
        if (!strcmp(argv[i], "-v"))
            verbose = true;
        if (!strcmp(argv[i], "-p"))
            myPort = argv[i + 1];
    }
}

string getUDPCommand(string command) {
    string response;
    istringstream iss(command);
    string whichCommand;
    iss >> whichCommand;

    checkExpiredAuctions();

    if (whichCommand == "LIN") {
        string user, password, status;
        iss >> user >> password;
        response = login(user, password);
    }
    else if (whichCommand == "LOU") {
        string user, password, status;
        iss >> user >> password;
        response = logout(user, password);
    }
    else if (whichCommand == "UNR") {
        string user, password, status;
        iss >> user >> password;
        response = unregister(user, password);
    }
    else if (whichCommand == "LMA") {
        string user, status;
        iss >> user;
        response = getMyAuctions(user);
    }
    else if (whichCommand == "LMB") {
        string user, status;
        iss >> user;
        response = getMyBids(user);
    }
    else if (whichCommand == "LST") {
        response = listAuctions();
    }
    else if (whichCommand == "SRC") {
        string auctionId;
        iss >> auctionId;
        response = showRecord(auctionId);
    }
    else response = "ERR";
    return response + "\n";
}

string getTCPCommand(string command) {
    string response;
    istringstream iss(command);
    string whichCommand;
    iss >> whichCommand;

    checkExpiredAuctions();

    if (whichCommand == "OPA") {
        string user, password, name, start_value, timeactive, Fname, Fsize, Fdata;
        iss >> user >> password >> name >> start_value >> timeactive >> Fname >> Fsize;
        cout << "command.size() = " << command.size() << "\nstoi(Fsize) = " << stoi(Fsize) << endl; // Debug
        Fdata = command.substr(command.size() - stoi(Fsize) - 1, stoi(Fsize));
        response = open(user, password, name, start_value, timeactive, Fname, Fsize, Fdata);
    }
    else if (whichCommand == "SAS") {
        string auctionId;
        iss >> auctionId;
        response = showAsset(auctionId);
    }
    else if (whichCommand == "CLS") {
        string userId, password, auctionId;
        iss >> userId >> password >> auctionId;
        response = closeAuction(userId, password, auctionId);
    }
    else if (whichCommand == "BID") {
        string userId, password, auctionId, value;
        iss >> userId >> password >> auctionId >> value;
        response = addBid(userId, password, auctionId, stoi(value));
    }
    else response = "ERR";
    return response + "\n";
}

void dealWithUDP() {
    addrlen = sizeof(udp_useraddr);
    ret = recvfrom(ufd, prt_str, 80, 0, (struct sockaddr *)&udp_useraddr, &addrlen);
    if (ret > 0)
    {
        if (strlen(prt_str) > 0)
            prt_str[ret - 1] = 0;
        cout << "---UDP socket: " << prt_str << endl;

        string returnString = getUDPCommand(prt_str);

        errcode = getnameinfo((struct sockaddr *)&udp_useraddr, addrlen, host, sizeof host, service, sizeof service, 0);
        if (errcode == 0 && verbose)
            cout << "       Sent by [" << host << ":" << service << "]" << endl;

        cout << "vou devolver: " << returnString << endl;
        n = sendto(ufd, returnString.c_str(), returnString.length(), 0, (struct sockaddr *)&udp_useraddr, addrlen); // Send message to client
        if (n == -1) /*error*/
            exit(1);
    }
}

int acceptTCP() {
    addrlen = sizeof(tcp_useraddr);
    if ((new_tfd = accept(tfd, (struct sockaddr *)&tcp_useraddr, &addrlen)) == -1)
    {
        exit(1);
    }
    cout << "Accepted TCP socket" << endl; // Debug

    errcode = getnameinfo((struct sockaddr *)&tcp_useraddr, addrlen, host, sizeof host, service, sizeof service, 0);
    if (errcode == 0 && verbose)
        cout << "       Sent by [" << host << ":" << service << "]" << endl;

    cout << "Entrei no read TCP" << endl; // Debug
    return new_tfd;
}

void dealWithTCP() {
    int nWritten, nRead;
    string finalBuffer, fSize;
    ssize_t bytesRead, matchedBytes;
    size_t fileSize;
    int len;
    bool isOpen = false;
    string returnString;
    while ((nRead = read(new_tfd, buffer, 127)) != 0)
    {
        if (nRead < 0)
            exit(1);
        finalBuffer.append(buffer, nRead);

        len = finalBuffer.length();
        cout << "len" << endl;
        cout << len << endl;
        if (len >= 3) {
            string command = finalBuffer.substr(0, 3);
            if (command == "OPA") {
                //pattern of the beggining of the response, up until file Size 
                regex pattern(R"(OPA (\d{6}) ([a-zA-Z0-9]{8}) ([a-zA-Z0-9_-]{1,10}) (\d{1,6}) (\d{1,5}) ([a-zA-Z0-9_.-]+) (\d+) )");
                smatch match; //Used to match with the patern

                if (regex_search(finalBuffer, match, pattern)) {
                    cout << "finalBuffer" << endl;
                    cout << finalBuffer << endl;
                    cout << "match.size() = " << match.size() << endl;
                    if (match.size() == 8) {
                        fSize = match[7];

                        //+2 to make up for an extra two " "
                        matchedBytes = match.position(0) + match.length(0);

                        cout << "matchedBytes" << endl;
                        cout << matchedBytes << endl;

                        bytesRead = len;
                        bytesRead -= matchedBytes;

                    }
                    else {
                        cerr << "Unexpected number of matches." << endl;
                        // return "ERROR";
                        exit(1);
                    }
                }
                else {
                    cerr << "Failed to match pattern." << endl;
                    // return "ERROR";
                    exit(1);
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
                string Ffile = finalBuffer.substr(matchedBytes, bytesRead);
                // cout << "Ffile" << endl;
                // cout << Ffile << endl;
                cout << "fileSize" << endl;
                cout << fileSize << endl;
                while (bytesRead < fileSize) {

                    n = read(new_tfd, buffer, min(sizeof(buffer) - 1, fileSize - bytesRead));

                    Ffile.append(buffer, n);
                    bytesRead += n;
                    cout << "bytesRead" << endl;
                    cout << bytesRead << endl;
                    // cout << "Ffile do while" << endl;
                    // cout << Ffile << endl;
                }

                if (n == -1) {

                    cerr << "Error reading response." << endl;
                    close(new_tfd);
                    // return "ERROR";
                    exit(1);

                }

                cout << "match: ";
                cout << match[0] << " - " << match[1] << " - " << match[2] << " - " << match[3] << " - " << match[4] << " - " << match[5] << " - " << match[6] << " - " << match[7] << endl;
                isOpen = true;
                returnString = open(match[1], match[2], match[3], match[4], match[5], match[6], match[7], Ffile) + '\n';
                //cout << "returnString " << returnString << "<-\n";
                break;
            }
        }

        if (nRead < 127 && buffer[nRead - 1] == '\n') break; // FIXME acho que não pode ser assim mas por enquanto está a funcionar :))
    }
    // cout << "---TCP socket: " << finalBuffer << endl; // Debug

    if (!isOpen) returnString = getTCPCommand(finalBuffer);

    cout << "vou devolver por TCP: ->" << returnString << "<-\n";

    nWritten = write(new_tfd, returnString.c_str(), returnString.length()); // Send message to client
    if (nWritten == -1) // Will always get an error when using 'nc'
    {
        exit(1);
    }
    close(new_tfd); // Close socket
    cout << "TCP socket closed" << endl; // Debug
}



int main(int argc, char *argv[])
{
    getArgs(argc, argv);

    // vector<string> files = getSortedFilesFromDirectory("AUCTIONS/");
    // for (int i = 0;i < files.size();i++) {
    //     cout << "aaaa ";
    //     cout << files[i] << endl;
    // }

    // UDP SERVER SECTION
    memset(&hints, 0, sizeof(hints));
    hints.ai_family = AF_INET;
    hints.ai_socktype = SOCK_DGRAM;
    hints.ai_flags = AI_PASSIVE | AI_NUMERICSERV;

    if ((errcode = getaddrinfo(NULL, myPort.c_str(), &hints, &res)) != 0)
        exit(1); // On error

    ufd = socket(res->ai_family, res->ai_socktype, res->ai_protocol);
    if (ufd == -1)
        exit(1);

    if (::bind(ufd, res->ai_addr, res->ai_addrlen) == -1)
    {
        sprintf(prt_str, "Bind error UDP server\n");
        write(1, prt_str, strlen(prt_str));
        exit(1); // On error
    }
    if (res != NULL)
        freeaddrinfo(res);

    // TCP SERVER SELECTION
    memset(&hints, 0, sizeof hints);
    hints.ai_family = AF_INET;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_flags = AI_PASSIVE | AI_NUMERICSERV;

    if ((errcode = getaddrinfo(NULL, myPort.c_str(), &hints, &res)) != 0)
        exit(1); // On error

    tfd = socket(res->ai_family, res->ai_socktype, res->ai_protocol);
    if (tfd == -1)
        exit(1);

    if (::bind(tfd, res->ai_addr, res->ai_addrlen) == -1)
    {
        sprintf(prt_str, "Bind error TCP server\n");
        write(1, prt_str, strlen(prt_str));
        exit(1); // On error
    }
    if (res != NULL)
        freeaddrinfo(res);

    // FIXME not sure how many connections we should allow
    if (listen(tfd, 5) == -1)
    {
        exit(1);
    }

    // SET CHANNELS
    FD_ZERO(&inputs);     // Clear input mask
    FD_SET(0, &inputs);   // Set standard input channel on
    FD_SET(ufd, &inputs); // Set UDP channel on
    FD_SET(tfd, &inputs); // Set TCP channel on

    // printf("Size of fd_set: %d\n",sizeof(fd_set));
    // printf("Value of FD_SETSIZE: %d\n",FD_SETSIZE);

    while (1)
    {
        testfds = inputs; // Reload mask

        // printf("testfds byte: %d\n",((char *)&testfds)[0]); // Debug

        memset((void *)&timeout, 0, sizeof(timeout)); // Meter a 0 o timeout
        timeout.tv_sec = 10;
        out_fds = select(FD_SETSIZE, &testfds, (fd_set *)NULL, (fd_set *)NULL, (struct timeval *)&timeout);
        // testfds is now '1' at the positions that were activated

        // printf("testfds byte: %d\n",((char *)&testfds)[0]); // Debug

        switch (out_fds)
        {
        case 0:
            cout << "\n ---------------Timeout event-----------------\n";
            break;
        case -1:
            perror("select");
            exit(1);
        default:
            if (FD_ISSET(0, &testfds)) // Vê se a posição 0 foi ativada. Se foi, foi por causa do teclado.
            {
                fgets(in_str, 50, stdin);
                cout << "---Input at keyboard: " << in_str << endl;
                // FIXME -> check if its easy extra points :)
                // in_str[strcspn(in_str, "\n")] = '\0';
                if (!strcmp(in_str, "exit\n"))
                {
                    goto exit_loop;
                }
            }
            if (FD_ISSET(ufd, &testfds)) // Vê se foi pelo socket do UDP
            {
                // pid_t pid = fork();
                // if (pid == 0) {
                dealWithUDP();
            //     exit(0);
            // }
            // else if (pid == -1) {
            //     cout << "error pid UDP";
            //     exit(1);
            // }
            }
            if (FD_ISSET(tfd, &testfds)) // Vê se foi pelo socket do TCP
            {
                // pid_t pid = fork();
                // if (pid == 0) {
                new_tfd = acceptTCP();
                dealWithTCP();
                // FD_SET(new_tfd, &inputs); // Set TCP read channel on
            //     exit(0);
            // }
            // else if (pid == -1) {
            //     cout << "error pid TCP";
            //     exit(1);
            // }
            }
            // else if (FD_ISSET(new_tfd, &testfds)) // Depois do accept tem de voltar a entrar no select
            // {
            //     dealWithTCP();               
            //     close(new_tfd); // Close socket
            //     cout << "TCP socket closed" << endl; // Debug
            //     FD_CLR(new_tfd, &inputs); // Set TCP read channel off
            // }
            // wait(NULL);
        }
    }
exit_loop:; // FIXME: should i force to close all the sockets here?
}