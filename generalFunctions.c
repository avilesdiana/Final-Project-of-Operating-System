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

//Estructura de la tabla de información
typedef struct imageInformation
{
  short int *sectorSize;
  int numberOfSectorsPerCluster;
  short int *reservedSectors;
  int numberOfCopiesofFat;
  short int *numberOfEntriesRootDirectory;
  int *numberOfDiskSectors;
  short int *fatSize;
  char volumenLabel[11];
  char idSistema[11];
  int rootDirectoryAddress;
  int addressImageInfoBegins;

} imageInformation;

int fd; // Archivo a leer
int fs;
char *map;

imageInformation imageInfo;

//Función que mapea el archivo
char *mapFile(char *filePath)
{
  /* Abre archivo */
  fd = open(filePath, O_RDWR);
  if (fd == -1)
  {
    system("clear");
    perror("\nError abriendo el archivo\n");
    getchar();
    return (NULL);
  }

  /* Mapea archivo */
  struct stat st;
  fstat(fd, &st);
  fs = st.st_size;

  char *map = mmap(0, fs + 200, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
  if (map == MAP_FAILED)
  {
    close(fd);
    system("clear");
    perror("\nError mapeando el archivo\n");
    getchar();
    return (NULL);
  }
  return map;
}

//Función que 
int getNext(int cluster, int base)
{
  // Para FAT12
  int offset = cluster + cluster / 2;
  int flag = cluster % 2; // Nos dice si en la parte baja o alta

  unsigned char b1, b2;
  printf("%x", map);
  b1 = map[base + offset];
  printf("%02x:", b1);
  b2 = map[base + offset + 1];
  printf("%02x\n", b2);
  int result = b1 | b2 << 8; // Los bits mas significativos van al final

  if (flag)
  {
    result >>= 4;
  }
  else
  {
    result &= 0xfff;
  }

  printf("%04x\n", result);

  return result;
}

// Función que 
void pruebas()
{
  int d;
  for (int i = 0; i < 10; i++)
  {
    d = getNext(i, 0x200);
    printf("%03x  %d\n", d, d);
  }
}

// Función que Abre el archivo e imprime su información
void abre(char *filename)
{
  map = mapFile(filename);
  if (map == NULL)
  {
    exit(EXIT_FAILURE);
  }

  //pruebas();
  system("clear");
  printf("\n    Tabla de información de la imagen\n");
  printf("--------------------------------------------\n\n");
  imageInfo.sectorSize = (short int *)&map[11];
  printf("Tamaño del  sector:              %d \n", *imageInfo.sectorSize);

  imageInfo.numberOfSectorsPerCluster = map[13];
  printf("Numero de sectores por cluster:  %d \n", imageInfo.numberOfSectorsPerCluster);

  imageInfo.reservedSectors = (short int *)&map[14];
  printf("Sectores reservados:             %d \n", *imageInfo.reservedSectors);

  imageInfo.numberOfCopiesofFat = map[16];
  printf("Número de copias del FAT:        %d \n", imageInfo.numberOfCopiesofFat);

  imageInfo.numberOfEntriesRootDirectory = (short int *)&map[17];
  printf("Entradas directorio Raíz:        %d \n", *imageInfo.numberOfEntriesRootDirectory);

  imageInfo.numberOfDiskSectors = (int *)&map[32];
  printf("Número de sectores del disco:    %d \n", *imageInfo.numberOfDiskSectors);

  imageInfo.fatSize = (short int *)&map[22];
  printf("Tamaño del FAT:                  %d \n", *imageInfo.fatSize);

  strcpy(imageInfo.volumenLabel, &map[43]);
  printf("Etiqueta del Volumen:            %s \n ", imageInfo.volumenLabel);

  strcpy(imageInfo.idSistema, &map[0x36]);
  printf("Id Systema:                      %s \n ", imageInfo.idSistema);

  imageInfo.rootDirectoryAddress = (*imageInfo.reservedSectors + (imageInfo.numberOfCopiesofFat * *imageInfo.fatSize)) * *imageInfo.sectorSize;
  imageInfo.addressImageInfoBegins = imageInfo.rootDirectoryAddress + (*imageInfo.numberOfEntriesRootDirectory * 32);

  printf("\nDireccion del directorio raiz: 0x%04x\n", imageInfo.rootDirectoryAddress);
  printf("\nDireccion donde comienza la información de archivos en la imagen: 0x%04x\n", imageInfo.addressImageInfoBegins);

  printf("\n\t\t[Press enter to continue]\n\n");
  getchar();

  if (munmap(map, fs) == -1)
  {

    system("clear");
    perror("\nError al desmapear\n");
    getchar();
  }
  close(fs);
}
