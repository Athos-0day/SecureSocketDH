#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h> 
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include "dh.h"  
#include "crypto_utils.h"

#define MYPORT 3999 /* Connection port for clients. */

int main() {
    int server_fd, client_fd;
    struct sockaddr_in server_addr;
    struct sockaddr_in client_addr;
    socklen_t sin_size = sizeof(struct sockaddr_in);

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

    // --- Sending p and g to the client ---
    // Assign the predefined Diffie-Hellman parameters to local variables
    uint64_t p = DH_P;
    uint64_t g = DH_G;

    // Send the prime number p to the client
    if (send(client_fd, &p, sizeof(p), 0) == -1) {
        perror("Error sending p");
        close(client_fd);
        close(server_fd);
        exit(EXIT_FAILURE);
    }

    // Send the generator g to the client
    if (send(client_fd, &g, sizeof(g), 0) == -1) {
        perror("Error sending g");
        close(client_fd);
        close(server_fd);
        exit(EXIT_FAILURE);
    }

    // Print confirmation that p and g were sent successfully
    printf("[*] Sent p = %llu and g = %llu to client\n", p, g);

    // Generate private and public key
    uint64_t private_key = generate_private_key(p);
    uint64_t public_key = generate_public_key(g, private_key, p);

    // --- Sign public key ---
    uint8_t signature[256];
    size_t siglen;

    if (sign_message("private.pem", (uint8_t *)&public_key, sizeof(public_key), signature, &siglen) != 0) {
        fprintf(stderr, "Error signing public key.\n");
        close(client_fd);
        close(server_fd);
        exit(EXIT_FAILURE);
    }

    // --- Send public key ---
    if (send(client_fd, &public_key, sizeof(public_key), 0) == -1) {
        perror("Error sending public key");
        close(client_fd);
        close(server_fd);
        exit(EXIT_FAILURE);
    }

    // --- Send signature length ---
    if (send(client_fd, &siglen, sizeof(siglen), 0) == -1) {
        perror("Error sending signature length");
        close(client_fd);
        close(server_fd);
        exit(EXIT_FAILURE);
    }

    // --- Send signature ---
    if (send(client_fd, signature, siglen, 0) == -1) {
        perror("Error sending signature");
        close(client_fd);
        close(server_fd);
        exit(EXIT_FAILURE);
    }

    printf("[*] Sent signed public key (%llu) with signature (%zu bytes)\n", public_key, siglen);

    //Receive the public key from the client 
    uint64_t client_pub_key;
    if (recv(client_fd, &client_pub_key, sizeof(client_pub_key), 0) <= 0) {
        perror("Failed to receive client's public key");
        exit(EXIT_FAILURE);
    }

    printf("[*] Received client's public key: %llu\n", client_pub_key);

    //Compute shared secret
    uint64_t shared_secret = compute_shared_secret(client_pub_key, private_key, p);
    printf("[*] Shared secret: %llu\n", shared_secret);

    close(client_fd);
    close(server_fd);
    return 0;
}