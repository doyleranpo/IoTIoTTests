example.o: example.c HC-SR04.h
	gcc -c -g example.c
HC-SR04.o: HC-SR04.c HC-SR04.h
	gcc -c -g HC-SR04.c
example: example.o HC-SR04.o
	gcc -o example example.o HC-SR04.o -l shunyaInterfaces

