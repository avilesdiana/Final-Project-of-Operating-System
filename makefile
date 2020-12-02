default:main.o generalFunctions.o validations.o
	gcc main.o generalFunctions.o validations.o hexEditor.o practica3.o -o proyectoFinal -lncurses
main.o:main.c
	gcc -c main.c
generalFunctions.o:generalFunctions.c
	gcc -c generalFunctions.c
validations.o:validations.c
	gcc -c validations.c
practica3.o:practica3.c
	gcc -c practica3.c
hexEditor.o:hexEditor.c
	gcc -c hexEditor.c
todo:
	gcc main.c generalFunctions.c validations.c hexEditor.c practica3.c -o proyectoFinal -lncurses
clean:
	rm *.o
