#ifndef RSA_H
#define RSA_H


#include <openssl/rand.h>
#include <string.h>
#include <openssl/bn.h>


struct key{
    BIGNUM* pub ;
    BIGNUM* priv ;
    BIGNUM* n;
};

typedef struct key  key_;

static key_ key ;
static BIGNUM *p,*q,*n,*fi_n,*e,*d;
static BIGNUM *message_enc;
static BIGNUM *message_dec;


void init_bignum();
void gen_rsa_key(int taille);
void rsa_enc(unsigned char * message);
void rsa_dec(unsigned char * message);

#endif