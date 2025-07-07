#ifndef DH_H
#define DH_H

#include <stdint.h>

#define DH_P  0xFFFFFFFBULL
#define DH_G  5

/**
 * @brief Compute modular exponentiation.
 *
 * Calculates (base^exp) mod mod efficiently using the
 * square-and-multiply method (also known as binary exponentiation).
 *
 * @param base The base of the exponentiation.
 * @param exp The exponent.
 * @param mod The modulus.
 * @return The result of (base^exp) mod mod.
 */
uint64_t modexp(uint64_t base, uint64_t exp, uint64_t mod);

/**
 * @brief Generate a private key for Diffie-Hellman.
 *
 * Randomly generates a private key in the range [2, p-1], where p is the prime modulus.
 *
 * @param p The prime modulus.
 * @return A random private key (secret exponent).
 */
uint64_t generate_private_key(uint64_t p);

/**
 * @brief Generate a public key from a private key.
 *
 * Computes the public key as g^private_key mod p.
 *
 * @param g The generator (base).
 * @param private_key The private key (secret exponent).
 * @param p The prime modulus.
 * @return The corresponding public key.
 */
uint64_t generate_public_key(uint64_t g, uint64_t private_key, uint64_t p);

/**
 * @brief Compute the shared secret using the received public key.
 *
 * Computes the shared secret as (received_pub^private_key mod p),
 * which is the same value the other party will compute.
 *
 * @param received_pub The public key received from the other party.
 * @param private_key Your own private key.
 * @param p The prime modulus.
 * @return The computed shared secret key.
 */
uint64_t compute_shared_secret(uint64_t received_pub, uint64_t private_key, uint64_t p);

#endif // DH_H