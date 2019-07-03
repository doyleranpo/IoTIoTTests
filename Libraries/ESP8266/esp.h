#include <stdio.h>
#include <stdlib.h>
#include <shunyaInterfaces.h>
#include <Serial.h>
#include <string.h>

int checkOk(int);
void dispDetails(char*);
void initESP(int, int);
void connectToWiFi(char*, char*, int);
void establishServer(int);
void sendOverServer(int , char *);
void getIP(int);
void disconnectWiFi(int);
void isWiFiConnected(int);
void setAutoConnect(int , int);
void getMACAddress(int);
void getMACAddress(int , char *);
void beginSmartConfig(int);
void stopSmartConfig(int);
void connectWPS(int,int);
void getHostname(int);
void getHostname(int, char*);
void dnsL(int , char*);
void connectionDetails(int);
