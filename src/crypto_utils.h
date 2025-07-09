// crypto_utils.h
#ifndef CRYPTO_UTILS_H
#define CRYPTO_UTILS_H

#include <stdint.h>
#include <stddef.h>

/**
 * @brief Signs a message using an RSA private key.
 *
 * This function signs the given message using the RSA private key stored
 * in the specified PEM file. The result is stored in the provided signature buffer.
 *
 * @param priv_key_path Path to the PEM file containing the RSA private key.
 * @param msg Pointer to the message to be signed.
 * @param msglen Length of the message in bytes.
 * @param sig Output buffer where the signature will be stored.
 * @param siglen Pointer to a variable that will contain the signature length.
 * @return 0 on success, or a negative error code on failure.
 */
int sign_message(const char *priv_key_path, const uint8_t *msg, size_t msglen, uint8_t *sig, size_t *siglen);

/**
 * @brief Verifies a message's signature using an RSA public key.
 *
 * This function checks whether the provided signature is valid for the given message,
 * using the RSA public key stored in the specified PEM file.
 *
 * @param pub_key_path Path to the PEM file containing the RSA public key.
 * @param msg Pointer to the original message.
 * @param msglen Length of the message in bytes.
 * @param sig Pointer to the signature to verify.
 * @param siglen Length of the signature in bytes.
 * @return 0 if the signature is valid, or a negative error code if invalid or an error occurred.
 */
int verify_signature(const char *pub_key_path, const uint8_t *msg, size_t msglen, const uint8_t *sig, size_t siglen);

#endif // CRYPTO_UTILS_H
