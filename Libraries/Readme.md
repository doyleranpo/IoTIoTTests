## Sensor Integration (Arduino 37 in 1 Sensor Kit)
___

Arduino 37 in 1 kit consists of various sensors.
These sensors have been exported to work with ShunyaOS on Raspberry Pi
More information on these sensors can be found [here](https://www.instructables.com/id/Arduino-37-in-1-Sensors-Kit-Explained/) 

**The library consists of the following so far**

- [x] Light Blocking Sensor 
- [x] Avoidance Sensor
- [ ] Ultrasonic Sensor
- [x] Heartbeat Sensor
- [x] Small Sound Sensor
- [x] Reed Switch Sensor
- [ ] Joystick
- [x] Tilt Switch
- [x] Big Sound Sensor
- [ ] IR Receiver
- [x] Touch Sensor
- [ ] PIR Sensor
- [x] Ball Switch 
- [ ] Alcohol Sensor
- [ ] Temperature and Humidity sensor
- [ ] Digital Temperature Sensor
- [x] ESP8266 (WIP)
### Sensor Configuration 
___
```c
void initReedSwitch(int);
void initTiltSwitch(int);
void initReadSound(int);
void initLightBlocking(int);
void initTouchSensor(int);
void initBalSwitch(int);
void initHeartbeat(int);
void initUltra(int, int);
```

### Reading Raw Data from the sensor
___
```c
int readReedSwitch(int);
int readLightBlocking(int);
int readSound(int);
int readTouchSensor(int);
int readTiltSensor(int);
int readHeartbeat(int);
int readUltraSonic();
int readAvoidanceSensor(int);
```

### Computing/Calculating values from Raw Data
___
```c
int readPulse(int);
int ifCompare(int);
```


