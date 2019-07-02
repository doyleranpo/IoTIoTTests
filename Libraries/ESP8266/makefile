example.o: example.c esp.h
	gcc -c -g example.c
esp.o: esp.c esp.h
	gcc -c -g esp.c
example: example.o esp.o
	gcc -o example example.o esp.o -l shunyaInterfaces

