#include <stdio.h>
#include <stdlib.h>
#include <shunyaInterfaces.h>

//init functions
void initReedSwitch(int);
void initTiltSwitch(int);
void initReadSound(int);
void initLightBlocking(int);
void initTouchSensor(int);
void initBalSwitch(int);
void initHeartbeat(int);
void initUltra(int, int);

//readRaw functions
int readReedSwitch(int);
int readLightBlocking(int);
int readSound(int);
int readTouchSensor(int);
int readTiltSensor(int);
int readHeartbeat(int);
int readUltraSonic();
int readAvoidanceSensor(int);

//Compute functions
int readPulse(int);
int ifCompare(int);
