#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <stdint.h>
#include <sys/types.h> 
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include "dh.h"
#include "crypto_utils.h"

#define DESTIP "127.0.0.1"
#define DESTPORT 3999

int main() {
    int sockfd;
    struct sockaddr_in dest_addr;

    if ((sockfd = socket(AF_INET, SOCK_STREAM, 0)) == -1) {
        perror("Error during socket creation");
        exit(EXIT_FAILURE);
    }

    dest_addr.sin_family = AF_INET;
    dest_addr.sin_port = htons(DESTPORT);
    dest_addr.sin_addr.s_addr = inet_addr(DESTIP);
    bzero(&(dest_addr.sin_zero), 8);

    if (connect(sockfd, (struct sockaddr *)&dest_addr, sizeof(struct sockaddr)) == -1) {
        perror("Error during connect");
        close(sockfd);
        exit(EXIT_FAILURE);
    }

    printf("[+] Connected to server %s:%d\n", DESTIP, DESTPORT);

    /* Step 1: Receive p and g from server */
    uint64_t p, g;
    if (recv(sockfd, &p, sizeof(p), 0) <= 0) {
        perror("Failed to receive p");
        exit(EXIT_FAILURE);
    }
    if (recv(sockfd, &g, sizeof(g), 0) <= 0) {
        perror("Failed to receive g");
        exit(EXIT_FAILURE);
    }

    printf("[*] Received p = %llu, g = %llu\n", p, g);

    /* Step 2: Generate private and public key */
    uint64_t private_key = generate_private_key(p);
    uint64_t public_key = generate_public_key(g, private_key, p);

    /* Step 3: Receive server's public key */
    uint64_t server_pub_key;
    if (recv(sockfd, &server_pub_key, sizeof(server_pub_key), 0) <= 0) {
        perror("Failed to receive server's public key");
        exit(EXIT_FAILURE);
    }

    /* Step 4: Receive signature length */
    size_t siglen;
    if (recv(sockfd, &siglen, sizeof(siglen), 0) <= 0) {
        perror("Failed to receive signature length");
        exit(EXIT_FAILURE);
    }

    /* Step 5: Receive signature */
    uint8_t *signature = malloc(siglen);
    if (!signature) {
        fprintf(stderr, "Memory allocation failed for signature\n");
        exit(EXIT_FAILURE);
    }
    if (recv(sockfd, signature, siglen, 0) <= 0) {
        perror("Failed to receive signature");
        free(signature);
        exit(EXIT_FAILURE);
    }

    /* Step 6: Verify signature */
    if (verify_signature("public.pem", (uint8_t *)&server_pub_key, sizeof(server_pub_key), signature, siglen) != 0) {
        fprintf(stderr, "Signature verification failed! Possible MITM attack.\n");
        free(signature);
        close(sockfd);
        exit(EXIT_FAILURE);
    }
    printf("[*] Signature verified successfully.\n");

    free(signature);

    /* Step 7: Send public key to server */
    if (send(sockfd, &public_key, sizeof(public_key), 0) == -1) {
        perror("Failed to send public key");
        close(sockfd);
        exit(EXIT_FAILURE);
    }

    printf("[*] Sent public key: %llu\n", public_key);

    /* Step 8: Compute shared secret */
    uint64_t shared_secret = compute_shared_secret(server_pub_key, private_key, p);
    printf("[*] Shared secret: %llu\n", shared_secret);

    close(sockfd);
    return 0;
}
