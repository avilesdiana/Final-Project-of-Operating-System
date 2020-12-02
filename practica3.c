//
//  practica3.c
//  SO
//
//  Created by Eduardo Pérez, Diana Áviles y Manuel Tijerina on 10/10/20.
//

#include "lib.h"
#include "hexEditor.c"

int fd; //Maneja archivo
int fs;

struct s_dir{
    int tipo;
    char *nombre;
} res[128];

int leeDir(char *direc){
    DIR *dir = opendir(direc);
    
    struct dirent *dp;
    int i=0;
    while((dp=readdir(dir))!= NULL){
        res[i].tipo=dp->d_type;
        res[i].nombre=strdup(dp->d_name);
        i++;
    }
  for(int j=0; j<i; j++) {
     if (res[j].tipo == DT_DIR) {
        printf("D ");
     }
     else {
        printf("F ");
     }
     printf("%s\n",res[j].nombre);
  }
    closedir(dir);
    return i;
}

int leeChar() {
  int chars[5];
  int ch,i=0;
  nodelay(stdscr, TRUE);
  while((ch = getch()) == ERR); /* Espera activa */
  ungetch(ch);
  while((ch = getch()) != ERR) {
    chars[i++]=ch;
  }
  /* convierte a numero con todo lo leido */
  int res=0;
  for(int j=0;j<i;j++) {
    res <<=8;
    res |= chars[j];
  }
  return res;
}

void selecciona()
{
    char cwd[256];
    getcwd(cwd,256);
  
    int max=leeDir(cwd);
    int pos=0;
    int offset=0;
    int pan=20;
    int c;
   
    do {
      for (int j=0; j<pan && (j+offset)<max; j++) {
         if (j == pos) {
           attron(A_REVERSE);
         }
          char *nom=res[j+offset].nombre;
          move(5+j,4);
          addstr(nom);
          attroff(A_REVERSE);
      }
      move(5+pos,4);
      refresh();
      c = leeChar();
      switch(c) {
         case KEY_UP:
              if(pos>0){
                  pos -= 1;
              }
              else{
                  if(offset>0){
                      offset-=1;
                  }
                  else{
                      offset=(max>pan)?max-pan:0;
                      pos=(max>pan)?pan-1:max-1;
                  }
                  clear();
              }
              break;
          case KEY_DOWN:
              if (max<pan && pos==(max-1)){
                  pos=0;
                  clear();
              }
                  if(pos<pan){
                  pos+=1;
              }
                  else{
                  if(max>pan && offset < (max-pan)){
                      offset +=1;
                  }
                  else{
                      pos=0;
                      offset=0;
                      clear();
                  }
              }
            break;
          case 10:
              switch(res[pos+offset].tipo){
                  case DT_REG:
                      clear();
                      move(2,10);
                      printw("Elegiste %s",res[pos+offset].nombre);
                      getch();
                      edita(res[pos+offset].nombre);
                      break;
                  case DT_DIR:
                      if(strcmp("..", res[pos+offset].nombre) == 0){
                              //Cambia al papá
                              char *p=strrchr(cwd,'/');
                          if(p!=cwd){
                              *p=0;
                          }
                          else{
                              *(p+1)=0;
                          }
                              max=leeDir(cwd);
                              pos=0;
                              clear();
                      } else {
                          //Cambia a otro directorio
                          strncat(cwd,"/",255);
                          strncat(cwd,res[pos+offset].nombre,255);
                          max=leeDir(cwd);
                          pos=0;
                          clear();
                      }
                      break;
              }
              break;
                  default:
                  //Nothing
                  break;
      }
        move(2,10);
        printw("Estoy en posición %d: Offset %d: Lei %xd: Max %d",pos,offset,c,max);
        move(3,10);
        printw("Dir %s",cwd);
    } while (c != 24);
}

int main(){
    initscr();
    raw();
    noecho(); /* No muestres el caracter leido */
    keypad(stdscr, TRUE);
    
    selecciona();
    
    endwin();
    return 0;
}
