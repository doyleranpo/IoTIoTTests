#include "esp.h"

int main(){
    int fd,c;
    fd=serialOpen("/dev/ttyS1",115200);
    initESP(fd);
    connectToWiFi("iotiot1","Router4me",fd);
    establishServer(fd);
    char buf[10];
    printf("\nEnter a string to send\n");
    scanf("%s",buf);
    getIP(fd);
    printf("Do you wish to send data\n",&c);
    scanf("%d",&c);
    if (c==1)
        sendOverServer(fd,buf);
    else 
        return 0;
}
