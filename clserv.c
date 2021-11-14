
#include "clserv.h"
#include "graphique.h"
#include <arpa/inet.h>
#include <netdb.h>
#include <netinet/in.h>
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>


#define BUF_SIZE 1024
#define CLE_PUB_SIZE 38

int pas_de_connexion_cl = 1;
int pas_de_connexion_sr = 1;

key_ * key_cl  ;
unsigned char message_enc_cl[33];
unsigned char message_dec_cl[9];


key_ * key_sr;
unsigned char message_enc_sr[33];
unsigned char message_dec_sr[9];


// Fonction qui gère les erreurs 
void error(char *msg) {
  perror(msg);
  exit(0);
}

// Fonction appelèe par le client pour ètablir la connection pour la première fois 
void *premiere_connection_cl(void *arg) {
  //char conn_msg[BUF_SIZE] = " Pour ètablir la connection ";

  key_cl = (key_ *)malloc(1*sizeof(key_));
  gen_rsa_key(128,key_cl);

  unsigned char * pub  = key_cl->pub;
  unsigned char * priv = key_cl-> priv;
  unsigned char * nn   = key_cl->n; 

  unsigned char ma_cle_pub[38];
  unsigned char cle_pub_serv[38];

  for(int i = 0; i< 32;i++)
    ma_cle_pub[i] = nn[i];

  for(int i = 0; i< 5;i++)
    ma_cle_pub[i+32] = pub[i];

  n = sendto(sock, ma_cle_pub, BUF_SIZE, 0, (void *)&server_cl, length);
  if (n < 0) {
    error("Sendto");
  }

  fprintf(stderr," je suis dans client ma clé public   ");
  for(int i = 0 ; i< 38;i++){
    fprintf(stderr,"%c",ma_cle_pub[i]);
    if(i==31)
      fprintf(stderr,"/");
  }
  fprintf(stderr,"\n");

  
  n = recvfrom(sock, cle_pub_serv, CLE_PUB_SIZE, 0, (struct sockaddr *)&from_cl, &length);
  if (n < 0) {
    error("recvfrom");
  }
  

  fprintf(stderr," je suis dans client la clé public du serveur   ");
  for(int i = 0 ; i< 38;i++){
    fprintf(stderr,"%c",cle_pub_serv[i]);
    if(i==31)
      fprintf(stderr,"/");
  }
  fprintf(stderr,"\n");



  append_item(NULL, NULL, " ========== Je suis Connectè  =========");
  pas_de_connexion_cl = 0;
}

// Fonction appelèe par le serveur  pour ètablir la connection pour la première fois 
void *premiere_connection_sr(void *arg) {
  //int conn_msg[BUF_SIZE];
  unsigned char pub_client[38];
  unsigned char ma_cle_pub[38];

  key_sr = (key_ *)malloc(1*sizeof(key_));
  gen_rsa_key(128,key_sr);

  n = recvfrom(sock, pub_client, CLE_PUB_SIZE, 0, (struct sockaddr *)&from_sr,
               &fromlen);
  if (n < 0) {
    error("recvfrom");
  }

  fprintf(stderr," je suis dans server la clé public du client =   ");
  for(int i = 0 ; i< 38;i++){
    fprintf(stderr,"%c",pub_client[i]);
    if(i == 31)
      fprintf(stderr,"/");
  }
  fprintf(stderr,"\n");

  
  unsigned char * pub  = key_sr->pub;
  unsigned char * priv = key_sr-> priv;
  unsigned char * nn   = key_sr->n; 


  for(int i = 0; i< 32;i++)
    ma_cle_pub[i] = nn[i];

  for(int i = 0; i< 5;i++)
    ma_cle_pub[i+32] = pub[i];

  

  n = sendto(sock, ma_cle_pub, BUF_SIZE, 0, (struct sockaddr *)&from_sr,
             length);
  if (n < 0) {
    error("Sendto");
  }

  fprintf(stderr," je suis dans serveur  ma  clé public   ");
  for(int i = 0 ; i< 38;i++){
    fprintf(stderr,"%c",ma_cle_pub[i]);
    if(i==31)
      fprintf(stderr,"/");
  }
  fprintf(stderr,"\n");




  append_item(NULL, NULL, " ========== Je suis Connectè  =========");
  pas_de_connexion_sr = 0;
  // sleep(10);
}


// Fonction qui reçoit les message du client 
void *reception_cl(void *arg) {
  while(pas_de_connexion_cl){
    // on attend que le client fait ça premiére connexion
  }
  char buf[BUF_SIZE];
  while (1) {
    bzero(buf, BUF_SIZE);
    n = recvfrom(sock, buf, BUF_SIZE, 0, (struct sockaddr *)&from_cl, &length);
    if (n < 0) {
      error("recvfrom");
    }

    if (buf[0] == '0') {
      if (strcmp(buf + 1, "stop") == 0) {
        append_item(NULL, NULL, " Stop reçu  <connexion fermè> ");
        break;
      }
      append_item(NULL, NULL, buf + 1);
      bzero(buf, BUF_SIZE);
      buf[0] = '1';
      strcpy(buf + 1, "---------------------------------------- Message "
                      "systeme :< J'AI BIEN REÇUE TON MESSAGE >");
      n = sendto(sock, buf, BUF_SIZE, 0, (struct sockaddr *)&server_cl, length);
      if (n < 0) {
        error("sendto");
      }
    } else {
      append_item(NULL, NULL, buf + 1);
    }
  }

  return 0;
}

// Fonction qui reçoit les messages du serveur 
void *reception_sr(void *arg) {

  // sleep(10);
  while(pas_de_connexion_sr){
    // on attend que le servur fait ça premiére connexion
  }
  char buf[BUF_SIZE];
  while (1) {
    bzero(buf, BUF_SIZE);
    n = recvfrom(sock, buf, BUF_SIZE, 0, (struct sockaddr *)&from_sr, &fromlen);
    if (n < 0) {
      error("recvfrom");
    }
    if (buf[0] == '0') {
      if (strcmp(buf + 1, "stop") == 0) {
        append_item(NULL, NULL, " Stop reçu  <connexion fermè> ");

        break;
      }
      append_item(NULL, NULL, buf + 1);
      bzero(buf, BUF_SIZE);
      buf[0] = '1';
      strcpy(buf + 1, "---------------------------------------- Message "
                      "systeme :< J'AI BIEN REÇUE TON MESSAGE >");
      n = sendto(sock, buf, BUF_SIZE, 0, (struct sockaddr *)&from_sr, length);
      if (n < 0) {
        error("sendto");
      }
    } else {
      append_item(NULL, NULL, buf + 1);
    }
  }
  return 0;
}

// Fonction qui envoie les messages du client 
void envoi_cl(const char *chaine) {

  char buffer[BUF_SIZE];
  bzero(buffer, BUF_SIZE);
  buffer[0] = '0';
  strcpy(buffer + 1, chaine);
  n = sendto(sock, buffer, strlen(buffer), 0, (void *)&server_cl, length);
  if (n < 0) {
    error("Sendto");
  }

  if (strcmp((char *)chaine, "stop") == 0)
    mon_exit(NULL, NULL);
}

// Fonction qui envoie les messages du serveur 
void envoi_sr(const char *chaine) {

  char buffer[BUF_SIZE];
  bzero(buffer, BUF_SIZE);
  buffer[0] = '0';
  strcpy(buffer + 1, chaine);
  n = sendto(sock, buffer, strlen(buffer), 0, (struct sockaddr *)&from_sr,
             length);
  if (n < 0) {
    error("Sendto");
  }

  if (strcmp((char *)chaine, "stop") == 0)
    mon_exit(NULL, NULL);
}
