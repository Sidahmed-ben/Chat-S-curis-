

#include "Des.h"

int main() {
  int i = 98;
  float f = 23.45f;
  unsigned char   txt_cl_Av[1024] = "le loup est un animal carnivore qui vit dans les montagnes en groupe il vit AZERT12345";
  unsigned char   cle[9] = "benkazer";

  unsigned char * enc = initialisation_bloc(16);
  unsigned char * dec = initialisation_bloc(16);

  int taille_chaine_a_enc = strlen(txt_cl_Av);
  // printf(" la taille = %d \n",taille_chaine_a_enc);

  // printf("message avant chiffrement = ");
  // printBits(taille_chaine_a_enc*sizeof(unsigned char), txt_cl_Av);
  // puts("");


  enc = enctypte_tout(txt_cl_Av,taille_chaine_a_enc,cle);
  // printf("message après chiffrement = ");
  // printBits(16*sizeof(unsigned char), enc);
  // puts("");

  int taille_chaine_a_dec = strlen(enc);
  // printf("taille chaine a decrypte %d \n",taille_chaine_a_dec);
  dec = decrypte_tout(enc, taille_chaine_a_dec,cle);
  // printf("message après déchiffrement = \n");
  // printBits(taille_chaine_a_dec*sizeof(unsigned char), dec);
  // puts("");

  printf(" message dec = %s\n",dec);

  // printf("chaine dec = %s\n",dec);

  return 0;
}