main: arbol.o main.o
	cc arbol.o main.o -o main
arbol.o: arbol.c arbol.h
	cc -c arbol.c
main.o: main.c arbol.h
	cc -c main.c