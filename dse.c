#include <stdio.h>
#include <unistd.h>
#include <assert.h>
#include <stdlib.h>
#include <string.h>

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
            //printf("%u",byte);
            //printf(" %d -> %u \n",ind,byte);
            if(byte == 1)
              mon_bloc[ind] = '1';
            else  
              mon_bloc[ind] ='0';
             //printf(" %d -> %u \n",ind,mon_bloc[ind]);
        }
    }
    // printf("\n");
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




int permutation_table[64] = {58, 50, 42, 34, 26, 18, 10, 2,  60, 52, 44, 36, 28, 20,
                             12, 4,  62, 54, 46, 38, 30, 22, 14, 6,  64, 56, 48,
                             40, 32, 24, 16, 8,  57, 49, 41, 33, 25, 17, 9,  1,
                             59, 51, 43, 35, 27, 19, 11, 3,  61, 53, 45, 37, 29,
                             21, 13, 5,  63, 55, 47, 39, 31, 23, 15, 7};



int inv_permutation_table[64] = { 40,8,48,16,56,24,64,32,
                                        39,7,47,15,55,23,63,31,
                                        38,6,46,14,54,22,62,30,
                                        37,5,45,13,53,21,61,29,
                                        36,4,44,12,52,20,60,28,
                                        35,3,43,11,51,19,59,27,
                                        34,2,42,10,50,18,58,26,
                                        33,1,41,9,49,17,57,25
                                      };

int bit32_to_48[] = { 32,1,2,3,4,5,
                      4,5,6,7,8,9,
                      8,9,10,11,12,13,
                      12,13,14,15,16,17,
                      16,17,18,19,20,21,
                      20,21,22,23,24,25,
                      24,25,26,27,28,29,
                      28,29,30,31,32,1
                    };


int sbox[8][4][16] = { { 14, 4, 13, 1, 2, 15, 11, 8, 3, 10, 6, 12, 5, 9, 0, 7,
                      0, 15, 7, 4, 14, 2, 13, 1, 10, 6, 12, 11, 9, 5, 3, 8,
                      4, 1, 14, 8, 13, 6, 2, 11, 15, 12, 9, 7, 3, 10, 5, 0,
                      15, 12, 8, 2, 4, 9, 1, 7, 5, 11, 3, 14, 10, 0, 6, 13 },
                    { 15, 1, 8, 14, 6, 11, 3, 4, 9, 7, 2, 13, 12, 0, 5, 10,
                      3, 13, 4, 7, 15, 2, 8, 14, 12, 0, 1, 10, 6, 9, 11, 5,
                      0, 14, 7, 11, 10, 4, 13, 1, 5, 8, 12, 6, 9, 3, 2, 15,
                      13, 8, 10, 1, 3, 15, 4, 2, 11, 6, 7, 12, 0, 5, 14, 9 },
                    { 10, 0, 9, 14, 6, 3, 15, 5, 1, 13, 12, 7, 11, 4, 2, 8,
                      13, 7, 0, 9, 3, 4, 6, 10, 2, 8, 5, 14, 12, 11, 15, 1,
                      13, 6, 4, 9, 8, 15, 3, 0, 11, 1, 2, 12, 5, 10, 14, 7,
                      1, 10, 13, 0, 6, 9, 8, 7, 4, 15, 14, 3, 11, 5, 2, 12 },
                    { 7, 13, 14, 3, 0, 6, 9, 10, 1, 2, 8, 5, 11, 12, 4, 15,
                      13, 8, 11, 5, 6, 15, 0, 3, 4, 7, 2, 12, 1, 10, 14, 9,
                      10, 6, 9, 0, 12, 11, 7, 13, 15, 1, 3, 14, 5, 2, 8, 4,
                      3, 15, 0, 6, 10, 1, 13, 8, 9, 4, 5, 11, 12, 7, 2, 14 },
                    { 2, 12, 4, 1, 7, 10, 11, 6, 8, 5, 3, 15, 13, 0, 14, 9,
                      14, 11, 2, 12, 4, 7, 13, 1, 5, 0, 15, 10, 3, 9, 8, 6,
                      4, 2, 1, 11, 10, 13, 7, 8, 15, 9, 12, 5, 6, 3, 0, 14,
                      11, 8, 12, 7, 1, 14, 2, 13, 6, 15, 0, 9, 10, 4, 5, 3 },
                    { 12, 1, 10, 15, 9, 2, 6, 8, 0, 13, 3, 4, 14, 7, 5, 11,
                      10, 15, 4, 2, 7, 12, 9, 5, 6, 1, 13, 14, 0, 11, 3, 8,
                      9, 14, 15, 5, 2, 8, 12, 3, 7, 0, 4, 10, 1, 13, 11, 6,
                      4, 3, 2, 12, 9, 5, 15, 10, 11, 14, 1, 7, 6, 0, 8, 13 },
                    { 4, 11, 2, 14, 15, 0, 8, 13, 3, 12, 9, 7, 5, 10, 6, 1,
                      13, 0, 11, 7, 4, 9, 1, 10, 14, 3, 5, 12, 2, 15, 8, 6,
                      1, 4, 11, 13, 12, 3, 7, 14, 10, 15, 6, 8, 0, 5, 9, 2,
                      6, 11, 13, 8, 1, 4, 10, 7, 9, 5, 0, 15, 14, 2, 3, 12 },
                    { 13, 2, 8, 4, 6, 15, 11, 1, 10, 9, 3, 14, 5, 0, 12, 7,
                      1, 15, 13, 8, 10, 3, 7, 4, 12, 5, 6, 11, 0, 14, 9, 2,
                      7, 11, 4, 1, 9, 12, 14, 2, 0, 6, 10, 13, 15, 3, 5, 8,
                      2, 1, 14, 7, 4, 10, 8, 13, 15, 12, 9, 0, 3, 5, 6, 11 } };


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


 int per_sec[32] = { 16, 7, 20, 21,
                    29, 12, 28, 17,
                    1, 15, 23, 26,
                    5, 18, 31, 10,
                    2, 8, 24, 14,
                    32, 27, 3, 9,
                    19, 13, 30, 6,
                    22, 11, 4, 25 };




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
        // printf("a[i] == %c , b[i] == %c ",a[i],b[i]);
        if (a[i] == b[i]) {
            res[i] = '0';
        }
        else {
            res[i] = '1';
        }
    }
    return res;
}



int keyp1[56] = { 57, 49, 41, 33, 25, 17, 9,
                     1, 58, 50, 42, 34, 26, 18,
                     10, 2, 59, 51, 43, 35, 27,
                     19, 11, 3, 60, 52, 44, 36,
                     63, 55, 47, 39, 31, 23, 15,
                     7, 62, 54, 46, 38, 30, 22,
                     14, 6, 61, 53, 45, 37, 29,
                     21, 13, 5, 28, 20, 12, 4 };

int keyp2[48] = { 
	14,17,11,24,1,5, 
	3,28,15,6,21,10, 
	23,19,12,4,26,8, 
	16,7,27,20,13,2, 
	41,52,31,37,47,55, 
	30,40,51,45,33,48, 
	44,49,39,56,34,53, 
	46,42,50,36,29,32 
	}; 

unsigned char * shift_left_once( unsigned char * key){ 
        unsigned char key_chunk[29] ;
        for(int i =0; i< 28;i++){
          key_chunk[i] = key[i];
        }

        for(int i = 0; i < 27; i++){ 
            key[i] = key_chunk[i+1]; 
            // printf("%u",key_chunk[i+1]);
        } 
        key[27] = key_chunk[0];
        // printf("\n");   
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




// declaration du tableau des clè
unsigned char tableau_key[16][48] ;

void generation_des_cle(unsigned char * key_56x8){

  unsigned char * left  = initialisation_bloc(28);
  unsigned char * right = initialisation_bloc(28);
  unsigned char * combiner = initialisation_bloc(56);
  // for(int i = 0; i< 16 ; i++)
  //   tableau_key[i] =  initialisation_bloc(6*8);

  unsigned char * key_56x8_string = initialisation_bloc(7*8);

  // convertir la cle key_56x8 on string 
  key_56x8_string = AffectBits(7*sizeof(unsigned char), key_56x8, key_56x8_string);

  // unsigned char test[56] = "11000011110000000011001110100011001111110000110011111010";
  // for(int i = 0; i< 56;i++)
  //     key_56x8_string[i] = test[i];

  // printf("key_56x8_string = ");
  //   for(int i = 0; i< 56; i++){
  //   printf("%c",key_56x8_string[i]);
  //   if((i+1)%8==0  )
  //     printf("/");
  // }
  // printf("\n");


  // initialisation de left et right de la clé
  for(int i = 0; i< 28;i++){
    left[i] = key_56x8_string[i];
  }
  for(int i = 0; i< 28;i++){
    right[i] = key_56x8_string[i+28];
  }



  unsigned char combiner_pc2[48] ;
  for(int i=0; i<16; i++){ 
      //tableau_key[i] = initialisation_bloc(48);
      // 3.1. For rounds 1, 2, 9, 16 the key_chunks
      // are shifted by one.
      if(i == 0 || i == 1 || i==8 || i==15 ){
        left= shift_left_once(left); 
        // for(int i = 0 ; i< 28; i++)
        //   printf("left[i] = %u\n",left[i]);
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

    // for(int i = 0; i< 56 ; i++)
    //   printf("combine[i] = %u \n",combiner[i]);

    // 5. Finally, the PC2 table is used to transpose
    // the key bits
    for(int j = 0; j < 48; j++){ 
      combiner_pc2[j] = combiner[keyp2[j]-1]; 
    }   

    for(int j = 0; j < 48; j++){ 
      tableau_key[i][j] = combiner_pc2[j];
    }   
    
	} 

  // for(int i = 0; i< 16;i++){
  //     printf("key[%d] = ",i);
  //   for(int c = 0 ; c< 48 ; c++)
  //     printf("%c",tableau_key[i][c]);
  // printf("\n");

  //}

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

  /* Transformer le right de 32bits vers 48bits*/bloc_48 = fonc32_to_48(R0, bloc_48);

  // printf("perm_txt_cl+4 = \n");
  // printBits(4*sizeof(unsigned char), perm_txt_cl+4);
  // puts("");


  // printf("bloc_48 = \n");
  // printBits(6*sizeof(unsigned char), bloc_48);
  // puts("");

  /*  Auxilliaire : sauvegarder les 48bits dans une chaine pour faciliter la substitution*/ mon_chaine_48x8_test = AffectBits(6*sizeof(unsigned char),bloc_48, mon_chaine_48x8_test);
  // printf(" ma_chaine_48x8_test ->  ");
  // for(int i = 0 ; i< 48; i++){
  //   printf("%c",mon_chaine_48x8_test[i]);
  // }
  // printf("\n");
  
  //         La il faut rajouter le xor entre la  "clé" et  et la sortie de la boite E   "mon_chaine_48_8"  après l'envoyer à la sbox .

  // for(int i = 0; i< 1;i++){
  //     printf("key[%d]                =  ",i);
  //   for(int c = 0 ; c< 48 ; c++)
  //     printf("%c",tab_key[c]);

  //   printf("\n");
  // }


  cle_xor_sor_E = xor_(mon_chaine_48x8_test,tab_key,cle_xor_sor_E);
  // printf(" cle_xor_sor_E ->        ");
  // for(int i = 0 ; i< 48; i++){
  //   printf("%c",cle_xor_sor_E[i]);
  // }
  // printf("\n");




  /* Auxilliaire : convertir les bloc de 48x8 vers 64x6 pour les envoyer dans la sbox*/ mon_bloc_64x6 = convert_48x8_64x6(cle_xor_sor_E,mon_bloc_64x6);

  // printf("mon_bloc_64x6 = \n");
  // printBits(8*sizeof(unsigned char), mon_bloc_64x6);
  // puts("");


  // for(int i = 0; i< 8 ; i++)
  //   printf("%d \n",mon_bloc_64x6[i]);



  // for(int i = 0; i< 8 ; i++){
  //     sbox_cl = 0;sbox_lg = 0;
  //     ind_fr_bloc(mon_bloc_64x6[i], &sbox_lg, &sbox_cl);
  //     printf(" mon bloc [i]  %d  : lg -> %d / cl -> %d \n",mon_bloc_64x6[i],(int)sbox_lg,(int)sbox_cl);
  // }

  /* construire bloc 64x4 après le passage de la sbox*/mon_bloc_64x4 = substitution_sbox(mon_bloc_64x6, mon_bloc_64x4);

  // for(int i = 0; i< 8 ; i++){
  //   printf("mon_bloc_64x4[%d] = %d \n",i,mon_bloc_64x4[i]);
  // }

  // Sortie de la sbox 
  // printf("mon_bloc_64x4 = ");
  // printBits(8*sizeof(unsigned char), mon_bloc_64x4);
  // puts("");


  /* convertir le bloc 64x4 en un bloc 32x8 pour l'ajouter après à la permutation secondaire */mon_bloc_32x8 = conv_64x4_to_32x4(mon_bloc_64x4, mon_bloc_32x8);
  // printf("mon_bloc_32x8 = ");
  // printBits(4*sizeof(unsigned char), mon_bloc_32x8);
  // puts("");


  /* Faire la permutation secondaire */perm_bloc_32x8 = permutation_secondaire(mon_bloc_32x8, perm_bloc_32x8);
  // printf("perm_bloc_32x8 = ");
  // printBits(4*sizeof(unsigned char), perm_bloc_32x8);
  // puts("");
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
  // printf("new_R1 =  ");
  // printBits(4*sizeof(unsigned char), new_R1);
  // puts("");
  
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

  /* Faire la permutation initialle */perm_txt_cl = permutation_initiale(message, perm_txt_cl, 8);

  // printf("perm_txt_cl =  ");
  // printBits(8*sizeof(unsigned char), perm_txt_cl);
  //puts("");

  // pour transformer la cle de 8octet en une clè de 7octet 
  key_56x8 = key_64x8_to_56x8(cle, key_56x8);
  // printf("key_56x8 =  ");
  // printBits(7*sizeof(unsigned char),key_56x8);
  // puts("");


  generation_des_cle(key_56x8);

  
  for(int i = 0; i< 16;i++)
    perm_txt_cl = etape_de_compression(perm_txt_cl,tableau_key[i]);


  for(int i = 0 ; i< 4;i++)
    before_final_encrypte[i] = perm_txt_cl[i+4];

  
  for(int i = 0 ; i< 4;i++)
    before_final_encrypte[i+4] = perm_txt_cl[i];


  // printf("cle  ");
  // printBits(8*sizeof(unsigned char),cle);
  // puts("");

  final_encrypte = inverse_permutation(before_final_encrypte, final_encrypte, 8);
  // printf("final_encrypte = \n");
  // printBits(8*sizeof(unsigned char), final_encrypte);
  // puts("");

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
  // printf("key_56x8 =  ");
  // printBits(7*sizeof(unsigned char),key_56x8);
  // puts("");


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
  printf("nouvelle taille = %d \n",nouvelle_taille);
  unsigned char * enc_tout_msg =  initialisation_bloc(nouvelle_taille);
  unsigned char * message_orga =  initialisation_bloc(nouvelle_taille);
  unsigned char * enc_bloc = initialisation_bloc(8);
  int tours = nouvelle_taille/8 ;
  int ind_bloc;


  message_orga = message_org(tout_msg, taille,message_orga);
  printf("message orga  = \n");
  printBits(nouvelle_taille*sizeof(unsigned char), message_orga);
  puts("");

  for(int i = 0 ; i< tours ; i++){
    ind_bloc = 8*i;
    enc_bloc = des_encrypte(message_orga+ind_bloc , cle);

    for(int j = 0 ; j< 8 ; j++){
      enc_tout_msg[i*8+j] = enc_bloc[j];
    }
  }

  printf("message après chiffrement = ");
  printBits(nouvelle_taille*sizeof(unsigned char), enc_tout_msg);
  puts("");

  return enc_tout_msg;
}



unsigned char * decrypte_tout(unsigned char * tout_msg , int taille, unsigned char * cle){

  unsigned char * dec_tout_msg =  initialisation_bloc(taille);
  unsigned char * dec_bloc     =  initialisation_bloc(8);
  int tours = taille/8 ;
  printf("taille = %d",taille);
  int ind_bloc;

  for(int i = 0 ; i< tours ; i++){
    ind_bloc = 8*i;
    dec_bloc = des_decrypte(tout_msg+ind_bloc , cle);

    for(int j = 0 ; j< 8 ; j++){
      dec_tout_msg[i*8+j] = dec_bloc[j];
    }
  }

  printf("message après déchiffrement = \n");
  printBits(taille*sizeof(unsigned char), dec_tout_msg);
  puts("");

  return dec_tout_msg;
}






int main() {
  int i = 98;
  float f = 23.45f;
  unsigned char   txt_cl_Av[1024] = "le loup est un animal carnivore qui vit dans les montagnes en groupe il se nourrit des autres autres animaux comme le lapin ou les souris ou les oiseaux salut ";
  unsigned char   cle[9] = "benkazer";

  unsigned char * enc = initialisation_bloc(16);
  unsigned char * dec = initialisation_bloc(16);

  int taille_chaine_a_enc = strlen(txt_cl_Av);
  printf(" la taille = %d \n",taille_chaine_a_enc);

  printf("message avant chiffrement = ");
  printBits(taille_chaine_a_enc*sizeof(unsigned char), txt_cl_Av);
  puts("");


  enc = enctypte_tout(txt_cl_Av,taille_chaine_a_enc,cle);
  // printf("message après chiffrement = ");
  // printBits(16*sizeof(unsigned char), enc);
  // puts("");

  int taille_chaine_a_dec = strlen(enc);
  printf("taille chaine a decrypte %d \n",taille_chaine_a_dec);
  dec = decrypte_tout(enc, taille_chaine_a_dec,cle);
  // printf("message après déchiffrement = \n");
  // printBits(taille_chaine_a_dec*sizeof(unsigned char), dec);
  // puts("");

  // printf(" message dec = %s\n",dec);

  printf("chaine dec = %s\n",dec);

  return 0;
}