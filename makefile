default:main.o operaciones.o
	gcc main.o operaciones.o -o proyectoFinal
main:main.c
	gcc -c main.c
operaciones.o:operaciones.c
	gcc -c operaciones.c
todo:
	gcc main.c operaciones.c -o proyectoFinal
clean:
	rm *.o
