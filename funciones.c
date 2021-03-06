char* mapFile(char *filePath, long *fs){
  fd=open(filePath, O_RDONLY);
  if(fd==-1){
    perror("Error abriendo el archivo");
    return (NULL);
  }
  struct stat st;
  fstat(fd, &st);
  *fs = st.st_size;
  
  map=mmap(0, *fs, PROT_READ, MAP_SHARED);
  if(map==MAP_FAILED){
    close(fd);
    perror("Error mapeando el archivo");
    return (NULL);
  }
  return map;
}

int leeChar(){
  int chars[5];
  int ch, i=0;
  nodelay(stdscr, TRUE);
  while((ch=getch())==ERR)
    ;
  ungetch(ch);
  while((ch=getch())!=ERR){
    chars[i++]=ch;
  }
  int res=0;
  for(int j=0;j<i;j++){
    res <<=8;
    res |= chars[j];
  }
  return res;
}

int getNext(int cluster, int base){
  //Para FAT12
  int offset=cluster+cluster/2;
  int flag=cluster % 2; 
  
  unsigned char b1, b2;
  b1=map[base + offset];
  b2=map[base + offset + 1];
  printf("%02x:%02x\n",b2,b1);
  int result = b1|b2<<8;
  
  if(flag){
    result >>= 4;
  } else {
    result &= 0xfff;
  }
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
  int offset = cluster * size * 512;
  memcpy(buffer,&map[inicio+offset],size*512);
}

void leeArchivo(char *nombre, long tam, int cluster){
  int fo=open(nombre, O_WRONLY | O_CREA...); //no esta terminado
  char buffer[512];
  int te;
  do{
    readCluster(cluster, buffer, 0x3E00);
    cluster=getNext(cluster,0x200);
    te=(tam>512) ? 512 : tam;
    write(fo, buffer, te);
    tam -= 512;
  } while (tam>0);
  close (fo);
}

void leeArchivo2(char *nombre, long tam, int cluster){
int fo=open(nombre, O_WRONLY | O_CREA...); //no esta terminado
  char buffer[512];
  int te;
  do{
    readCluster(cluster, buffer, 0x3E00);
    cluster=getNext(cluster,0x200);
    te=(tam>512) ? 512 : tam;
    write(fo, buffer, te);
    tam -= 512;
  } while (tam>0);
  close (fo);
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

//void copiaMem32(char *map, int cluster, long tam, int inicio, int size)

  int MBR(char *base){
    int res = 1; //asumimos verdad
    int i=0;  
    
    //checa firma
    if(base[510] != 0x55 && base[511] != 0xAA) res=0;
  
    //Checa que haya al menos una partición
    if(res && base[0x1BF]==0 && base[0x1C0]==0 && base[0x1C1]==0) res=0;
    
    //Checa que las particiones sean validas
    while(res && i<4){
      int p = 0x1BE + i*16;
      if(!(base[p] == 0 || base[p] == 0x80)) res=0;
      i++
      }
   return res;
  }
  
  void leeInfo(char *map){
    clear();
    move(4,5);
    addstr("Informacion de Disco");
    short int *ss = (short int*)&map[11];
    mvprintw(6, 5, "Tamaño sector:%d\n", *ss);
    short int *sr = (short int*)&map[14];
    mvprintw(8, 5, "Sectores reservados:%d\n", *sr);
    short int *re = (short int*)&map[17];
    mvprintw(10, 5, "Entradas directorio:%d\n", *re);
    short int *tf = (short int*)&map[22];
    mvprintw(12, 5, "Tamaño FAT:%d\n", *tf);
    
    //De 4B
    int *ns=(int*)&map[32];
    mvprintw(14,5,"Numero de Sectores:%d\n",*ns);
    
    //De 1B
    int spc = map[13];
    mvprintw(16,5,"Numero de sectores por cluster:%d\n",spc);
    int nfat = map[16];
    mvprintw(18,5,"Numero de FAT:%d\n",nfat);
    
    //Cadenas
    char ID[9];
    char label[11];
    
    strncpy(ID, &map[54],9);
    ID[5]=0;
    strncpy(label, &map[43],10);
    label[11]=0;
    mvprintw(20,5,"ID:%s\n",ID);
    mvprintw(22,5,"Etiqueta:%s\n",label);
    
    //Cuentas
    int draiz=((*sr)+nfat*(*tf))*(*ss); //no esta terminado
    mvprintw(24,5,"Directorio raiz 0x%x\n",draiz);
    int inicio=((*sr)+nfat*(*tf))*(*ss); //no esta terminado
    mvprintw(26,5,"Inicio de datos 0x%x\n",inicio);
    
    //Es MBR
    if(MBR(map)){
      attron(A_REVERSE);
      mvprintw(4,40,"Es MBR");
      attrof(A_REVERSE);
      
      leeChar();
      clear();
      
      move(4,5);
      addstr("Partición C  H  S Sector");
      for(int i=0;i<4;i++){
        int h=(byte)map[0x1BE + i*16];
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
    mvprintw(5+i,35,"%ld",tamaño);
    i++;
  }
  
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

  int abre(char *filename){
    long fs;
    //Lee archivo
    char *map = mapFile(filename, &fs);
    if(map == NULL){
      exit(EXIT_FAILURE);
    }
    
    pruebas();
    
    if(munmap(map,fd) == -1){
      perror("Error al desmapear");
    }
    close(fd);
    return 0;
  }

  //int main
