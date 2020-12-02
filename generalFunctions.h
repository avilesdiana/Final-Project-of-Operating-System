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


#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <ctype.h>
#include <string.h>
#include <curses.h>
#include <sys/stat.h>
#include <sys/mman.h>

char *mapFile(char *filePath, long *fs);

int leeChar();

int getNext(int cluster, int base);

int getNext16(int cluster, int base);

int getNext32(int cluster, int base);

void readCluster(int cluster, char *buffer, int inicio, int size);

void leeArchivo(char *nombre, long tam, int cluster);

void copiaMem(char *map, int cluster, long tam, int inicio, int size);

void copiaMem16(char *map, int cluster, long tam, int inicio, int size);

int MBR(char *base);

void leeInfo(char *map);

void leeDirectorio(int d);

extern int edita(char *map, long fs);

void pruebas();

int abre(char *filename);

void validations(int valida);

void cleanSystem();

void helpA();

void presentatiog();
