#include "generalFunctions.h"

void cleanScreen()
{
    system("clear");
}

//
void validations(int valida)
{
    switch (valida)
    {
    case 1:
        cleanScreen();
        printf("\nFaltan Argumentos \n");
        getchar();
        break;

    case 2:
        cleanScreen();
        printf("\nSobran Argumentos \n");
        getchar();
        break;

    case 3:
        cleanScreen();
        printf("\nError: la extensión del archivo no es .img\n");
        getchar();
        break;

    case 4:
        cleanScreen();
        printf("ayuda");
        getchar();
        break;

    default:
        cleanScreen();
        printf("\nIngrese una opción correcta\n");
        printf("Puede ingresar ./proyectoFinal -h o ./proyectoFinal ayuda \n");
        printf("para más información de opciones.");
        getchar();
    }
}
