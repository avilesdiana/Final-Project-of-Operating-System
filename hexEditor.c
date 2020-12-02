//
//  hexEditor.c
//  SO
//
//  Created by Eduardo Pérez, Diana Áviles y Manuel Tijerina on 05/10/20.
//

#include "lib.h"

/* Variable global para mejor legibilidad */
int fd; // Archivo a leer
int fs;

char *hazLinea(char *base, int dir) {
    char linea[100]; // La linea es mas pequeña
    int o=0;
    // Muestra 16 caracteres por cada linea
    o += sprintf(linea,"%08x ",dir); // offset en hexadecimal
    for(int i=0; i < 4; i++) {
        unsigned char a,b,c,d;
        a = base[dir+4*i+0];
        b = base[dir+4*i+1];
        c = base[dir+4*i+2];
        d = base[dir+4*i+3];
        o += sprintf(&linea[o],"%02x %02x %02x %02x ", a, b, c, d);
    }
    for(int i=0; i < 16; i++) {
        if (isprint(base[dir+i])) {
            o += sprintf(&linea[o],"%c",base[dir+i]);
        }
        else {
            o += sprintf(&linea[o],".");
        }
    }
    sprintf(&linea[o],"\n");

    return(strdup(linea));
}

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

extern int leeChar(void);

void pantalla(char *map, int base){
    for(int i= 0; i<25; i++) {
        // Haz linea, base y offset
        char *l = hazLinea(map, base+i*16);
        move(i,0);
        addstr(l);
    }
    refresh();
}

int edita(char *map, long fs)
{
    int c;
    int x=0;
    int y=0;
    int px=0;
    int offset=0;
    
    pantalla(map, 0);
    
    do{
        px=(x<16)? x*3:32+x;
        move(0+y,9+px);
        c=leeChar();
        switch(c){
            case KEY_UP: //0x1B5B41
                if(y>0){
                    y-=1;
                }else{
                    if(offset>0){
                        offset -= 1;
                        clear();
                        pantalla(map, offset*16);
                    }
                }
                break;
            case KEY_DOWN: //0x1B5B42
                if(y<24) {
                    y+=1;
                } else {
                    if(fs > (25*16) && offset < (fs/16)){
                        offset +=1;
                    } else {
                        y=0;
                        offset=0;
                        clear();
                    }
                    pantalla(map,offset*16);
                }
                break;
            case KEY_RIGHT: //0x1B5B43
                if(x<32){
                    x+=1;
                }
                break;
            case KEY_LEFT: //0x1B5B44
                if(x>0){
                    x-=1;
                }
                break;
            case KEY_DC:
                memcpy(&map[(offset+y)*16+x],&map[(offset+y)*16+x+1],fs-(offset+y)*16+x);
                fs-=1;
                pantalla(map,offset*16);
                break;
           /*default:
                //Depende de donde estemos
                if(x<16){
                    //Debe ser digito o 'a','b'...'f'
                    char n=tolower(c);
                    if((n>='0' && n<='9') || (n>='a' && n<='f')){
                        char c1=leeChar();
                        char n1=tolower(c1);
                        if((n1>='0' && n1<='9') || (n1>='a' && n1<='f')){
                            char hd[3];
                            hd[0]=n;
                            hd[1]=n1;
                            hd[2]=0;
                            long l=strtol(hd, NULL, 16);
                            map[(offset+y)*16+x]=l;
                            pantalla(map,offset*16);
                        } else {
                            move(28, 5);
                            addstr("Entrada erronea");
                        }
                    } else {
                        if(isprint(c)){
                            map[(offset+y)*16+x]=c;
                        }
                    }
                }*/
        }
    } while(c != 24);
   return 0;
}
