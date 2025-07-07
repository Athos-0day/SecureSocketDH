#include "dh.h"
#include <stdlib.h>
#include <time.h>
#include <unistd.h>

uint64_t modexp(uint64_t base, uint64_t exp, uint64_t mod) {
    uint64_t result = 1;
    base = base % mod;

    while (exp > 0) {
        if (exp % 2 == 1) result = (result * base) % mod;
        exp = exp >> 1;
        base = (base * base) % mod;
    }
    return result;
}

uint64_t generate_private_key(uint64_t p) {
    srand(time(NULL) ^ getpid());
    return (rand() % (p - 2)) + 2; // private key in [2, p-1]
}

uint64_t generate_public_key(uint64_t g, uint64_t private_key, uint64_t p) {
    return modexp(g, private_key, p);
}

uint64_t compute_shared_secret(uint64_t received_pub, uint64_t private_key, uint64_t p) {
    return modexp(received_pub, private_key, p);
}
