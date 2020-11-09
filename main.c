#include "generalFunctions.h"

int main(int argc, char *argv[])
{
  char imageName[25];
  int valida = 0;

  if (argc < 1)
  {
    valida = 1;
    validations(valida);
  }
  else
  {
    if (argc > 2)
    {
      valida = 2;
      validations(valida);
    }
    else
    {
      if ((strcmp(argv[1], "-h") == 0) || (strcmp(argv[1], "ayuda") == 0))
      {
        valida = 4;
        validations(valida);
      }
      else
      {
        if (strstr(argv[1], ".img") == NULL)
        {
          valida = 3;
          validations(valida);
        }
        else
        {
          strcpy(imageName, argv[1]);
          abre(imageName);
        }
      }
    }
  }
}
