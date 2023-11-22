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

#define MYPORT "58011"

int main(){
    char in_str[128];
    fd_set inputs, testfds; // fd_set -> mascara. Corresponde a descritores.
    //struct timeval timeout;
    int i, out_fds, n, errcode, ret;
    char prt_str[90];

    // socket variables
    struct addrinfo hints, *res;
    struct sockaddr_in udp_useraddr, tcp_useraddr;
    socklen_t addrlen;
    int ufd, tfd;
    char host[NI_MAXHOST], service[NI_MAXSERV];

    //UDP CLIENT SECTION
    // Preenche a estrutura com 0s e depois atribui a informação já conhecida da ligação
    memset(&hints, 0, sizeof(hints));
    hints.ai_family = AF_INET;      // IPv4
    hints.ai_socktype = SOCK_DGRAM | AI_NUMERICSERV; // UDP socket

    if ((errcode = getaddrinfo("tejo.tecnico.ulisboa.pt", "58081" MYPORT, &hints, &res)) != 0)    //TODO check if it is really localhost
        exit(1); // On error
    
    ufd = socket(res->ai_family, res->ai_socktype, res->ai_protocol);
    if (ufd == -1)
        exit(1);

    /* Envia para o `fd` (socket) a mensagem "Hello!\n" com o tamanho 7.
       Não são passadas flags (0), e é passado o endereço de destino.
       É apenas aqui criada a ligação ao servidor. */
    n = sendto(ufd, "Hello!\n", 7, 0, res->ai_addr, res->ai_addrlen);
    if (n == -1) {
        exit(1);
    }

    /* Recebe 128 Bytes do servidor e guarda-os no buffer.
       As variáveis `addr` e `addrlen` não são usadas pois não foram inicializadas. */
    addrlen = sizeof(udp_useraddr);
    n = recvfrom(ufd, in_str, 128, 0, (struct sockaddr *)&udp_useraddr, &addrlen);
    if (n == -1) {
        exit(1);
    }

    /* Imprime a mensagem "echo" e o conteúdo do buffer (ou seja, o que foi recebido
    do servidor) para o STDOUT (fd = 1) */

    cout << "echo: ";
    write(1, in_str, n);

    /* Desaloca a memória da estrutura `res` e fecha o socket */
    freeaddrinfo(res);
    close(ufd);
}