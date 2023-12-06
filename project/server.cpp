#include <sys/types.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <time.h>
#include <fcntl.h>
#include <sys/socket.h>
#include <sys/ioctl.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <signal.h>

#include <sstream>
#include <fstream>
#include <filesystem>
#include <vector>
#include <algorithm>

#include <iostream>
using namespace std;

string myPort = "58000";
bool verbose = false;

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

/**
 *
 * @param directoryPath O caminho para a pasta a ser criada.
 * @return Retorna 0 se a pasta for criada com sucesso, -1 em caso de erro.
 *
*/
int createFolder(filesystem::path directoryPath) {
    if (!filesystem::exists(directoryPath)) {
        if (filesystem::create_directories(directoryPath)) {
            cout << "Directory " << directoryPath << " created successfully." << endl; // Debug
        }
        else {
            cerr << "Error creating directory." << endl; // Debug
            return -1;
        }
    }
    else {
        cout << "The directory " << directoryPath << " already exists." << endl; // Debug
    }
    return 0;
}

/**
 * @param path O caminho para o ficheiro a ser criado.
 * @param content O conteúdo a ser escritop no ficheiro (opcional).
 * @return Retorna 0 se o ficheiro for criado com sucesso, -1 em caso de erro.
 *
*/
int createFile(string path, string content = "") {
    ofstream file(path);
    if (file.is_open()) {
        if (!content.empty()) {
            file << content;
        }
        file.close();
        cout << "file created in " << path << endl; // Debug
        return 0;
    }
    else {
        cerr << "Erro ao criar o ficheiro: " << path << endl; // Debug
        return -1;
    }
}

string readFromFile(filesystem::path filePath) {
    // cout << filePath << endl; // Debug
    if (!filesystem::exists(filePath)) {
        cerr << "File don't exists" << endl;
        return "-1"; // FIXME: throw std::runtime_error("Error opening file: " + filePath);
    }
    string content, line;
    ifstream file(filePath);
    if (file.is_open()) {
        while (getline(file, line)) {
            content += line;
        }
        file.close();
        return content;
    }
    else {
        cerr << "Error reading the file: " << filePath << endl; // Debug
        // return -1; // FIXME: throw std::runtime_error("Error opening file: " + filePath);
        return "-1";
    }
}

/**
 * Diretoria USERS contém toda a informação de cada utilizador
*/
void createUsersFolder() {
    filesystem::path directoryPath("USERS");
    createFolder(directoryPath);
}

/**
 * O servidor AS criar´a dentro da directoria USERS uma directoria por cada utilizador que se regista.
 * A designa¸c˜ao da directoria de utilizador coincide com o UID do utilizador em causa.
*/
void createUserFolder(string userId) {
    filesystem::path directoryPath("USERS/" + userId);
    createFolder(directoryPath);
}

/**
 * Um ﬁcheiro (uid) pass.txt que cont´em a password do utilizador.
 * Este ﬁcheiro existir´a enquanto o utilizador permanecer registado.
 */
bool createUserPasswordFile(string userId, string password) {
    filesystem::path filePath = "USERS/" + userId + "/" + userId + "_pass.txt";
    if (createFile(filePath, password) != -1) return true;
    return false;
}

/**
 * Um ﬁcheiro (uid) pass.txt que cont´em a password do utilizador.
 * Este ﬁcheiro existir´a enquanto o utilizador permanecer registado.
 */
bool deleteUserPasswordFile(string userId) { // TODO: deal with the errors
    filesystem::path filePath = "USERS/" + userId + "/" + userId + "_pass.txt";
    return filesystem::remove(filePath);
}

/**
 * Um ﬁcheiro (uid) login.txt indicando que o utilizador está em sessão.
 * Este ﬁcheiro existe apenas durante a sess˜ao do utilizador
*/
void createUserLoginFile(string userId) {
    filesystem::path filePath = "USERS/" + userId + "/" + userId + "_login.txt";    createFile(filePath);
}

/**
 *
 * Um ﬁcheiro (uid) login.txt indicando que o utilizador está em sessão.
 * Este ﬁcheiro existe apenas durante a sess˜ao do utilizador
*/
bool deleteUserLoginFile(string userId) { // TODO: deal with the errors
    filesystem::path filePath = "USERS/" + userId + "/" + userId + "_login.txt";
    if (filesystem::remove(filePath) != -1)
        return true;
    else return false;
}

/**
 * Uma directoria designada HOSTED contendo informa¸c˜ao sobre todos os leil˜oes iniciados pelo utilizador.
 * A cada leil˜ao iniciado pelo utilizador corresponde um ficheiro dentro da directoria HOSTED.
*/
void createHostedFolder(string userId) {
    filesystem::path directoryPath("USERS/" + userId + "/HOSTED/");
    createFolder(directoryPath);
}

/**
 * Uma directoria designada BIDDED contendo informa¸c˜ao sobre todos os leil˜oes nos quais o utilizador licitou. A cada leil˜ao no qual o
 * utilizador licitou, corresponde um ﬁcheiro dentro da directoria BIDDED
*/
void createBiddedFolder(string userId) {
    filesystem::path directoryPath("USERS/" + userId + "/BIDDED/");
    createFolder(directoryPath);
}

// void createAuctionsFolder() {
//     filesystem::path directoryPath("AUCTIONS");
//     createFolder(directoryPath);
// }

/**
 * Cria todas as diretorias e ficheiros necessários para guardar as informações do utilizador.
*/
void createUser(string userId, string password) {
    // cout << "creating the folder struct" << endl; // Debug
    createUserFolder(userId);
    createHostedFolder(userId);
    createBiddedFolder(userId);
    createUserPasswordFile(userId, password);
    createUserLoginFile(userId);
    // cout << "User folder struct created" << endl; // Debug
}

/**
 * Checks if the userAlreadyExists
 * aka if the folder structure from that user already was created
*/
bool userAlreadyExists(string userId) {
    string directoryPath = "USERS/" + userId;
    return filesystem::exists(directoryPath);
}

/**
 * Checks if the user is registed
 * aka if has a password
*/
bool isRegistedUser(string userId) {
    filesystem::path filePath = "USERS/" + userId + "/" + userId + "_pass.txt";
    return filesystem::exists(filePath);
}

bool isCorrectPassword(string userId, string passwordToTest) {
    filesystem::path filePath = "USERS/" + userId + "/" + userId + "_pass.txt";
    string corretPassword = readFromFile(filePath);
    // cout << "corretPassword " << corretPassword << endl; // Debug
    // cout << "passwordToTest " << passwordToTest << endl; // Debug
    return corretPassword == passwordToTest ? true : false;
}

bool isLoggedIn(string userId) {
    filesystem::path filePath = "USERS/" + userId + "/" + userId + "_login.txt";
    return filesystem::exists(filePath);
}

vector<string> getSortedFilesFromDirectory(filesystem::path directoryPath) {
// vector<filesystem::directory_entry> getSortedFilesFromDirectory(filesystem::path directoryPath) {
    if (filesystem::is_directory(directoryPath)) {
        vector<filesystem::directory_entry> files;
        vector<string> filenames;

        for (const auto& entry : filesystem::directory_iterator(directoryPath)) {
            files.push_back(entry);
        }

        sort(files.begin(), files.end(), [](const filesystem::directory_entry& a, const filesystem::directory_entry& b) {
            return a.path().filename() < b.path().filename();
            });

        for (const auto& entry : files) {
            filenames.push_back(entry.path().filename());
        }

        // return files;
        return filenames;
    }
    else { // FIXME deal with the errors
        cerr << "O caminho especificado não é um diretório válido." << std::endl;
        exit(-1);
    }
}



// TODO :  se puder ser, é só checkar se a pasta HOSTED
bool hasOngoingAudictions(string userId) {
    // TODO
    return false;
}

bool isAudictionActive(string audictionId) {
    filesystem::path filePath = "AUCTIONS/" + audictionId + "/END_" + audictionId + ".txt";
    return !(filesystem::exists(filePath));
}


string getMyAudictions(string userId) {
    string status;
    if (!hasOngoingAudictions(userId)) status = "NOK";
    else if (!isLoggedIn(userId)) status = "NLG";
    else {
        status = "OK";
        filesystem::path directoryPath("USERS/" + userId + "/HOSTED/");
        vector<string> auctions = getSortedFilesFromDirectory(directoryPath);

        int nAudictions = auctions.size();
        for (int i = 0;i < nAudictions;i++) {
            status += " " + auctions[i] + " " + (isAudictionActive(auctions[i]) ? "1" : "0");
        }
    }
    return "RMA " + status;
}

string login(string userId, string password) {
    string status;
    // cout << "login do user " << userId << "com a password " << password << endl; // Debug

    // TODO: check if user and password are allowed (char number etc)

    if (isRegistedUser(userId)) {
        if (!isCorrectPassword(userId, password)) { status = "NOK"; }
        else {
            createUserLoginFile(userId);
            status = "OK";
        }
    }
    else {
        createUser(userId, password);
        status = "REG";
    }
    return "RLN " + status;
}

string logout(string userId, string password) {
    string status;

    if (isLoggedIn(userId)) {
        if (!isCorrectPassword(userId, password)) status = "NOK"; // incorrect password
        else if (deleteUserLoginFile(userId)) status = "OK"; // unregister user
        else status = "NOK";// something gone wrong deleting the file
    }
    else if (!isRegistedUser(userId)) status = "UNR"; // not registered
    else status = "NOK"; // not loggedin

    return "RLO " + status;
}

string unregister(string userId, string password) {
    string status;

    if (!isRegistedUser(userId)) status = "UNR"; // not registered
    else if (isLoggedIn(userId)) {
        if (!isCorrectPassword(userId, password)) status = "NOK";   // incorrect password
        else {
            if (deleteUserLoginFile(userId) && deleteUserPasswordFile(userId)) status = "OK";    // unregister user
            else status = "NOK"; // something gone wrong deleting the files
        }
    }
    else status = "NOK"; // not loggedin
    return "RUR " + status;
}

string getCommand(string command) {
    istringstream iss(command);
    string whichCommand;
    iss >> whichCommand;

    if (whichCommand == "LIN") {
        string user, password, status;
        iss >> user >> password;
        return login(user, password);
    }
    else if (whichCommand == "LOU") {
        string user, password, status;
        iss >> user >> password;
        return logout(user, password);
    }
    else if (whichCommand == "UNR") {
        string user, password, status;
        iss >> user >> password;
        return unregister(user, password);
    }
    else if (whichCommand == "LMA") {
        string user, status;
        iss >> user;
        return getMyAudictions(user);
    }
    return "ERR";
}

int main(int argc, char *argv[])
{
    getArgs(argc, argv);

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

    if (bind(ufd, res->ai_addr, res->ai_addrlen) == -1)
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

    if (bind(tfd, res->ai_addr, res->ai_addrlen) == -1)
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
                addrlen = sizeof(udp_useraddr);

                // FIXME: read with an while loop
                ret = recvfrom(ufd, prt_str, 80, 0, (struct sockaddr *)&udp_useraddr, &addrlen);
                if (ret > 0)
                {
                    if (strlen(prt_str) > 0)
                        prt_str[ret - 1] = 0;
                    cout << "---UDP socket: " << prt_str << endl;

                    string returnString = getCommand(prt_str);

                    errcode = getnameinfo((struct sockaddr *)&udp_useraddr, addrlen, host, sizeof host, service, sizeof service, 0);
                    if (errcode == 0 && verbose)
                        cout << "       Sent by [" << host << ":" << service << "]" << endl;

                    n = sendto(ufd, returnString.c_str(), returnString.length(), 0, (struct sockaddr *)&udp_useraddr, addrlen); // Send message to client
                    if (n == -1) /*error*/
                        exit(1);
                }
            }
            if (FD_ISSET(tfd, &testfds)) // Vê se foi pelo socket do TCP
            {
                addrlen = sizeof(tcp_useraddr);
                if ((new_tfd = accept(tfd, (struct sockaddr *)&tcp_useraddr, &addrlen)) == -1)
                {
                    exit(1);
                }
                cout << "Accepted TCP socket" << endl; // Debug

                errcode = getnameinfo((struct sockaddr *)&tcp_useraddr, addrlen, host, sizeof host, service, sizeof service, 0);
                if (errcode == 0 && verbose)
                    cout << "       Sent by [" << host << ":" << service << "]" << endl;

                FD_SET(new_tfd, &inputs); // Set TCP read channel on
            }
            if (FD_ISSET(new_tfd, &testfds)) // Depois do accept tem de voltar a entrar no select
            {
                int nWritten, nRead;
                string finalBuffer;
                while ((nRead = read(new_tfd, buffer, 128)) != 0)
                {
                    if (nRead < 0)
                        exit(1);
                    finalBuffer.append(buffer, nRead);
                }
                cout << "---TCP socket: " << finalBuffer << endl; // Debug

                nWritten = write(new_tfd, finalBuffer.c_str(), finalBuffer.length()); // Send message to client
                if (nWritten == -1) // Will always get an error when using 'nc'
                {
                    exit(1);
                }


                close(new_tfd); // Close socket
                FD_CLR(new_tfd, &inputs); // Set TCP read channel off
                cout << "TCP socket closed" << endl; // Debug
            }
        }
    }
exit_loop:;
}