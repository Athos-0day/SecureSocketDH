#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h> 
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>  

#define MYPORT 3999 /* Connection port for clients. */

int main() {
    int server_fd, client_fd;
    struct sockaddr_in server_addr;
    struct sockaddr_in client_addr;
    int sin_size = sizeof(struct sockaddr_in);

    if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == -1) {
        perror("Error during socket creation");
        exit(EXIT_FAILURE);
    }

    /* Initialization of server_addr. */
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(MYPORT);
    server_addr.sin_addr.s_addr = INADDR_ANY;
    bzero(&(server_addr.sin_zero), 8);

    /* Creation of the association between the IP Address and the listening port. */
    if (bind(server_fd, (struct sockaddr *)&server_addr, sizeof(struct sockaddr)) == -1) {
        perror("Error during bind");
        close(server_fd);
        exit(EXIT_FAILURE);
    }

    /* Launch of the listening. */
    if (listen(server_fd, 1) == -1) {
        perror("Error during listen");
        close(server_fd);
        exit(EXIT_FAILURE);
    }

    printf("[*] Waiting for a client on port %d...\n", MYPORT);

    /* Acceptation of a connection. */
    if ((client_fd = accept(server_fd, (struct sockaddr *)&client_addr, &sin_size)) == -1) {
        perror("Error during accept");
        close(server_fd);
        exit(EXIT_FAILURE);
    }

    printf("[+] Client connected from %s:%d\n",
           inet_ntoa(client_addr.sin_addr),
           ntohs(client_addr.sin_port));


    char buffer[1024] = {0};
    int bytes_received;

    bytes_received = recv(client_fd, buffer, sizeof(buffer) - 1, 0);
    if (bytes_received > 0) {
        buffer[bytes_received] = '\0';
        printf("[CLIENT] Received message: %s\n", buffer);
    } else {
        perror("Error receiving message");
    }

    close(client_fd);
    close(server_fd);
    return 0;
}