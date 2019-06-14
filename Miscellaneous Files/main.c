#include "ultra.h"

int main(){
	shunyaInterfacesSetup();
	initUltra(38,40);
	int x;
	while(1){
		x=readUltra();
		printf("%d \n",x);
		}
	}
