#include "Des.h"


// fonction auxilliaire qui permet d'afficher une valeur en bianire. 
void printBits(size_t const size, void const *const ptr) {
  unsigned char *b = (unsigned char *)ptr;
  unsigned char byte;
  int i, j;

  for (i = 0; i < size; i++) {
    for (j = 7; j >= 0; j--) {
      byte = (b[i] >> j) & 1;
      printf("%u", byte);
    }
    printf("/");
  }
  puts("");
}


// fonctions qui permet de tranformer un bloc binair en un block string .
void AffectBits(size_t const size, void const *const ptr,
                unsigned char *mon_bloc) {
  unsigned char *b = (unsigned char *)ptr;
  unsigned char byte;
  int i, j, ind;

  for (i = 0; i < size; i++) {
    for (j = 7; j >= 0; j--) {
      byte = (b[i] >> j) & 1;
      ind = (i * 8 + (7 - j));
      if (byte == 1)
        mon_bloc[ind] = '1';
      else
        mon_bloc[ind] = '0';
    }
  }
}

// Cette fonction permet de trouver la ligne et la colonne dans la sbox à partir des 5 bits d'entrèes .
void ind_fr_bloc(unsigned char bloc, unsigned char *ligne,
                 unsigned char *colonne) {
  unsigned char cl_tmp, lg_tmp;

  lg_tmp = bloc >> 5 & 1;
  lg_tmp = lg_tmp << 1;
  *ligne |= lg_tmp;

  lg_tmp = 0;
  lg_tmp = bloc >> 0 & 1;
  lg_tmp = lg_tmp << 0;
  *ligne |= lg_tmp;

  cl_tmp = bloc >> 4 & 1;
  cl_tmp = cl_tmp << 3;
  *colonne |= cl_tmp;

  cl_tmp = 0;
  cl_tmp = bloc >> 3 & 1;
  cl_tmp = cl_tmp << 2;
  *colonne |= cl_tmp;

  cl_tmp = 0;
  cl_tmp = bloc >> 2 & 1;
  cl_tmp = cl_tmp << 1;
  *colonne |= cl_tmp;

  cl_tmp = 0;
  cl_tmp = bloc >> 1 & 1;
  cl_tmp = cl_tmp << 0;
  *colonne |= cl_tmp;
}



// cette fonction permet de faire la substitution sbox pour transformer un bloc de 48bits en un bloc 32bits
void substitution_sbox(unsigned char *bloc_64x6, unsigned char *bloc_64x4) {
  unsigned char ligne, colonne = 0;
  for (int i = 0; i < 8; i++) {
    ligne = 0;
    colonne = 0;
    ind_fr_bloc(bloc_64x6[i], &ligne, &colonne);
    bloc_64x4[i] = sbox[i][ligne][colonne];
  }
}


// cette fonction permet de transformer un string de taille 8*8 en 6*8 
void convert_48x8_64x6(unsigned char *chaine_48x8, unsigned char *bloc_64x6) {
  unsigned char bloc = {0};
  unsigned char bloc_tmp = {0};
  int shift;
  int ind;
  for (int i = 0; i < 8; i++) {
    bloc_tmp = 0;
    bloc = 0;
    for (int j = 0; j < 6; j++) {
      if (chaine_48x8[i * 6 + j] == '1') {
        ind = (i * 6 + j);
        shift = 5 - j;
        bloc_tmp = (bloc_tmp >> shift) | 1;
        bloc_tmp = bloc_tmp << shift;
        bloc |= bloc_tmp;
      } else {
        continue;
      }
    }
    bloc_64x6[i] = bloc;
  }
}


// fonction qui converte un block de 32 bits en un bloc de 48 bits
void fonc32_to_48(unsigned char *bloc32, unsigned char *bloc48) {
  unsigned char permutation_bloc = {0};
  unsigned char permutation_bloc_tmp = {0};
  int ind_tc = 0;
  int ind_shift = 0;
  for (int i = 0; i < 6; i++) {
    permutation_bloc = 0;
    permutation_bloc_tmp = 0;
    for (int j = 0; j < 8; j++) {
      ind_tc = (int)((bit32_to_48[i * 8 + j] - 1) /
                     8 /* 4 = nombre de bloc de la srource */);
      ind_shift = 7 - ((bit32_to_48[i * 8 + j] - 1) % 8);
      permutation_bloc_tmp = (bloc32[ind_tc] >> ind_shift) & 1;
      permutation_bloc_tmp = permutation_bloc_tmp << (7 - j);
      permutation_bloc |= permutation_bloc_tmp;
    }
    bloc48[i] = permutation_bloc;
  }
}



// Une foction qui crèe et retourne un bloc de taille 'size' . 
unsigned char *initialisation_bloc(int size) {
  unsigned char *perm_t_c =
      (unsigned char *)malloc(size * sizeof(unsigned char));
  assert(perm_t_c);
  return perm_t_c;
}


// Fonction qui permet de faire la permutation initialle du chiffrement DES.
void permutation_initiale(unsigned char text_claire[9],
                          unsigned char *permutation_text_claire, int taille) {
  unsigned char permutation_bloc = {0};
  unsigned char permutation_bloc_tmp = {0};
  int ind_tc = 0;
  int ind_shift = 0;

  for (int i = 0; i < taille; i++) {
    permutation_bloc = 0;
    permutation_bloc_tmp = 0;
    for (int j = 0; j < 8; j++) {
      ind_tc = (int)((permutation_table[i * 8 + j] - 1) / 8);
      ind_shift = 7 - ((permutation_table[i * 8 + j] - 1) % 8);
      permutation_bloc_tmp = (text_claire[ind_tc] >> ind_shift) & 1;
      permutation_bloc_tmp = permutation_bloc_tmp << (7 - j);
      permutation_bloc |= permutation_bloc_tmp;
    }
    permutation_text_claire[i] = permutation_bloc;
  }
}


// Fonction qui permet de faire l'inverse de la permutation initial du chiffrement DES.
void inverse_permutation(unsigned char *text_claire,
                         unsigned char *permutation_text_claire, int taille) {
  unsigned char permutation_bloc = {0};
  unsigned char permutation_bloc_tmp = {0};
  int ind_tc = 0;
  int ind_shift = 0;
  for (int i = 0; i < 8; i++) {
    permutation_bloc = 0;
    permutation_bloc_tmp = 0;
    for (int j = 0; j < 8; j++) {
      ind_tc = (int)((inv_permutation_table[i * 8 + j] - 1) / 8);
      ind_shift = 7 - ((inv_permutation_table[i * 8 + j] - 1) % 8);
      permutation_bloc_tmp = (text_claire[ind_tc] >> ind_shift) & 1;
      permutation_bloc_tmp = permutation_bloc_tmp << (7 - j);
      permutation_bloc |= permutation_bloc_tmp;
    }
    permutation_text_claire[i] = permutation_bloc;
  }
}


// Fonction qui tranforme un block de 8 mots de 4 bits en un bloc de 4 mots de 8 bits (condenser les blocs) 
void conv_64x4_to_32x4(unsigned char *bloc_64x4, unsigned char *bloc_32_4) {
  char bloc, bloc_tmp = 0;
  ;
  for (int i = 0; i < 4; i++) {
    bloc_tmp = 0;
    bloc_tmp = bloc_64x4[i * 2] << 4 | bloc_64x4[(i * 2) + 1];
    bloc_32_4[i] = bloc_tmp;
  }
}


// Fonction qui permet de faire la permutation secondaire du chiffrement DES.
void permutation_secondaire(unsigned char *bloc_32x4,
                            unsigned char *permutation_bloc_32x4) {

  unsigned char permutation_bloc = {0};
  unsigned char permutation_bloc_tmp = {0};
  int ind_tc = 0;
  int ind_shift = 0;

  for (int i = 0; i < 4; i++) {
    permutation_bloc = 0;
    permutation_bloc_tmp = 0;
    for (int j = 0; j < 8; j++) {
      ind_tc = (int)((per_sec[i * 8 + j] - 1) / 8);
      ind_shift = 7 - ((per_sec[i * 8 + j] - 1) % 8);
      permutation_bloc_tmp = (bloc_32x4[ind_tc] >> ind_shift) & 1;
      permutation_bloc_tmp = permutation_bloc_tmp << (7 - j);
      permutation_bloc |= permutation_bloc_tmp;
    }
    permutation_bloc_32x4[i] = permutation_bloc;
  }
}


// Fonction qui permet de faire le XOR binair entre des blos de taille 32 bits
void bloc32x8_xor_bloc32x8(unsigned char *result, unsigned char *left_bloc_32x8,
                           unsigned char *bloc_f_32x8) {
  for (int i = 0; i < 4; i++) {
    result[i] = left_bloc_32x8[i] ^ bloc_f_32x8[i];
  }
}


// Fonction qui permet de faire le XOR entre deux chaines de charactéres.
void xor_(unsigned char *a, unsigned char *b, unsigned char *res) {

  for (int i = 0; i < 48; i++) {
    if (a[i] == b[i]) {
      res[i] = '0';
    } else {
      res[i] = '1';
    }
  }
}


// Fonction copié sur le net qui permet de faire le shift à gauche de la clé intérmédiaire DES. 
void shift_left_once(unsigned char *key) {
  unsigned char key_chunk[29];
  for (int i = 0; i < 28; i++) {
    key_chunk[i] = key[i];
  }

  for (int i = 0; i < 27; i++) {
    key[i] = key_chunk[i + 1];
  }
  key[27] = key_chunk[0];
}


// Fonction copiée sur le net qui permet de faire deux fois le shift de la clé intémédiaire DES.
void shift_left_twice(unsigned char *key) {
  unsigned char key_chunk[29];
  for (int i = 0; i < 28; i++) {
    key_chunk[i] = key[i];
  }

  for (int i = 0; i < 26; i++) {
    key[i] = key_chunk[i + 2];
  }
  key[26] = key_chunk[0];
  key[27] = key_chunk[1];
}


// Fonction qui contient une partie copié sur le net qui permet de générer la clè de chiffrement DES 
void generation_des_cle(unsigned char *key_56x8) {
  unsigned char left[28];
  unsigned char right[28];
  unsigned char combiner[56];
  unsigned char key_56x8_string[56];

  // convertir la cle key_56x8 on string
  AffectBits(7 * sizeof(unsigned char), key_56x8, key_56x8_string);
  // initialisation de left et right de la clé
  for (int i = 0; i < 28; i++) {
    left[i] = key_56x8_string[i];
  }
  for (int i = 0; i < 28; i++) {
    right[i] = key_56x8_string[i + 28];
  }

  unsigned char combiner_pc2[48];
  for (int i = 0; i < 16; i++) {
    if (i == 0 || i == 1 || i == 8 || i == 15) {
      shift_left_once(left);
      shift_left_once(right);
    }

    else {
      shift_left_twice(left);
      shift_left_twice(right);
    }

    for (int k = 0; k < 28; k++) {
      combiner[k] = left[k];
    }
    for (int k = 28; k < 56; k++) {
      combiner[k] = right[k - 28];
    }

    for (int j = 0; j < 48; j++) {
      combiner_pc2[j] = combiner[keyp2[j] - 1];
    }

    for (int j = 0; j < 48; j++) {
      tableau_key[i][j] = combiner_pc2[j];
    }
  }
}



// Fonction qui permet d'appliquer la transformation du cotè Droit du message à chiffrer avec la clè de chiffrement.  
unsigned char *F(unsigned char *R0, unsigned char tab_key[48]) {
  unsigned char *perm_bloc_32x8 = initialisation_bloc(4);
  unsigned char bloc_48[6];
  unsigned char mon_bloc_64x6[8];
  unsigned char mon_chaine_48x8_test[48];
  unsigned char mon_bloc_64x4[8];
  unsigned char mon_bloc_32x8[4];
  unsigned char cle_xor_sor_E[48];

  /* Transformer le right de 32bits vers 48bits*/
  fonc32_to_48(R0, bloc_48);
  /*Fonction Auxilliaire : sauvegarder les 48bits dans une chaine pour faciliter la substitution*/
  AffectBits(6 * sizeof(unsigned char), bloc_48, mon_chaine_48x8_test);
  // Appliquer le xor.
  xor_(mon_chaine_48x8_test, tab_key, cle_xor_sor_E);
  /*Fonction  Auxilliaire : convertir les bloc de 48x8 vers 64x6 pour les envoyer dans la sbox*/
  convert_48x8_64x6(cle_xor_sor_E, mon_bloc_64x6);
  /* construire bloc 64x4 après le passage de la sbox*/
  substitution_sbox(mon_bloc_64x6, mon_bloc_64x4);
  /* convertir le bloc 64x4 en un bloc 32x8 pour l'envoyer  après à la permutation secondaire */
  conv_64x4_to_32x4(mon_bloc_64x4, mon_bloc_32x8);
  /* Faire la permutation secondaire */
  permutation_secondaire(mon_bloc_32x8, perm_bloc_32x8);
  return perm_bloc_32x8;
}


// Fonction qui permet de transformer le clé de 8blocs de 8bits en 7blocs de 8bits 
void key_64x8_to_56x8(unsigned char *key_64x8, unsigned char *key_56x8) {
  unsigned char permutation_bloc = {0};
  unsigned char permutation_bloc_tmp = {0};
  int ind_tc = 0;
  int ind_shift = 0;

  for (int i = 0; i < 7; i++) {
    permutation_bloc = 0;
    permutation_bloc_tmp = 0;
    for (int j = 0; j < 8; j++) {
      ind_tc = (int)((keyp1[i * 8 + j] - 1) / 8);
      ind_shift = 7 - ((keyp1[i * 8 + j] - 1) % 8);
      permutation_bloc_tmp = (key_64x8[ind_tc] >> ind_shift) & 1;
      permutation_bloc_tmp = permutation_bloc_tmp << (7 - j);
      permutation_bloc |= permutation_bloc_tmp;
    }
    key_56x8[i] = permutation_bloc;
  }
}



// Fonction qui permet de rassembler les deux sorties (gauche,droite) d'une étape de permutation en une seule sortie
unsigned char *etape_de_compression(unsigned char *perm_txt_cl,
                                    unsigned char tab_key[48]) {

  unsigned char *sortie_F = NULL;
  unsigned char new_R1[4];
  unsigned char *new_sortie = initialisation_bloc(8);

  sortie_F = F(perm_txt_cl + 4, tab_key);
  bloc32x8_xor_bloc32x8(new_R1, sortie_F, perm_txt_cl);

  free(sortie_F);
  sortie_F = NULL;

  for (int i = 0; i < 4; i++) {
    new_sortie[i] = (perm_txt_cl + 4)[i];
  }
  for (int i = 4; i < 8; i++) {
    new_sortie[i] = new_R1[i - 4];
  }
  return new_sortie;
}



// Fonction qui permet de chiffrer une chaine de 8 char.
unsigned char *des_encrypte(unsigned char *message, unsigned char *cle) {

  unsigned char *perm_txt_cl = initialisation_bloc(8);
  unsigned char key_56x8[7];
  unsigned char before_final_encrypte[8];
  unsigned char *final_encrypte = initialisation_bloc(8);

  /* Faire la permutation initialle */
  permutation_initiale(message, perm_txt_cl, 8);

  // pour transformer la cle de 8octet en une clè de 7octet
  key_64x8_to_56x8(cle, key_56x8);
  generation_des_cle(key_56x8);

  for (int i = 0; i < 16; i++)
    perm_txt_cl = etape_de_compression(perm_txt_cl, tableau_key[i]);

  for (int i = 0; i < 4; i++)
    before_final_encrypte[i] = perm_txt_cl[i + 4];

  for (int i = 0; i < 4; i++)
    before_final_encrypte[i + 4] = perm_txt_cl[i];

  inverse_permutation(before_final_encrypte, final_encrypte, 8);

  free(perm_txt_cl);
  perm_txt_cl = NULL;

  return final_encrypte;
}


// Fonction qui permet de déchiffrer une chaine de 8 char.
unsigned char *des_decrypte(unsigned char *message, unsigned char *cle) {

  unsigned char *perm_txt_en = initialisation_bloc(8);
  unsigned char before_final_encrypte[8];
  unsigned char *final_encrypte = initialisation_bloc(8);
  unsigned char key_56x8[7];

  permutation_initiale(message, perm_txt_en,
                       8); /* Faire la permutation initialle */

  // generation des clès
  key_64x8_to_56x8(cle, key_56x8);
  generation_des_cle(key_56x8);

  for (int i = 15; i >= 0; i--)
    perm_txt_en = etape_de_compression(perm_txt_en, tableau_key[i]);

  for (int i = 0; i < 4; i++)
    before_final_encrypte[i] = perm_txt_en[i + 4];

  for (int i = 0; i < 4; i++)
    before_final_encrypte[i + 4] = perm_txt_en[i];

  inverse_permutation(before_final_encrypte, final_encrypte, 8);

  free(perm_txt_en);
  perm_txt_en = NULL;

  return final_encrypte;
}


// Fonction qui permet d'organiser un message en lui rajoutant des 0 pour que le nombre de char 
// constituant le message soit un multiple de 8 pour faciliter le chiffrement.

void message_org(unsigned char *message, int taille,
                 unsigned char *message_orga) {
  int ecart = 8 - (taille % 8);
  int nouvelle_taille = taille + ecart;

  for (int i = 0; i < taille; i++) {
    message_orga[i] = message[i];
    // a regargder si en rajoute les esapces pour les dernier bits de l'ecart .
  }
  for (int i = 0; i < ecart; i++) {
    message_orga[i + taille] = 0;
  }
}


// Fonction qui renvoie la chaine chiffrée de n'importe quelle taille.
unsigned char *enctypte_tout(unsigned char *tout_msg, int taille,
                             unsigned char *cle) {

  int ecart = 8 - (taille % 8);
  int nouvelle_taille = taille + ecart;
  unsigned char *enc_tout_msg = initialisation_bloc(nouvelle_taille);
  unsigned char *message_orga = initialisation_bloc(nouvelle_taille);
  unsigned char *enc_bloc = NULL;
  int tours = nouvelle_taille / 8;
  int ind_bloc;

  message_org(tout_msg, taille, message_orga);

  for (int i = 0; i < tours; i++) {
    ind_bloc = 8 * i;
    enc_bloc = des_encrypte(message_orga + ind_bloc, cle);

    for (int j = 0; j < 8; j++) {
      enc_tout_msg[i * 8 + j] = enc_bloc[j];
    }

    free(enc_bloc);
    enc_bloc = NULL;
  }

  free(message_orga);
  message_orga = NULL;
  return enc_tout_msg;
}



// Fonction qui renvoie la chaine déchiffrée de n'importe quelle taille.
unsigned char *decrypte_tout(unsigned char *tout_msg, int taille,
                             unsigned char *cle) {

  unsigned char *dec_tout_msg = initialisation_bloc(taille);
  unsigned char *dec_bloc;
  int tours = taille / 8;
  int ind_bloc;

  for (int i = 0; i < tours; i++) {
    ind_bloc = 8 * i;
    dec_bloc = des_decrypte(tout_msg + ind_bloc, cle);

    for (int j = 0; j < 8; j++) {
      dec_tout_msg[i * 8 + j] = dec_bloc[j];
    }

    free(dec_bloc);
    dec_bloc = NULL;
  }

  return dec_tout_msg;
}
