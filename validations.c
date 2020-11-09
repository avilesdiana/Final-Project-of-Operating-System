#include "generalFunctions.h"

//
void validations(int valida){
   switch (valida){
       case 1:
       printf("Faltan Argumentos \n");
       break;

       case 2:
       printf("Sobran Argumentos \n");
       break; 

       case 3:
       printf("Error: la extensión del archivo no es .img\n");
       break;
       
       default:
       printf("INGRESE UNA OPCIÓN CORRECTA");

   }
}
