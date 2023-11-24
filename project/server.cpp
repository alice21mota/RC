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
        // FIXME -> check if this is working properly
        if (new_tfd != -1) // Set TCP read channel on
        {
            cout << "new_tfd"; // Debug
            FD_SET(new_tfd, &inputs);
        }

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
                if (strcmp(in_str, "exit"))
                {
                    goto exit_loop;
                }
            }
            if (FD_ISSET(ufd, &testfds)) // Vê se foi pelo socket do UDP
            {
                addrlen = sizeof(udp_useraddr);
                ret = recvfrom(ufd, prt_str, 80, 0, (struct sockaddr *)&udp_useraddr, &addrlen);
                if (ret > 0)
                {
                    if (strlen(prt_str) > 0)
                        prt_str[ret - 1] = 0;
                    cout << "---UDP socket: " << prt_str << endl;
                    errcode = getnameinfo((struct sockaddr *)&udp_useraddr, addrlen, host, sizeof host, service, sizeof service, 0);
                    if (errcode == 0 && verbose)
                        cout << "       Sent by [" << host << ":" << service << "]" << endl;

                    n = sendto(ufd, prt_str, ret, 0, (struct sockaddr *)&udp_useraddr, addrlen);
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
                cout << "Accepted TCP socket" << endl;

                errcode = getnameinfo((struct sockaddr *)&tcp_useraddr, addrlen, host, sizeof host, service, sizeof service, 0);

                if (errcode == 0 && verbose)
                    cout << "       Sent by [" << host << ":" << service << "]" << endl;
            }
            if (FD_ISSET(new_tfd, &testfds)) // Depois do accept tem de voltar a entrar no select
            {
                // FIXME: read with an while loop
                n = read(new_tfd, buffer, 128);
                if (n == -1)
                {
                    exit(1);
                }
                cout << "---TCP socket: " << buffer << endl;
            }
        }
    }
exit_loop:;
}
