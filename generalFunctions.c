#include "generalFunctions.h"

typedef struct imageInformation
{
  short int *sectorSize;
  int numberOfSectorsPerCluster;
  short int *reservedSectors;
  int numberOfCopiesofFat;
  short int *numberOfEntriesRootDirectory;
  int *numberOfDiskSectors;
  short int *fatSize;
  char *volumenLabel;
  char *idSistema;

} imageInformation;

int fd; // Archivo a leer
int fs;
char *map;

imageInformation imageInfo;

//
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

//
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

//
void pruebas()
{
  int d;
  for (int i = 0; i < 10; i++)
  {
    d = getNext(i, 0x200);
    printf("%03x  %d\n", d, d);
  }
}

//
void abre(char *filename)
{
  map = mapFile(filename);
  if (map == NULL)
  {
    exit(EXIT_FAILURE);
  }

  //pruebas();
  imageInfo.sectorSize = (short int *)&map[11];
  printf("Tamaño del  sector:              %d \n", *imageInfo.sectorSize);

  imageInfo.numberOfSectorsPerCluster = map[13];
  printf("Numero de sectores por cluster:  %d \n", imageInfo.numberOfSectorsPerCluster);

  imageInfo.reservedSectors = (short int *)&map[14];
  printf("Sectores reservados:             %d \n", *imageInfo.reservedSectors);

  imageInfo.numberOfCopiesofFat = map[16];
  printf("Número de copias del FAT :       %d \n", imageInfo.numberOfCopiesofFat);

  imageInfo.numberOfEntriesRootDirectory = (short int *)&map[17];
  printf("Entradas directorio Raíz:        %d \n", *imageInfo.numberOfEntriesRootDirectory);

  imageInfo.numberOfDiskSectors = (int *)&map[32];
  printf("Número de sectores del disco:    %d \n", *imageInfo.numberOfDiskSectors);

  imageInfo.fatSize = (short int *)&map[22];
  printf("Tamaño del FAT:                  %d \n", *imageInfo.fatSize);

  if (munmap(map, fs) == -1)
  {

    system("clear");
    perror("\nError al desmapear\n");
    getchar();
  }
  close(fs);
}
