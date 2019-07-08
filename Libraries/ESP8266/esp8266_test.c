#include "esp8266.h"

int main(){
    shunyaInterfacesSetup();
    int fd,c;
    char passw[16];
    char wssid[20];
    fd=serialOpen("/dev/ttyS1",115200);
    initESP(fd, 1);
    printf("Enter SSID\n");
    scanf("%s",wssid);
    printf("Enter password\n")
    scanf("%s",passw);
    connectToWiFi(wssid,passw,fd);
    delay(100);
    printf("MAC Address is\n");
    getMACAddress(fd);
    delay(100);
    printf("Connection Summary\n");
    connectionDetails(fd);
    delay(100);
    printf("Disconnecting WiFi\n");
    disconnectWiFi(fd);
}
