example.o: example.c ADXL.h
	gcc -c -g example.c
pcf8591.o: pcf8591.c pcf8591.h
	gcc -c -g pcf8591.c
ADXL.o: ADXL.c ADXL.h
	gcc -c -g ADXL.c
example: example.o ADXL.o
	gcc -o example example.o ADXL.o pcf8591.o -l shunyaInterfaces

