#include <stdio.h>
#include <stdlib.h>
#include <shunyaInterfaces.h>
#include <Serial.h>
#include <string.h>

// Loop Control and Display
int checkOk(int);
void dispDetails(char*);

//Initialization
void initESP(int, int);
void connectToWiFi(char*, char*, int);
void softAPinit(int, char*, char*, int, int, int, int);
void softAPconfig(int, char*, char*, char*);

//Server Configuration
void establishServer(int);

//Sending Data over server
void sendOverServer(int , char *);

//Reading properties and values
void getIP(int);
void getHostname(int);
void isWiFiConnected(int);
void setAutoConnect(int , int);
void getMACAddress(int);
void connectionDetails(int);
void softAPgetStation(int);
void softAPmacAddress(int);
void softAPIP(int);

//WiFi Configurations
void beginSmartConfig(int);
void stopSmartConfig(int);
void connectWPS(int,int);
void alterMACAddress(int , char *);
void alterHostname(int, char*);
void dnsL(int , char*);
void softAPaltermacAddress(int, char*);
void disconnectWiFi(int);


