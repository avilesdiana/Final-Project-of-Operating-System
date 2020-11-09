#include "operaciones.h"


int main(int argc, char *argv[]){
    char imageName[25];

   if(argc < 1){
     printf("Faltan Argumentos \n");
   }
   else{
     if(argc > 2){
       printf("Sobran argumentos \n");
     }
     else{
       if(strstr(argv[1], ".img") == NULL){
        printf("Error: la extensión del archivo no es .img\n");
       }
       else{
          strcpy(imageName,argv[1]);
          abre(imageName);
       }
     }
   }
}