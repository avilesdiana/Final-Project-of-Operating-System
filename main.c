#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <ctype.h>
#include <string.h>
#include <curses.h>
#include <sys/stat.h>
#include <sys/mman.h>



typedef struct imageInformation {
    int sectorSize;
    int numberOfSectorsPerCluster;
    int reservedSectors;
    int numberOfCopiesofFat;
    int numberOfEntriesRootDirectory;
    int numberOfDiskSectors;
    int diskSectorsSize;
    int fatSize;
    int volumenLabel;
    int idSistema;

  } imageInformation;




int fd; // Archivo a leer
int fs;
char *map;

char *mapFile(char *filePath) {
    /* Abre archivo */
    fd = open(filePath, O_RDWR);
    if (fd == -1) {
        perror("Error abriendo el archivo");
        return(NULL);
    }

    /* Mapea archivo */
    struct stat st;
    fstat(fd,&st);
    fs = st.st_size;

    char *map = mmap(0, fs+200, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
    if (map == MAP_FAILED) {
        close(fd);
        perror("Error mapeando el archivo");
        return(NULL);
    }
  return map;
}

int getNext(int cluster, int base) {
	// Para FAT12
	int offset = cluster + cluster/2;
	int flag = cluster % 2; // Nos dice si en la parte baja o alta

	unsigned char b1,b2;
    printf("%x", map);
	b1 = map[base+offset];
	printf("%02x:",b1);
	b2 = map[base+offset+1];
	printf("%02x\n",b2);
	int result = b1 | b2<<8; // Los bits mas significativos van al final

	if(flag) {
		result >>= 4;
	}
	else {
		result &= 0xfff;
	}

	printf("%04x\n",result);
  
	return result;
}

void pruebas(){
    int d;
    for(int i = 0; i <10; i++){
        d = getNext(i,0x200);
        printf("%03x  %d\n",d,d);
    }
}


void abre(char *filename){
    map = mapFile(filename);
    if (map == NULL) {
      exit(EXIT_FAILURE);
    }

     pruebas();

    if (munmap(map, fs) == -1) {
      perror("Error al desmapear");
    }
    close(fs);
    
}





//
int main(){
    abre("allinone.img");
}