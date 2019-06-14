example.o: example.c LightBlocking.h
	gcc -c -g example.c
LightBlocking.o: LightBlocking.c LightBlocking.h
	gcc -c -g LightBlocking.c
example: example.o LightBlocking.o
	gcc -o example example.o LightBlocking.o -l shunyaInterfaces

