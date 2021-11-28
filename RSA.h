#ifndef RSA_H
#define RSA_H


#include <openssl/rand.h>
#include <string.h>
#include <openssl/bn.h>


struct key{
    unsigned char  pub[6] ;
    unsigned char  priv[33] ;
    unsigned char  n[33];
};

typedef struct key  key_;

void gen_rsa_key(int taille, key_* key);
void rsa_enc(unsigned char * message,unsigned char * message_enc,key_ * key);
void rsa_dec(unsigned char * message, unsigned char * message_dec , key_ * key);

#endif