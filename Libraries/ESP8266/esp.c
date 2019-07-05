/**
***************************************************************************************************
* @file		esp.c
* @author	Ayush Srivastava
* @brief	The ESP8266 is a low-cost Wi-Fi microchip with full TCP/IP 
*			stack and microcontroller capability
***************************************************************************************************
						#### Functions Description ####
===================================================================================================
(#)void setupShunyaInterfaces() 	:Initialize the shunya interface
(#)void delay(ms)					:Produces a delay equal to the value given(in ms) 
(#)serialPrintf()					:Prints value to serial buffer
(#)char serialGetchar()				:Returns the character present in the serial buffer
****************************************************************************************************
						####		NOTE 			####
===================================================================================================
*	In the above library all responses which are printed on the terminal window are responses to AT 
*	commands each response follows a particular pattern for eg. +CIPSTA: <ip> this can be a little 
*	difficult to understand if the user is not familiar with AT commands and their respective 
*	responses.
*	Thus to simplify this the tokens need to be seperated into different strings which will allow 
*	us to use them as certain checking variables and add more functions like smartConfigStatus(),
*	RSSI(), BSSID(), etc.
*	Issue: The number of parameters in every response is different thus no common function 
*	can be written
****************************************************************************************************
						#### 		Reference		####
===================================================================================================
Links to refer while trying to add functions to library
https://arduino-esp8266.readthedocs.io/en/latest/esp8266wifi/station-class.html
https://www.espressif.com/sites/default/files/documentation/4a-esp8266_at_instruction_set_en.pdf
https://arduino-esp8266.readthedocs.io/en/latest/libraries.html#wifi-esp8266wifi-library

****************************************************************************************************
						#### Prototype to seperate string ####
===================================================================================================
*	Trial code for seperating out strings from responses by ESP8266
*	WIP
*	Open to suggestions 
	// while(strncmp(temp,"+",1)!=0)
	// {
	// 	while(x = serialGetChar(fd))
	// 	{
	// 		if(x == '\n') break;
	// 		temp[i] == x;
	// 		i++
	// 	}
	// 	temp[i] = '\0';
	// }
	// while(x = serialGetChar(fd))
	// {
	// 	sscanf(buf,"%s %s %s %s %s",ip,netmask,gateway,mac,bssid) //Example
	// }
****************************************************************************************************
*/

#include "esp.h"

// Loop control and Display Functions
int checkOk(int fd){
	char x;
    char buf[100];
	int i = 0,res;
	while(x = serialGetchar(fd)) {
		if(x == '\n') break;	    
		buf[i] = x;
		i++;
        }
	buf[i]='\0';
	delay(1000);
	res = strncmp(buf,"OK",2);
	if(res==0){
		return 1;
	} else {
			dispDetails(buf);
			delay(1000);
			return 0;
	}
}

void dispDetails(char *buf){
    printf("%s\n",buf);
}

// ESP8266 Initial Configuration and setup functions

void initESP(int fd, int choice){
    
    serialPrintf(fd,"AT+CWMODE=%d\r\n",choice);
    delay(500);
	while(checkOk(fd)==0);
}

//Statiion Configuration functions. 
//ESP8266 Station Mode

void connectToWiFi(char *ssid, char *pass, int fd){
    serialPrintf(fd,"AT+CWJAP=\"%s\",\"%s\"\r\n",ssid,pass);
    while(checkOk(fd)==0);
    delay(10);
}

void getIP(int fd){
	serialPrintf(fd,"AT+CIFSR\r\n");
	while(checkOk(fd)==0);
	delay(10);
}

void disconnectWiFi(int fd){
	serialPrintf(fd,"AT+CWQAP\r\n");
	while (checkOk(fd) == 0);
	delay(10);
}

void isWiFiConnected(int fd){
	serialPrintf(fd,"AT+CIPSTATUS\r\n");
	while(checkOk(fd) == 0);
	delay(10);	
	printf("<stat>: status of the ESP8266 Station interface.\n\t
				‣2: The ESP8266 Station is connected to an AP and its IP is obtained.\n\t 
				‣3: The ESP8266 Station has created a TCP or UDP transmission.\n\t 
				‣4: The TCP or UDP transmission of ESP8266 Station is disconnected.\n\t 
				‣5: The ESP8266 Station does NOT connect to an AP. \n
			•<link ID>: ID of the connection (0~4), used for multiple connections. \n
			•<type>: string parameter, "TCP" or "UDP". \n
			•<remote IP>: string parameter indicating the remote IP address. \n
			•<remote port>: the remote port number.\n 
			•<local port>: ESP8266 local port number.\n 
			•<tetype>: ‣0: ESP8266 runs as a client. \n\t
						‣1: ESP8266 runs as a server\n");
}

void setAutoConnect(int fd, int choice){
	serialPrintf(fd,"AT+CWAUTOCONN=%d\r\n",choice);
	while (checkOk(fd) == 0);
	delay(10);
}

void getMACAddress(int fd){
	serialPrintf(fd,"AT+CIPSTAMAC?\r\n");
	while( checkOk(fd) == 0;)
}

void getMACAddress(int fd, char *mac){
	serialPrintf(fd,"AT+CIPSTAMAC=%s\r\n",mac);
	while( checkOk(fd) == 0;)
}

void beginSmartConfig(int fd){
	serialPrintf(fd,"AT+CWSTARTSMART=3\r\n");
	while (checkOk(fd) == 0);
	delay(10);
}

void stopSmartConfig(int fd){
	serialPrintf(fd,"AT+CWSTOPSMART\r\n");
	while(checkOk(fd) == 0);
	delay(10);
}

void connectWPS(int fd,int choice){
	serialPrintf(fd,"AT+WPS=%d\r\n",choice);
	while(checkOk(fd)==0);
	delay(10);
}

void getHostname(int fd){
	serialPrintf(fd,"AT+CWHOSTNAME?\r\n");
	while( checkOk(fd) == 0);
	delay(10);
}

void getHostname(int fd, char* host){
	serialPrintf(fd,"AT+CWHOSTNAME=%s\r\n",host);
	while( checkOk(fd) == 0);
	delay(10);
}

void dnsL(int fd, char* dns){
	serialPrintf(fd,"AT+CIPDOMAIN=\"%s\"",dns);
	while(checkOk(fd) == 0);
	delay(10);
}

void connectionDetails(int fd){
	serialPrintf(fd,"AT+CIPSTA?\r\n");
	while (checkOk(fd) == 0);
	delay(10);
}

//Soft Access point configuration functions
//ESP8266 Soft Access Point Mode

void softAPinit(char *ssid, char *pwd, int channel, int enc, int max_con, int broadcast){
	serialPrintf(fd,"AT+CWSAP=\"%s\",\"%s\",%d,%d,%d,%d",ssid,pwd,enc,max_con,broadcast);
	while(checkOk(fd) == 0);
	delay(10);
}

void softAPconfig(int fd, char* ip, char* gateway, char* subnet){
	serialPrintf(fd,"AT+CIPAP=\"%s\",\"%s\",\"%s\"\r\n",ip,gateway,subnet);
	while(checkOk(fd) == 0);
	delay(10);
}

void softAPIP(int fd){
	serialPrintf(fd,"AT+CIPAP?\r\n");
	while (checkOk(fd) == 0);
	delay(10);
}

void softAPgetStation(int fd){
	serialPrintf(fd,"AT+CWLIF\r\n");
	while(checkOk == 0);
	delay(10);
}

void softAPmacAddress(int fd){
	serialPrintf(fd,"AT+CIPAPMAC?\r\n");
	while (checkOk(fd) == 0);
	delay(10);	 
}

void softAPmacAddress(int fd, char* mac){
	serialPrintf(fd,"AT+CIPAPMAC=\"%s\"",mac);
	while(checkOk(fd) == 0);
	delay(10);
}

//Server and client configuration functions
// ESP8266 Server and Client

void establishServer(int fd){
	serialPrintf(fd,"AT+CWMODE=3\r\n");
	while (checkOk(fd)==0);
	delay(10);
	serialPrintf(fd,"AT+CIPMUX=1\r\n");
	while (checkOk(fd)==0);
	delay(10);
	serialPrintf(fd,"AT+CIPSERVER=1,8080\r\n");
	while(checkOk(fd)==0);
	delay(10);	
}

void sendOverServer(int fd, char *buf){
	int n= strlen(buf),i;
	char temp[100],x;
	serialPrintf(fd,"AT+CIPSEND=0,%d\r\n",n);
	delay(10);
	while(strncmp(temp,">",1)==1){
		while(x = serialGetchar(fd)){
			if(x == '\n') break;	    
			temp[i] = x;
			i++;
		}
		temp[i]='\0';
	}
	serialPrintf(fd,"%s",buf);
	delay(10);
	serialPrintf(fd,"AT+CIPCLOSE=0\r\n");
	while (checkOk(fd)==0);
}
