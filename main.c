/*----------------------------------------------------------------------------------------------------
  P R O Y E C T O    F I N A L
  SISTEMAS OPERATIVOS
  Fecha de 2da Entrega: 17 de Noviembre 2020
  Integrantes:
  Diana Laura Aviles Elizalde
  Manuel Tijerina
  Eduardo Pérez 
  @Param: Archivo imagen
  @Salidas: Tabla información de la imagen, Calculo de
            La dirección de directorio Raíz
            La dirección donde empieza la información de archivos de la imagen
  ---------------------------------------------------------------------------------------------------*/

#include "generalFunctions.h"

int main(int argc, char *argv[])
{
  char imageName[25];
  int valida = 0;

  //Validación de no. de argumentos
  if (argc < 1)
  {
    valida = 1;
    validations(valida);
  }
  else
  {
    if (argc > 2)
    {
      valida = 2;
      validations(valida);
    }
    else
    {
      //Compara en caso de que queira imprimir el menu de ayuda
      if ((strcmp(argv[1], "-h") == 0) || (strcmp(argv[1], "ayuda") == 0))
      {
        valida = 4;
        validations(valida);
      }
      else
      {
        //Valida que ingrese un archivo válido
        if (strstr(argv[1], ".img") == NULL)
        {
          valida = 3;
          validations(valida);
        }
        else
        {
          strcpy(imageName, argv[1]);
          //Analiza en la imagen ingresada en el argumento
          abre(imageName);
        }
      }
    }
  }
}
