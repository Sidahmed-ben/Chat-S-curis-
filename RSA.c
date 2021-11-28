

#include "RSA.h"
#include <openssl/bn.h>
#include <openssl/ossl_typ.h>



// Fonction qui permet de générer des clés Rsa de taille "taille".
void gen_rsa_key(int taille,key_ * key){
    BN_CTX *ctx;
    ctx = BN_CTX_new();
    BIGNUM * sub_1 = BN_new();
    BIGNUM *p,*q,*n,*fi_n,*e,*d;
    fi_n = BN_new();
    p = BN_new();
    q = BN_new();
    n = BN_new();
    e = BN_new();
    d = BN_new();

    static const char rnd_seed[] = "Pour que la cle soit totalement alèatoire";
    static const char rnd_seed2[] =  " le même principe que que rnd_seed2 ";

    RAND_seed(rnd_seed, sizeof rnd_seed); 
    BN_generate_prime_ex(p, taille/2, 0, NULL, NULL, NULL);

    RAND_seed(rnd_seed2, sizeof rnd_seed2); 
    BN_generate_prime_ex(q, taille/2, 0, NULL, NULL, NULL);



    if(BN_cmp(p, q) == 0){
        printf(" ERROR PRIME EQUAL \n");
        exit(1);
    }

    if(!BN_mul( n , q, p, ctx)){
        printf(" ERROR MULTIPLICATION \n");
        exit(1); 
    }

    char exp[6] = "65537";

    if(!BN_dec2bn(&e, exp)){
        printf(" ERROR BN_dec2bn (gen_rsa_key) \n");
        exit(1); 
    }


    char * num_1 = "1";
    if(!BN_dec2bn(&sub_1, num_1)){
        printf(" ERROR BN_dec2bn (gen_rsa_key) \n");
        exit(1); 
    }

    if(!BN_sub( q , q, sub_1)){
        printf(" ERROR MULTIPLICATION \n");
        exit(1); 
    }

    if(!BN_sub( p , p, sub_1)){
        printf(" ERROR SUB \n");
        exit(1); 
    }


    if(!BN_mul(fi_n ,p , q, ctx)){
        printf(" ERROR MULTIPLICATION \n");
        exit(1); 
    }


    if(!BN_mod_inverse(d, e, fi_n, ctx)){
        printf(" ERROR MOD_INVERSE \n");
        exit(1); 
    }


    unsigned char * pub,*priv,*nn;

    pub  = "10001";
    priv = ((unsigned char *)BN_bn2hex(d));
    nn   = ((unsigned char *)BN_bn2hex(n));

    for(int i = 0; i< 32 ; i++)
        key->priv[i] = priv[i];

    for(int i = 0; i< 32 ; i++)
        key->n[i] = nn[i];

    for(int i = 0; i< 5 ; i++)
        key->pub[i] = pub[i];


    free(sub_1);
    free(ctx);
    free(fi_n);
    free(p);
    free(q);
    free(n);
    free(e);
    free(d);
}



// Fonction qui permet de chiffrer un message .
void rsa_enc(unsigned char * message,unsigned char * message_enc,key_ * key){
    BN_CTX *ctx;
    ctx = BN_CTX_new();


    BIGNUM * bignum_message_enc = BN_new();
    BIGNUM * e = BN_new();
    BIGNUM * n = BN_new();


    if(!BN_hex2bn(&bignum_message_enc, (char*)message)){
        printf(" ERROR BN_hex2bn rsa_enc");
        exit(0);
    }

    if(!BN_hex2bn(&e,((char*)key->pub))){
        printf(" ERROR BN_hex2bn rsa_enc");
        exit(0);
    }

    if(!BN_hex2bn(&n,((char*)key->n))){
        printf(" ERROR BN_hex2bn rsa_enc");
        exit(0);
    }


    if(!BN_mod_exp(bignum_message_enc, bignum_message_enc ,e,n, ctx)){
        printf(" ERROR MULTIPLICATION  rsa_enc \n");
        exit(1); 
    }

    unsigned char * msg_enc_tmp = (unsigned char *)BN_bn2hex(bignum_message_enc);  

    for(int i = 0; i< 32;i++){
        message_enc[i] = msg_enc_tmp[i];
    }
    message_enc[32]= '\0';

    free(ctx);
    free(e);
    free(n);
    free(bignum_message_enc);

}



// Fonction qui permet de déchiffrer un message. 
void rsa_dec(unsigned char * message, unsigned char * message_dec , key_ * key){
    BN_CTX *ctx;
    ctx = BN_CTX_new();

    BIGNUM * bignum_message_dec = BN_new() ;
    BIGNUM * d = BN_new();
    BIGNUM * n = BN_new();


    if(!BN_hex2bn(&bignum_message_dec, (char*)message)){
        printf(" ERROR BN_hex2bn rsa_dec 1");
        exit(0);
    }

    if(!BN_hex2bn(&d,((char*)key->priv))){
        printf(" ERROR BN_hex2bn rsa_dec 2 ");
        exit(0);
    }

    if(!BN_hex2bn(&n,((char*)key->n))){
        printf(" ERROR BN_hex2bn rsa_dec 3");
        exit(0);
    }


    if(!BN_mod_exp(bignum_message_dec, bignum_message_dec ,d,n, ctx)){
        printf(" ERROR MULTIPLICATION rsa_dec  4 \n");
        exit(1); 
    }

    unsigned char * message_dec_tmp;
    message_dec_tmp = (unsigned char *)BN_bn2hex(bignum_message_dec);


    for(int i = 0; i< 8;i++){
        message_dec[i] = message_dec_tmp[i];
    }
    message_dec[8]= '\0';


    free(ctx);
    free(d);
    free(n);
    free(bignum_message_dec);
}
