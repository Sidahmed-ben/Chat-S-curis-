
#include "clserv.h"
#include "Des.h"
#include "RSA.h"
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
#include <time.h>
#include <unistd.h>

// Possibilité d'envoyer jusuq'a 4500 caractère
#define BUF_SIZE 7000
#define CLE_PUB_SIZE 38
#define CLE_SYM_CHIFF 33
#define DEC_SIZE 6000

// Variables utilisèes pour les fonctions (premiere_connection) pour assurrer
// l'envoie de la clé au début de la conversation .
int pas_de_connexion_cl = 1;
int pas_de_connexion_sr = 1;

// tableau qui contient les valeurs possibles poue la clé de chiffrement
// symetrique en hexa.
unsigned char table_cle_char[] = {'0', '1', '2', '3', '4', '5', '6', '7',
                                  '8', '9', 'A', 'B', 'C', 'D', 'E', 'F'};

// Les deux clés symetriques du serveur et du client initialisées aléatoirement.
unsigned char Cle_symetrique_server[9];
unsigned char Cle_symetrique_client[9];

unsigned char cle_symetrique_serveur[9]; // c'est la clé utilisèe pour le
                                         // chiffrement symetrique reçue depuis
                                         // le serveur(aprés le déchiffrement).
unsigned char cle_symetrique_client[9]; // c'est la clé utilisée pour le
                                        // chiffrement symetrique reçue depuis
                                        // le client(aprés le déchiffrement) .

// Fonction qui gère les erreurs
void error(char *msg) {
  perror(msg);
  exit(0);
}

// Fonction appelèe par le client pour ètablir la connection pour la première
// fois
void *premiere_connection_cl(void *arg) {
  unsigned char chiffrement_cle_symetrique_client[33];
  unsigned char chiffrement_cle_symetrique_serveur[33];

  key_ *key_cl;
  key_cl = (key_ *)malloc(1 * sizeof(key_));
  // Génération de la clé (priv/pub) du client
  gen_rsa_key(128, key_cl);

  // Partie d'envoie de la clé public
  unsigned char *pub = key_cl->pub;
  unsigned char *priv = key_cl->priv;
  unsigned char *nn = key_cl->n;
  unsigned char ma_cle_pub[38];
  unsigned char cle_pub_serv[38];

  for (int i = 0; i < 32; i++)
    ma_cle_pub[i] = nn[i];
  for (int i = 0; i < 5; i++)
    ma_cle_pub[i + 32] = pub[i];
  n = sendto(sock, ma_cle_pub, BUF_SIZE, 0, (void *)&server_cl, length);
  if (n < 0) {
    error("Sendto");
  }

  // Partie de reception de la clé public
  n = recvfrom(sock, cle_pub_serv, CLE_PUB_SIZE, 0, (struct sockaddr *)&from_cl,
               &length);
  if (n < 0) {
    error("recvfrom");
  }
  key_ key_pub_serv;
  for (int i = 0; i < 32; i++)
    key_pub_serv.n[i] = cle_pub_serv[i];
  for (int i = 0; i < 5; i++)
    key_pub_serv.pub[i] = cle_pub_serv[i + 32];

  // Création de la clé symétrique du client
  for (int i = 0; i < 8; i++)
    Cle_symetrique_client[i] = table_cle_char[rand() % 16];
  Cle_symetrique_client[8] = '\0';

  // chiffrement de la clé symetrique du client pour l'envoyer au serveur .
  rsa_enc(Cle_symetrique_client, chiffrement_cle_symetrique_client,
          &key_pub_serv);

  // envoie de la clé symetrique chiffrèe vers le serveur
  n = sendto(sock, chiffrement_cle_symetrique_client, CLE_SYM_CHIFF, 0,
             (void *)&server_cl, length);
  if (n < 0) {
    error("Sendto");
  }

  // Reception de la clé symetrique chiffrè depuis le serveur
  n = recvfrom(sock, chiffrement_cle_symetrique_serveur, CLE_SYM_CHIFF, 0,
               (struct sockaddr *)&from_cl, &length);
  if (n < 0) {
    error("recvfrom");
  }

  // Déchiffrement de la clé symetrique reçue depuis le serveur
  rsa_dec(chiffrement_cle_symetrique_serveur, cle_symetrique_serveur, key_cl);
  printf("Clè claire reçue depuis serveur  %s\n", cle_symetrique_serveur);

  append_item(NULL, NULL, " ========== Je suis Connectè  =========");
  pas_de_connexion_cl = 0;
}

void *premiere_connection_sr(void *arg) {
  unsigned char chiffrement_cle_symetrique_client[33];
  unsigned char chiffrement_cle_symetrique_server[33];

  key_ *key_sr;
  key_sr = (key_ *)malloc(1 * sizeof(key_));

  // Génération de la clé (priv/pub) Rsa du serveur
  gen_rsa_key(128, key_sr);
  // partie envoie de la clé publique
  unsigned char *pub = key_sr->pub;
  unsigned char *priv = key_sr->priv;
  unsigned char *nn = key_sr->n;
  unsigned char pub_client[38];
  unsigned char ma_cle_pub[38];

  // partie reception de la clé Rsa publique du client
  n = recvfrom(sock, pub_client, CLE_PUB_SIZE, 0, (struct sockaddr *)&from_sr,
               &fromlen);
  if (n < 0) {
    error("recvfrom");
  }
  key_ key_pub_client;
  for (int i = 0; i < 32; i++)
    key_pub_client.n[i] = pub_client[i];

  for (int i = 0; i < 5; i++)
    key_pub_client.pub[i] = pub_client[i + 32];

  // Partie d'envoie de la clé Rsa public au client
  for (int i = 0; i < 32; i++)
    ma_cle_pub[i] = nn[i];
  for (int i = 0; i < 5; i++)
    ma_cle_pub[i + 32] = pub[i];
  n = sendto(sock, ma_cle_pub, BUF_SIZE, 0, (struct sockaddr *)&from_sr,
             length);
  if (n < 0) {
    error("Sendto");
  }

  // reception de la clè symetrique chiffrée du client
  n = recvfrom(sock, chiffrement_cle_symetrique_client, CLE_SYM_CHIFF, 0,
               (struct sockaddr *)&from_sr, &fromlen);
  if (n < 0) {
    error("recvfrom");
  }

  // Déchiffrement de la clé symetrique reçue depuis le client
  rsa_dec(chiffrement_cle_symetrique_client, cle_symetrique_client, key_sr);
  printf("Clè claire reçue depuis client  %s\n", cle_symetrique_client);

  // Création de la clé symetrique du serveur .
  srand(time(NULL));
  for (int i = 0; i < 8; i++)
    Cle_symetrique_server[i] = table_cle_char[rand() % 16];
  Cle_symetrique_server[8] = '\0';

  // chiffrement de la clé symetrique du serveur .
  rsa_enc(Cle_symetrique_server, chiffrement_cle_symetrique_server,
          &key_pub_client);

  // Envoi de la clè symetrique chiffrée du serveur au client
  n = sendto(sock, chiffrement_cle_symetrique_server, CLE_SYM_CHIFF, 0,
             (struct sockaddr *)&from_sr, length);
  if (n < 0) {
    error("Sendto");
  }

  append_item(NULL, NULL, " ========== Je suis Connectè  =========");
  pas_de_connexion_sr = 0;
}

// Fonction qui reçoit les message envoyés depuis le serveur.
void *reception_cl(void *arg) {
  while (pas_de_connexion_cl) {
    // on attend que le client fait ça premiére connexion
  }

  char buf[BUF_SIZE];
  while (1) {
    bzero(buf, BUF_SIZE);
    // Rècéption du message chiffré envoyé par le serveur
    n = recvfrom(sock, buf, BUF_SIZE, 0, (struct sockaddr *)&from_cl, &length);
    if (n < 0) {
      error("recvfrom");
    }

    unsigned char *dec;
    // Déchiffrer le message reçu depuis le serveur.
    dec = decrypte_tout((unsigned char *)buf, DEC_SIZE, cle_symetrique_serveur);

    // Ajouter le mesage déchiffré au Tchat.
    append_item(NULL, NULL, dec);
    free(dec);
    dec = NULL;
  }

  return 0;
}



// Fonction qui reçoit les messages envoyés depuis le client.
void *reception_sr(void *arg) {

  while (pas_de_connexion_sr) {
    // on attend que le serveur fait ça premiére connexion
  }

  char buf[BUF_SIZE];
  while (1) {
    bzero(buf, BUF_SIZE);
    // Rècéption du message chiffré envoyé par le client
    n = recvfrom(sock, buf, BUF_SIZE, 0, (struct sockaddr *)&from_sr, &fromlen);
    if (n < 0) {
      error("recvfrom");
    }

    unsigned char *dec;
    // Déchiffrement du message reçu depuis le client
    dec = decrypte_tout((unsigned char *)buf, DEC_SIZE, cle_symetrique_client);

    // Ajouter le message déchiffré au Tchat.
    append_item(NULL, NULL, dec);

    free(dec);
    dec = NULL;
  }

  return 0;
}



// Fonction qui envoie les messages du client
void envoi_cl(const char *chaine) {

  char buffer[BUF_SIZE];
  bzero(buffer, BUF_SIZE);
  strcpy(buffer, chaine);

  // chiffrer les message avec DES  avant l'envoie
  unsigned char *enc;
  int taille_chaine_a_enc = strlen(chaine);
  enc = enctypte_tout((unsigned char *)chaine, taille_chaine_a_enc,
                      Cle_symetrique_client);

  // Envoyer le message chiffré au serveur.
  n = sendto(sock, enc, BUF_SIZE, 0, (void *)&server_cl, length);
  if (n < 0) {
    error("Sendto");
  }

  free(enc);
  enc = NULL;
}


// Fonction qui envoie les messages du serveur
void envoi_sr(const char *chaine) {

  char buffer[BUF_SIZE];
  bzero(buffer, BUF_SIZE);
  strcpy(buffer, chaine);

  int taille_chaine_a_enc = strlen(chaine);
  unsigned char *enc;

  // Chiffrer le message avant de l'envoyer au client.
  enc = enctypte_tout((unsigned char *)chaine, taille_chaine_a_enc,
                      Cle_symetrique_server);

  // Envoyer le message chiffré au client
  n = sendto(sock, enc, BUF_SIZE, 0, (struct sockaddr *)&from_sr, length);
  if (n < 0) {
    error("Sendto");
  }

  free(enc);
  enc = NULL;
}
