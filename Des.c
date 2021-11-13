
#include "Des.h"

void printBits(size_t const size, void const * const ptr)
{
    unsigned char *b = (unsigned char*) ptr;
    unsigned char byte;
    int i, j,ind;
    
    for (i = 0; i < size; i++) {
        for (j = 7; j >=0; j--) {
            byte = (b[i] >> j) & 1;
            ind = (i*size +(7-j));
            printf("%u",byte);
        }
        printf("/");
    }
    puts("");
}

unsigned char *AffectBits(size_t const size, void const * const ptr , unsigned char *mon_bloc )
{
    unsigned char *b = (unsigned char*) ptr;
    unsigned char byte;
    int i, j,ind;

    for (i = 0; i < size; i++) {
        for (j = 7; j >=0; j--) {
            byte = (b[i] >> j) & 1;
            ind = (i*8 +(7-j));
            if(byte == 1)
              mon_bloc[ind] = '1';
            else  
              mon_bloc[ind] ='0';
        }
    }
    return mon_bloc;
}


void ind_fr_bloc(unsigned char bloc ,unsigned char *ligne ,unsigned char * colonne){
  unsigned char cl,cl_tmp,lg_tmp;

  // remplir par la lignepar le 5éme bit
  lg_tmp = bloc >> 5 & 1;
  lg_tmp = lg_tmp << 1; 
  *ligne |= lg_tmp;

  // remplir par la lignepar le 5éme bit
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







unsigned char * substitution_sbox(unsigned char * bloc_64x6 ,unsigned char * bloc_64x4){

  unsigned char ligne,colonne = 0;
  for(int i = 0; i< 8 ;i++){
    ligne = 0;
    colonne = 0;
    ind_fr_bloc(bloc_64x6[i], &ligne, &colonne);
    bloc_64x4[i] = sbox[i][ligne][colonne];
  }

  return bloc_64x4;
}





unsigned char * convert_48x8_64x6(unsigned char * chaine_48x8 , unsigned char * bloc_64x6){
  unsigned char bloc = {0};
  unsigned char bloc_tmp = {0};
  int shift;
  int ind;
  for(int i = 0 ; i < 8 ; i++){
      bloc_tmp = 0 ;
      bloc = 0;
      for(int j = 0 ; j< 6; j++){
        if(chaine_48x8[i*6+j] == '1'){
          ind = (i*6+j);
          shift = 5-j;
          bloc_tmp = (bloc_tmp >> shift) | 1;
          bloc_tmp = bloc_tmp << shift;
          bloc |= bloc_tmp;
        }else {
          continue;
        }
    }
    bloc_64x6[i] = bloc;
  }
  return bloc_64x6;
}



unsigned char * fonc32_to_48(unsigned char * bloc32,unsigned char *bloc48){
  unsigned char permutation_bloc ={0}  ;
  unsigned char permutation_bloc_tmp ={0}  ;
  int  ind_tc = 0;
  int  ind_shift = 0;  
   for(int i = 0 ; i< 6; i++){
    permutation_bloc = 0  ;
    permutation_bloc_tmp = 0  ;
    for(int j = 0; j< 8; j++){
      ind_tc  = (int)((bit32_to_48[i*8+j]-1)/8/* 4 = nombre de bloc de la srource */);
      ind_shift = 7-((bit32_to_48[i*8+j]-1)%8);
      permutation_bloc_tmp = (bloc32[ind_tc]>>ind_shift) & 1 ;
      permutation_bloc_tmp = permutation_bloc_tmp<<(7-j);
      permutation_bloc |= permutation_bloc_tmp;
    }
    bloc48[i] = permutation_bloc ;
  }
  return bloc48;
}


unsigned char * initialisation_bloc(int size){
  unsigned char * perm_t_c = (unsigned char*)malloc(size*sizeof(unsigned char));
  assert(perm_t_c);
  return perm_t_c;
}

unsigned char *permutation_initiale(unsigned char text_claire[9],unsigned char * permutation_text_claire, int taille) { 
  unsigned char permutation_bloc ={0}  ;
  unsigned char permutation_bloc_tmp ={0}  ;
  int  ind_tc = 0;
  int  ind_shift = 0;  

  for(int i = 0 ; i< taille; i++){
    permutation_bloc = 0  ;
    permutation_bloc_tmp = 0  ;
    for(int j = 0; j< 8; j++){
      ind_tc  = (int)((permutation_table[i*8+j]-1)/8);
      ind_shift = 7-((permutation_table[i*8+j]-1)%8);
      permutation_bloc_tmp = (text_claire[ind_tc]>>ind_shift) & 1 ;
      permutation_bloc_tmp = permutation_bloc_tmp<<(7-j);
      permutation_bloc |= permutation_bloc_tmp;
    }
    permutation_text_claire[i] = permutation_bloc ;
  }

  return permutation_text_claire;
}


unsigned char *inverse_permutation(unsigned char *text_claire,unsigned char * permutation_text_claire, int taille) { 
  unsigned char permutation_bloc ={0}  ;
  unsigned char permutation_bloc_tmp ={0}  ;
  int  ind_tc = 0;
  int  ind_shift = 0;  
  for(int i = 0 ; i< 8; i++){
    permutation_bloc = 0  ;
    permutation_bloc_tmp = 0  ;
    for(int j = 0; j< 8; j++){
      ind_tc  = (int)((inv_permutation_table[i*8+j]-1)/8);
      ind_shift = 7-((inv_permutation_table[i*8+j]-1)%8);
      permutation_bloc_tmp = (text_claire[ind_tc]>>ind_shift) & 1 ;
      permutation_bloc_tmp = permutation_bloc_tmp<<(7-j); 
      permutation_bloc |= permutation_bloc_tmp;
    }
    permutation_text_claire[i] = permutation_bloc ;
  }

  return permutation_text_claire;
}

unsigned char * conv_64x4_to_32x4(unsigned char * bloc_64x4 , unsigned char * bloc_32_4){
  char bloc , bloc_tmp =0;;
  for(int i = 0; i < 4 ; i++){
    bloc_tmp = 0;
    bloc_tmp = bloc_64x4[i*2] << 4| bloc_64x4[(i*2)+1] ;
    bloc_32_4[i] = bloc_tmp;
  }
  return bloc_32_4;
}


 




unsigned char *permutation_secondaire(unsigned char * bloc_32x4,unsigned char * permutation_bloc_32x4) { 

  unsigned char permutation_bloc ={0}  ;
  unsigned char permutation_bloc_tmp ={0}  ;
  int  ind_tc = 0;
  int  ind_shift = 0;  

  for(int i = 0 ; i< 4; i++){
    permutation_bloc = 0  ;
    permutation_bloc_tmp = 0  ;
    for(int j = 0; j< 8; j++){
      ind_tc  = (int)((per_sec[i*8+j]-1)/8);
      ind_shift = 7-((per_sec[i*8+j]-1)%8);
      permutation_bloc_tmp = (bloc_32x4[ind_tc]>>ind_shift) & 1 ;
      permutation_bloc_tmp = permutation_bloc_tmp<<(7-j);
      permutation_bloc |= permutation_bloc_tmp;
    }
    permutation_bloc_32x4[i] = permutation_bloc ;
  }


  return permutation_bloc_32x4;
}



unsigned char * bloc32x8_xor_bloc32x8( unsigned char * result,unsigned char * left_bloc_32x8 , unsigned char * bloc_f_32x8)
{
  for(int i = 0 ; i< 4 ; i++){
    result[i] = left_bloc_32x8[i] ^ bloc_f_32x8[i];
  }
  return result;
}








unsigned char * xor_(unsigned char * a, unsigned char * b , unsigned char * res)
{

    for (int i = 0; i < 48; i++) {
        if (a[i] == b[i]) {
            res[i] = '0';
        }
        else {
            res[i] = '1';
        }
    }
    return res;
}





unsigned char * shift_left_once( unsigned char * key){ 
        unsigned char key_chunk[29] ;
        for(int i =0; i< 28;i++){
          key_chunk[i] = key[i];
        }

        for(int i = 0; i < 27; i++){ 
            key[i] = key_chunk[i+1]; 
        } 
        key[27] = key_chunk[0];
    return key; 
} 

unsigned char * shift_left_twice( unsigned char * key){ 
        unsigned char key_chunk[29] ;
        for(int i =0; i< 28;i++){
           key_chunk[i] = key[i];
        }
         
        for(int i = 0; i < 26; i++){ 
          key[i] = key_chunk[i+2]; 
        } 
        key[26] = key_chunk[0];
        key[27] = key_chunk[1];   
    return key; 
} 





void generation_des_cle(unsigned char * key_56x8){

  unsigned char * left  = initialisation_bloc(28);
  unsigned char * right = initialisation_bloc(28);
  unsigned char * combiner = initialisation_bloc(56);


  unsigned char * key_56x8_string = initialisation_bloc(7*8);

  // convertir la cle key_56x8 on string 
  key_56x8_string = AffectBits(7*sizeof(unsigned char), key_56x8, key_56x8_string);
  // initialisation de left et right de la clé
  for(int i = 0; i< 28;i++){
    left[i] = key_56x8_string[i];
  }
  for(int i = 0; i< 28;i++){
    right[i] = key_56x8_string[i+28];
  }



  unsigned char combiner_pc2[48] ;
  for(int i=0; i<16; i++){ 
      if(i == 0 || i == 1 || i==8 || i==15 ){
        left= shift_left_once(left); 
        right= shift_left_once(right);
      } 
      // 3.2. For other rounds, the key_chunks
      // are shifted by two
      else{
        left= shift_left_twice(left); 
        right= shift_left_twice(right);
      }
    // 4. The chunks are combined
    for(int k = 0; k< 28;k++){
      combiner[k] = left[k];
    }
    for(int k = 28; k< 56;k++){
      combiner[k] = right[k-28];
    }

    // 5. Finally, the PC2 table is used to transpose
    // the key bits
    for(int j = 0; j < 48; j++){ 
      combiner_pc2[j] = combiner[keyp2[j]-1]; 
    }   

    for(int j = 0; j < 48; j++){ 
      tableau_key[i][j] = combiner_pc2[j];
    }   
    
	} 
}


unsigned char * F(unsigned char * R0, unsigned char tab_key[48]){

  unsigned char * bloc_48       =   initialisation_bloc(6);
  unsigned char * txt_cl_Ap     =   initialisation_bloc(8);
  unsigned char * mon_bloc_64x6 = initialisation_bloc(8);
  unsigned char * mon_chaine_48x8_test = initialisation_bloc(6*8);
  unsigned char * mon_bloc_64x4 = initialisation_bloc(8);
  unsigned char * mon_bloc_32x8 = initialisation_bloc(4);
  unsigned char * perm_bloc_32x8 = initialisation_bloc(4);
  unsigned char * cle_xor_sor_E = initialisation_bloc(6*8);

  /* Transformer le right de 32bits vers 48bits*/
  bloc_48 = fonc32_to_48(R0, bloc_48);
  /*  Auxilliaire : sauvegarder les 48bits dans une chaine pour faciliter la substitution*/ 
  mon_chaine_48x8_test = AffectBits(6*sizeof(unsigned char),bloc_48, mon_chaine_48x8_test);
  cle_xor_sor_E = xor_(mon_chaine_48x8_test,tab_key,cle_xor_sor_E);
  /* Auxilliaire : convertir les bloc de 48x8 vers 64x6 pour les envoyer dans la sbox*/ 
  mon_bloc_64x6 = convert_48x8_64x6(cle_xor_sor_E,mon_bloc_64x6);
  /* construire bloc 64x4 après le passage de la sbox*/
  mon_bloc_64x4 = substitution_sbox(mon_bloc_64x6, mon_bloc_64x4);
  /* convertir le bloc 64x4 en un bloc 32x8 pour l'ajouter après à la permutation secondaire */
  mon_bloc_32x8 = conv_64x4_to_32x4(mon_bloc_64x4, mon_bloc_32x8);
  /* Faire la permutation secondaire */
  perm_bloc_32x8 = permutation_secondaire(mon_bloc_32x8, perm_bloc_32x8);
  return  perm_bloc_32x8;

}




unsigned char * key_64x8_to_56x8(unsigned char * key_64x8, unsigned char * key_56x8){

  unsigned char permutation_bloc ={0}  ;
  unsigned char permutation_bloc_tmp ={0}  ;
  int  ind_tc = 0;
  int  ind_shift = 0;  

  for(int i = 0 ; i< 7; i++){
    permutation_bloc = 0  ;
    permutation_bloc_tmp = 0  ;
    for(int j = 0; j< 8; j++){
      ind_tc  = (int)((keyp1[i*8+j]-1)/8);
      ind_shift = 7-((keyp1[i*8+j]-1)%8);
      permutation_bloc_tmp = (key_64x8[ind_tc]>>ind_shift) & 1 ;
      permutation_bloc_tmp = permutation_bloc_tmp<<(7-j);
      permutation_bloc |= permutation_bloc_tmp;
    }
    key_56x8[i] = permutation_bloc ;
  }

  return key_56x8;
}







unsigned char * etape_de_compression(unsigned char * perm_txt_cl,unsigned char tab_key[48]){

  unsigned char * sortie_F   = initialisation_bloc(4);
  unsigned char * new_R1     = initialisation_bloc(4);
  unsigned char * new_sortie = initialisation_bloc(8);
  
  sortie_F = F(perm_txt_cl+4,tab_key);



  new_R1 = bloc32x8_xor_bloc32x8(new_R1, sortie_F, perm_txt_cl);

  for(int i = 0; i< 4 ; i++){
    new_sortie[i] = (perm_txt_cl+4)[i];
  }

  for(int i = 4; i< 8 ; i++){
    new_sortie[i] = new_R1[i-4];
  }

  return new_sortie;
}




unsigned char * des_encrypte(unsigned char * message , unsigned char * cle){

  unsigned char * perm_txt_cl   =   initialisation_bloc(8);
  unsigned char * perm_txt_en   =   initialisation_bloc(8);
  unsigned char * sortie_etape_compression = initialisation_bloc(8);
  unsigned char * key_56x8 = initialisation_bloc(7);
  unsigned char * before_final_encrypte = initialisation_bloc(8);
  unsigned char * final_encrypte = initialisation_bloc(8);
  unsigned char * txt_en = initialisation_bloc(8);

  /* Faire la permutation initialle */
  perm_txt_cl = permutation_initiale(message, perm_txt_cl, 8);

  // pour transformer la cle de 8octet en une clè de 7octet 
  key_56x8 = key_64x8_to_56x8(cle, key_56x8);
  generation_des_cle(key_56x8);

  
  for(int i = 0; i< 16;i++)
    perm_txt_cl = etape_de_compression(perm_txt_cl,tableau_key[i]);


  for(int i = 0 ; i< 4;i++)
    before_final_encrypte[i] = perm_txt_cl[i+4];

  
  for(int i = 0 ; i< 4;i++)
    before_final_encrypte[i+4] = perm_txt_cl[i];


  final_encrypte = inverse_permutation(before_final_encrypte, final_encrypte, 8);

  for(int i = 0; i< 8;i++)
    txt_en[i] = final_encrypte[i];

  return txt_en;
}




unsigned char * des_decrypte(unsigned char * message , unsigned char * cle){

  unsigned char * perm_txt_en   =   initialisation_bloc(8);
  unsigned char * before_final_encrypte = initialisation_bloc(8);
  unsigned char * final_encrypte = initialisation_bloc(8);
  unsigned char * key_56x8 = initialisation_bloc(7);



  /* Faire la permutation initialle */perm_txt_en = permutation_initiale(message, perm_txt_en, 8);

  // generation des clès 
  key_56x8 = key_64x8_to_56x8(cle, key_56x8);
  generation_des_cle(key_56x8);

  for(int i = 15; i >= 0;i--)
    perm_txt_en = etape_de_compression(perm_txt_en,tableau_key[i]);


  for(int i = 0 ; i< 4;i++)
    before_final_encrypte[i] = perm_txt_en[i+4];

  
  for(int i = 0 ; i< 4;i++)
    before_final_encrypte[i+4] = perm_txt_en[i];


  final_encrypte = inverse_permutation(before_final_encrypte, final_encrypte, 8);

  return final_encrypte;
}

unsigned char * message_org(unsigned char * message ,int taille, unsigned char * message_orga){
  int ecart = 8-(taille%8);
  int nouvelle_taille = taille+ecart;
  message_orga = initialisation_bloc(nouvelle_taille);
  for(int i =0; i< taille; i++){
    message_orga[i] = message[i]; 
    // a regargder si en rajoute les esapces pour les dernier bits de l'ecart .
  }

  return message_orga;
}

unsigned char * enctypte_tout(unsigned char * tout_msg , int taille, unsigned char * cle){

  int ecart = 8-(taille%8);
  int nouvelle_taille = taille+ecart;
  // printf("nouvelle taille = %d \n",nouvelle_taille);
  unsigned char * enc_tout_msg =  initialisation_bloc(nouvelle_taille);
  unsigned char * message_orga =  initialisation_bloc(nouvelle_taille);
  unsigned char * enc_bloc = initialisation_bloc(8);
  int tours = nouvelle_taille/8 ;
  int ind_bloc;


  message_orga = message_org(tout_msg, taille,message_orga);
  // printf("message orga  = \n");
  // printBits(nouvelle_taille*sizeof(unsigned char), message_orga);
  // puts("");

  for(int i = 0 ; i< tours ; i++){
    ind_bloc = 8*i;
    enc_bloc = des_encrypte(message_orga+ind_bloc , cle);

    for(int j = 0 ; j< 8 ; j++){
      enc_tout_msg[i*8+j] = enc_bloc[j];
    }
  }

  // printf("message après chiffrement = ");
  // printBits(nouvelle_taille*sizeof(unsigned char), enc_tout_msg);
  // puts("");

  return enc_tout_msg;
}



unsigned char * decrypte_tout(unsigned char * tout_msg , int taille, unsigned char * cle){

  unsigned char * dec_tout_msg =  initialisation_bloc(taille);
  unsigned char * dec_bloc     =  initialisation_bloc(8);
  int tours = taille/8 ;
  //printf("taille = %d",taille);
  int ind_bloc;

  for(int i = 0 ; i< tours ; i++){
    ind_bloc = 8*i;
    dec_bloc = des_decrypte(tout_msg+ind_bloc , cle);

    for(int j = 0 ; j< 8 ; j++){
      dec_tout_msg[i*8+j] = dec_bloc[j];
    }
  }

  // printf("message après déchiffrement = \n");
  // printBits(taille*sizeof(unsigned char), dec_tout_msg);
  // puts("");

  return dec_tout_msg;
}
