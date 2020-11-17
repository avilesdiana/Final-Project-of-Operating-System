default:main.o generalFunctions.o validations.o
	gcc main.o generalFunctions.o validations.o -o proyectoFinal -lncurses
main.o:main.c
	gcc -c main.c
generalFunctions.o:generalFunctions.c
	gcc -c generalFunctions.c
validations.o:validations.c
	gcc -c validations.c
todo:
	gcc main.c generalFunctions.c validations.c -o proyectoFinal -lncurses
clean:
	rm *.o
