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

//Función que limpia la pantalla
void cleanScreen()
{
    system("clear");
}

//Función de ayuda para saber como ejecutar el programa
void helpA()
{
    printf("\nPuede ingresar:\n ./proyectoFinal -h o ./proyectoFinal ayuda \n");
    printf("\npara más información de opciones.");
}

//Función que imprime en pantalla el funcionamiento del programa
void presentation()
{
    printf("\n\n\t\t\tPROYECTO FINAL\n\n");
    printf("\t  --------------------------------------------\n\n");
    printf("\t Equipo: \n");
    printf("\t   Diana Aviles, Manuel Tijerina, Eduardo Perez\n\n");
    printf("\t 1era entrega : 9 / 11 / 20\n\n");
    printf("\t  Tabla de información de la imagen y Calcular: \n");
    printf("\t   -La dirección del directorio raíz\n\t   -La dirección donde empieza la información de la imagen\n\n");
    printf("\t\tCOMPILACIÓN:    ./proyectoFinal [Nombre de la imagen]\n ");
    printf("\t\tEJEMPLO: ./proyectoFinal allinone.img\n\n");
    getchar();
}


//Función que valida los argumentos.
void validations(int valida)
{
    switch (valida)
    {
    case 1:
        cleanScreen();
        printf("\nFaltan Argumentos \n");
        helpA();
        getchar();
        break;

    case 2:
        cleanScreen();
        printf("\nSobran Argumentos \n");
        helpA();
        getchar();
        break;

    case 3:
        cleanScreen();
        printf("\nError: la extensión del archivo no es .img\n");
        helpA();
        getchar();
        break;

    case 4:
        cleanScreen();
        presentation();
        getchar();
        break;

    default:
        cleanScreen();
        printf("\nIngrese una opción correcta\n");
        helpA();
        getchar();
    }
}
