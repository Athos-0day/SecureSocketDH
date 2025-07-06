#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h> 
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>  

#define DESTIP "127.0.0.1"
#define DESTPORT 3999

int main() {
    int sockfd;
    struct sockaddr_in dest_addr;

    if ((sockfd = socket(AF_INET, SOCK_STREAM, 0)) == -1) {
        perror("Error during socket creation");
        exit(EXIT_FAILURE);
    }

    /* Initialization of server_addr. */
    dest_addr.sin_family = AF_INET;
    dest_addr.sin_port = htons(DESTPORT);
    dest_addr.sin_addr.s_addr = inet_addr(DESTIP);
    bzero(&(dest_addr.sin_zero), 8);

    /* Creation of the connection. */
    if (connect(sockfd, (struct sockaddr *)&dest_addr, sizeof(struct sockaddr)) == -1) {
        perror("Error during bind");
        close(sockfd);
        exit(EXIT_FAILURE);
    }

    printf("[+] Connected to server %s:%d\n", DESTIP, DESTPORT);

    /* Now we send a message to the server. */
    char *msg = "Hello server!";
    int len, bytes_sent;

    len = strlen(msg);

    if ((bytes_sent = send(sockfd, msg, len, 0)) == -1) {
        perror("Error during send");
        close(sockfd);
        exit(EXIT_FAILURE);
    }
}