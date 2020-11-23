int MBR(char *base){
  int res = 1; //asumimos verdad
  int i=0;  
  //checa firma
  if(base[510] != 0x55 && base[511] != 0xAA) res=0;
  
  //Checa que las particiones sean validas
  while(res && i<4){
    int p = 0x1BE + i*16;
    if(!(base[p] == 0) || base[p] == 0x80)) res=0;
    i++
    }
   return res;
  }
