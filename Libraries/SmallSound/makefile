example.o: example.c LM393.h
	gcc -c -g example.c
LM393.o: LM393.c LM393.h
	gcc -c -g LM393.c
example: example.o LM393.o
	gcc -o example example.o LM393.o -l shunyaInterfaces

