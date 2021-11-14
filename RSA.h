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

// static key_ key_cl ;
// static BIGNUM *p_cl,*q_cl,*n_cl,*fi_n_cl,*e_cl,*d_cl;
// static BIGNUM *message_enc_cl;
// static BIGNUM *message_dec_cl;

// static key_ key_sr ;
// static BIGNUM *p_sr,*q_sr,*n_sr,*fi_n_sr,*e_sr,*d_sr;
// static BIGNUM *message_enc_sr;
// static BIGNUM *message_dec_sr;

void gen_rsa_key(int taille, key_* key);
void rsa_enc(unsigned char * message,unsigned char * message_enc,key_ * key);
void rsa_dec(unsigned char * message, unsigned char * message_dec , key_ * key);

#endif