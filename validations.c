#include "generalFunctions.h"

void cleanScreen()
{
    system("clear");
}

void helpA()
{
    printf("\nPuede ingresar:\n ./proyectoFinal -h o ./proyectoFinal ayuda \n");
    printf("\npara más información de opciones.");
}

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
//
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
