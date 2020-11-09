default:main.o generalFunctions.o validations.o
	gcc main.o generalFunctions.o validations.o -o proyectoFinal
main.o:main.c
	gcc -c main.c
generalFunctions.o:generalFunctions.c
	gcc -c generalFunctions.c
validations.o:validations.c
	gcc -c validations.c
todo:
	gcc main.c generalFunctions.c validations.c -o proyectoFinal
clean:
	rm *.o
