

#include "RSA.h"


void init_bignum(){
    message_enc = BN_new();
    message_dec = BN_new();
    fi_n = BN_new();
    p = BN_new();
    q = BN_new();
    n = BN_new();
    e = BN_new();
    d = BN_new();
}




void gen_rsa_key(int taille){
    BIGNUM * test = BN_new();
    BN_CTX *ctx;
    ctx = BN_CTX_new();
    BIGNUM * sub_1 = BN_new();

    static const char rnd_seed[] = "string to make the random number";
    static const char rnd_seed2[] =  "generator think it has entropy";

    RAND_seed(rnd_seed, sizeof rnd_seed); 
    BN_generate_prime_ex(p, taille/2, 0, NULL, NULL, NULL);

    RAND_seed(rnd_seed2, sizeof rnd_seed2); 
    BN_generate_prime_ex(q, taille/2, 0, NULL, NULL, NULL);

    // printf(" p = ");
    // BN_print_fp(stdout,p);
    // printf("\n");

    // printf(" q = ");
    // BN_print_fp(stdout,q);
    // printf("\n");

    if(BN_cmp(p, q) == 0){
        printf(" ERROR PRIME EQUAL \n");
        exit(1);
    }

    if(!BN_mul( n , q, p, ctx)){
        printf(" ERROR MULTIPLICATION \n");
        exit(1); 
    }

    // printf(" n =  ");
    // BN_print_fp(stdout,n);
    // printf("\n");

    char exp[6] = "65537";

    if(!BN_dec2bn(&e, exp)){
        printf(" ERROR BN_dec2bn (gen_rsa_key) \n");
        exit(1); 
    }


    // printf(" e = ");
    // BN_print_fp(stdout,e);
    // printf("\n");

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


    // printf(" p-1  = ");
    // BN_print_fp(stdout,p);
    // printf("\n");



    // printf(" q-1  = ");
    // BN_print_fp(stdout,q);
    // printf("\n");

    if(!BN_mul(fi_n ,p , q, ctx)){
        printf(" ERROR MULTIPLICATION \n");
        exit(1); 
    }

    // printf(" fi_n  = ");
    // BN_print_fp(stdout,fi_n);
    // printf("\n");


    if(!BN_mod_inverse(d, e, fi_n, ctx)){
        printf(" ERROR MOD_INVERSE \n");
        exit(1); 
    }

    // printf(" d  = ");
    // BN_print_fp(stdout,d);
    // printf("\n");

    // BN_mod_mul(test,d,e,fi_n,ctx);

    // printf(" test  = ");
    // BN_print_fp(stdout,test);
    // printf("\n");
    key.pub  = e;
    key.priv = d;
    key.n = n;
    
}





void rsa_enc(unsigned char * message){
    BN_CTX *ctx;
    ctx = BN_CTX_new();
    //char  * message = "12345678"; 
    // printf("%lu",strlen(message));
    if(!BN_hex2bn(&message_enc, (char*)message)){
        printf(" ERROR BN_hex2bn rsa_enc");
        exit(0);
    }

    printf("message après transformation en BIGNUM ");
    BN_print_fp(stdout, message_enc);
    printf("\n");

    if(!BN_mod_exp(message_enc, message_enc ,key.pub,key.n, ctx)){
        printf(" ERROR MULTIPLICATION  rsa_enc \n");
        exit(1); 
    }

    printf("message après chiffrement  ");
    BN_print_fp(stdout, message_enc);
    printf("\n");
}




void rsa_dec(unsigned char * message){
    BN_CTX *ctx;
    ctx = BN_CTX_new();

    if(!BN_mod_exp(message_dec, message_enc ,key.priv,key.n, ctx)){
        printf(" ERROR MULTIPLICATION rsa_dec  \n");
        exit(1); 
    }

    message = (unsigned char *)BN_bn2hex(message_dec);

    printf("message après déchiffrement  ");
    for(int i =0; i < 8;i++)
        printf("%c",message[i]);
    printf("\n");

}
