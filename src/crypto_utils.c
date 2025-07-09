// crypto_utils.c
#include "crypto_utils.h"
#include <openssl/pem.h>
#include <openssl/evp.h>
#include <openssl/err.h>
#include <stdio.h>

int sign_message(const char *priv_key_path, const uint8_t *msg, size_t msglen, uint8_t *sig, size_t *siglen) {
    FILE *key_file = fopen(priv_key_path, "r");
    if (!key_file) return -1;

    EVP_PKEY *pkey = PEM_read_PrivateKey(key_file, NULL, NULL, NULL);
    fclose(key_file);
    if (!pkey) return -2;

    EVP_MD_CTX *ctx = EVP_MD_CTX_new();
    int success = 0;

    if (EVP_SignInit(ctx, EVP_sha256()) &&
        EVP_SignUpdate(ctx, msg, msglen) &&
        EVP_SignFinal(ctx, sig, (unsigned int *)siglen, pkey)) {
        success = 1;
    }

    EVP_MD_CTX_free(ctx);
    EVP_PKEY_free(pkey);
    return success ? 0 : -3;
}

int verify_signature(const char *pub_key_path, const uint8_t *msg, size_t msglen, const uint8_t *sig, size_t siglen) {
    FILE *key_file = fopen(pub_key_path, "r");
    if (!key_file) return -1;

    EVP_PKEY *pkey = PEM_read_PUBKEY(key_file, NULL, NULL, NULL);
    fclose(key_file);
    if (!pkey) return -2;

    EVP_MD_CTX *ctx = EVP_MD_CTX_new();
    int verified = 0;

    if (EVP_VerifyInit(ctx, EVP_sha256()) &&
        EVP_VerifyUpdate(ctx, msg, msglen) &&
        (verified = EVP_VerifyFinal(ctx, sig, siglen, pkey)) == 1) {
        EVP_MD_CTX_free(ctx);
        EVP_PKEY_free(pkey);
        return 0; // Success
    }

    EVP_MD_CTX_free(ctx);
    EVP_PKEY_free(pkey);
    return -3;
}
