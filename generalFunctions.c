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
typedef struct imageInformation {
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
char *mapFile(char *filePath, long *fs) {
  /* Abre archivo */
  fd = open(filePath, O_RDWR);
  if (fd == -1) {
    system("clear");
    perror("\nError abriendo el archivo\n");
    getchar();
    return (NULL);
  }

  /* Mapea archivo */
  struct stat st;
  fstat(fd, &st);
  *fs = st.st_size;

  char *map = mmap(0, *fs, PROT_READ, MAP_SHARED, fd, 0);
  if (map == MAP_FAILED) {
    close(fd);
    system("clear");
    perror("\nError mapeando el archivo\n");
    getchar();
    return (NULL);
  }
  return map;
}

int leeChar(){
  int chars[5];
  int ch,i=0;
  nodelay(stdscr,TRUE);
  while((ch=getch()) == ERR); //Espera activa
  ungetch(ch);
  while((ch=getch()) != ERR){
    chars[i++]=ch;
  }
  //Convierte a número con todo lo leído
  int res=0;
  for(int j=0;j<i;j++){
    res<<=8;
    res |= chars[j];
  }
}

//Función que 
int getNext(int cluster, int base) {
  // Para FAT12
  int offset = cluster + cluster / 2;
  int flag = cluster % 2; // Nos dice si en la parte baja o alta

  unsigned char b1, b2;
  b1 = map[base + offset];
  printf("%02x:", b1);
  b2 = map[base + offset + 1];
  printf("%02x\n", b2);
  int result = b1 | b2 << 8; // Los bits mas significativos van al final

  if (flag) {
    result >>= 4;
  } else {
    result &= 0xfff;
  }

  printf("%04x\n", result);

  return result;
}

int getNext16(int cluster, int base){
  //Para FAT 16
  int offset=cluster*2;
  
  unsigned short int b;
  b=*(short int *)&map[base + offset];
  int result = b;
  
  return result;
}

int getNext32(int cluster, int base){
  //Para FAT 32
  int offset=cluster*4;
  
  unsigned int b;
  b=*(int *)&map[base + offset];
  int result = b;
  
  return result;
}

void readCluster(int cluster, char *buffer, int inicio, int size){
    int offset = cluster*size*512;
    memcpy(buffer, &map[inicio+offset],size*512);
}

void leeArchivo(char *nombre, long tam, int cluster){
    int fo = open(nombre, O_WRONLY | O_CREAT, 0x755);
    char buffer[512];
    int te;
    do{
        readCluster(cluster,buffer,0x3E00);
        cluster = getNext(cluster,0x200);
        te=(tam>512) ? 512 : tam;
        write(fo,buffer,te);
        tam -= 512;
    } while(tam>0);
    close(fo);
}

void copiaMem(char *map, int cluster, long tam, int inicio, int size){
  do{
    readCluster(cluster, map, inicio, size);
    cluster=getNext(cluster, 0x200);
    tam-=512;
    map+=512;
  } while (tam>0);
}
 
void copiaMem16(char *map, int cluster, long tam, int inicio, int size){
  do{
    readCluster(cluster, map, inicio, size);
    cluster=getNext16(cluster,0x1000);
    tam-=512;
    map+=512;
  } while (tam>0);
}

int MBR(char *base){
    int res=1;
    int i=0;
    
    //Checa firma
    if(base[510] != 0x55 && base[511] != 0xAA){
        res=0;
    }
    
    //Checa que haya al menos una partición
    if(res && base[0x1BF]==0 && base[0x1C0]==0 && base[0x1C1]==0){
        res=0;
    }
    
    while(res && i<4){
        int p = 0x1BE + i*16;
        if(!(base[p] == 0 || base[p] == 0x80)) res=0;
        i++;
    }
    return res;
}

// Función que Abre el archivo
void leeInfo(char *map){
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
  printf("ID Sistema:                      %s \n ", imageInfo.idSistema);

  imageInfo.rootDirectoryAddress = (*imageInfo.reservedSectors + (imageInfo.numberOfCopiesofFat * *imageInfo.fatSize)) * *imageInfo.sectorSize;
  imageInfo.addressImageInfoBegins = imageInfo.rootDirectoryAddress + (*imageInfo.numberOfEntriesRootDirectory * 32);

  printf("\nDireccion del directorio raiz: 0x%04x\n", imageInfo.rootDirectoryAddress);
  printf("\nDireccion donde comienza la información de archivos en la imagen: 0x%04x\n", imageInfo.addressImageInfoBegins);

  printf("\n\t\t[Press enter to continue]\n\n");
  getchar();
  
    //Es MBR
       if(MBR(map)){
         attron(A_REVERSE);
         mvprintw(4,40,"Es MBR");
         attroff(A_REVERSE);
         
         leeChar();
         clear();
         
         move(4,5);
         addstr("Partición C  H  S Sector");
         for(int i=0;i<4;i++){
           int h=(unsigned char)map[0x1BE + i*16];
           mvprintw(5+i,20,"%d",h);
           int s=map[0x1BE + i*16 + 2] & 0x3F;
           int c=map[0x1BE + i*16 + 2] & 0xC0;
           c<<=2;
           mvprintw(5+i,25,"%d",s);
           c |= map[0x1BE + i*16 + 3];
           mvprintw(5+i,15,"%d",c);
           int sec=(c*255 + h)*63 +(s-1);
           mvprintw(5+i,30,"%d",sec);
         }
       }
}

void leeDirectorio(int d){
    clear();
    char *dir = &map[d];
    char nombre[14];
    int tipo;
    int cluster;
    unsigned int tam;
    mvprintw(4,5,"Nombre");
    mvprintw(4,20,"Tipo");
    mvprintw(4,5,"Cluster");
    mvprintw(4,5,"Tamaño");
    int i=0, j;
    while(i<25 && dir[i*32] != 0) j=0;
    strncpy(nombre, &dir[i*32],13);
    while(nombre[j] != ' ' && j<8) j++;
    strncpy (&nombre[j+1],&dir[8 + i*32],3);
    nombre[j+4]=0;
    tipo=dir[0xb + i*32];
    cluster=*((short int*) &dir[0x1a + i*32]);
    int o = 0x1c + i*32;
    tam=*(unsigned int*) &dir[0x1c + 32];
    mvprintw(5+i,5,"%s",nombre);
    mvprintw(5+i,20,"%d",tipo);
    mvprintw(5+i,25,"%d",cluster);
    mvprintw(5+i,35,"%ld",tam);
    i++;
}

// Función que
void pruebas(){
    int d;
    d=leeChar();
        
    if(d=='1'){
        leeInfo(map);
        leeChar();
        leeDirectorio(0x2600);
        leeChar();
        char *archmem=malloc(1024);
        copiaMem(archmem,3,830,0x3E00,1);
        edita(archmem,830);
    
    }else{
        leeInfo(map);
        leeChar();
        leeInfo(&map[0x100000]);
        leeChar();
        leeDirectorio(0x141000);
        leeChar();
        map=&map[0x100000];
        char *archmem=malloc(4096);
        copiaMem16(archmem,2,302,0x44000,8);
        edita(archmem,302);
    }
}

// Función que Abre el archivo
int abre(char *filename) {
    long fs;
    char *map = mapFile(filename,&fs);
    if (map == NULL) {
        exit(EXIT_FAILURE);
    }
    
    pruebas();
    
    if(munmap(map,fd)== -1){
        system("clear");
        perror("Error al desmapear");
    }
    close(fd);
    return 0;
}
