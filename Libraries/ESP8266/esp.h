#include <stdio.h>
#include <stdlib.h>
#include <shunyaInterfaces.h>
#include <Serial.h>
#include <string.h>

int checkOk(int);
void dispDetails(char*);
void initESP(int);
void connectToWiFi(char*, char*, int);
void establishServer(int);
void sendOverServer(int , char *);
void getIP(int);