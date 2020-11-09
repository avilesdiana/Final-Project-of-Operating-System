#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <ctype.h>
#include <string.h>
#include <curses.h>
#include <sys/stat.h>
#include <sys/mman.h>

char *mapFile(char *filePath);

int getNext(int cluster, int base);

void pruebas();

void abre(char *filename);

void validations(int valida);

void cleanSystem();