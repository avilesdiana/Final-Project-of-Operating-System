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

int leeChar()
{
  int chars[5];
  int ch, i = 0;
  nodelay(stdscr, TRUE);
  while ((ch = getch()) == ERR)
    ; //Espera activa
  ungetch(ch);
  while ((ch = getch()) != ERR)
  {
    chars[i++] = ch;
  }
  //Convierte a número con todo lo leído
  int res = 0;
  for (int j = 0; j < i; j++)
  {
    res <<= 8;
    res |= chars[j];
  }
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

  void readCluster(int cluster, char *buffer)
  {
    int offset = cluster * 512;
    memcpy(buffer, &map[0x3E00 + offset], 512);
  }

  void leeArchivo(char *nombre, long tam, int cluster)
  {
    int fo = open(nombre, O_WRONLY | O_CREAT, 0x755);
    char buffer[512];
    int te;
    do
    {
      readCluster(cluster, buffer);
      cluster = getNext(cluster, 0x200);
      te = (tam > 512) ? 512 : tam;
      write(fo, buffer, te);
      tam -= 512;
    } while (tam > 0);
    close(fo);
  }

  int MBR(char *base)
  {
    int res = 1;
    int i = 0;

    if (base[510] != 0x55 && base[511] != 0xAA)
      res = 0;

    while (res && i < 4)
    {
      int p = 0x1BE + i * 16;
      if (!(base[p] == 0 || base[p] == 0x80))
        res = 0;
      i++;
    }
    return res;
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
  //Funciono
  /*
  int partitions(){
   int a=0, a2=0;
  if(MBR(map)){
   printf("\nEs MBS\n\n");

   for(int i=0; i<=3; i++)
   {
    int h =(byte)map[0x1BE + 1 + (i*16)];
    if(h!=0){
    printf("CHS Paricion %d\n", i+1);
    printf("Head: %02x\n", h);
    int s = map[0x1BE +  2 + (i*16)] & 0x3F;
    int c = map[0x1BE + 2 + (i*16)] & 0xC0;
     c<<=2;
    printf("Sector: %02x\n", s);
    c |= map[0x1BE + 3 + (i*16)];
    printf("Cylinder: %02x\n",c);
    a= ((c*255 + h)*63 +(s-1))*512;
    printf("Partition Start: %02x\n\n", a);
    
    int h2 =(byte)map[a+0x1BE + 1];
    if(h2!=0)
    {
    printf("Partition Extended\n");
    printf("Head: %02x\n", h2);
    int s2 = map[a+0x1BE + 2] & 0x3F;
    int c2 = map[a+ 0x1BE + 2] & 0xC0;
     c2<<=2;
    printf("Sector: %02x\n", s2);
    c2 |= map[a+ 0x1BE + 3];
    printf("Cylinder: %02x\n",c2);
    a2= ((c2*255 + h2)*63 +(s2-1))*512;
    printf("Partition Extended Start: %02x\n\n", a2);
    leeChar(a2);
    }else{
      leeChar(a);
    }
    }
   }
  }else{
    printf("NO ES MBS\n");
  }
  */
}
