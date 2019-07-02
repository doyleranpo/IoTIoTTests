#include <stdio.h>
#include <stdlib.h>
#include <shunyaInterfaces.h>
#include <Serial.h>
#include <string.h>
char buf[100];
int res;
int checkOk(int fd){
	char x;
	int i = 0;
	while(x = serialGetchar(fd)) {
		if(x == '\n') break;	    
		buf[i] = x;
		i++;
        }
	buf[i]='\0';
	delay(1000);
	res = strncmp(buf,"OK",2);
	printf("Test %s\n",buf);
	if(res==0){
		printf("%s\n",buf);
		printf("Line 27\n");
		return 1;
	} else {
			printf("%s\n",buf);
			printf("line 33\n");
			delay(1000);
			return 0;
	}
}
int main(void) {
    shunyaInterfacesSetup();
    int fd=serialOpen("/dev/ttyS1",115200);
    int i = 0;
    char x;
    serialPrintf(fd,"AT+GMR\r\n");
    delay(500);
	while(checkOk(fd)==0);
    serialPrintf(fd,"AT+CWMODE=1\r\n");
    delay(10);
    while(checkOk(fd)==0);
    serialPrintf(fd,"AT+CWLAP\r\n");
    delay(10);
    while(checkOk(fd)==0);
    serialPrintf(fd,"AT+CWJAP="iotiot2","Empathy4ever"\r\n");
    while(checkOk(fd)==0);
    delay(10);
    serialPrintf(fd,"AT+CIFSR\r\n");
    delay(10);
    while(checkOk(fd)==0);
    return 0;
}
