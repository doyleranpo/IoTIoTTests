
//=========================================================================//
//                                                                         //
//  ## R30X Fingerprint Sensor Library Example 1 ##                        //
//                                                                         //
//  Filename : R30X_Fingerprint_Test.ino                                   //
//  Description : Arduino compatible test program for Fingerprint_VMA      //
//                library for R30X series fingerprint sensors.             //
//  Library version : 1.0                                                  //
//  Author : Vishnu M Aiea                                                 //
//  Src : https://github.com/vishnumaiea/R30X-Fingerprint-Sensor-Library   //
//  Author's website : https://www.vishnumaiea.in                          //
//  Initial release : IST 07:35 PM, 08-04-2019, Monday                     //
//  License : MIT                                                          //
//                                                                         //
//  Last modified : IST 11:44 PM, 08-04-2019, Monday                       //
//                                                                         //
//=========================================================================//

#include "fingerprint.h"



//R30X_Fingerprint fps = R30X_Fingerprint (&Serial2, 0x16161616, 0x16161616); //custom password and address


uint8_t enrollFinger(uint16_t location) {
  //enroll new fingerprint
  printf("\nEnrolling New Fingerprint");
  printf("\n-------------------------");

  if((location > 1000) || (location < 1)) { //if not in range (1-1000)
    printf("\n");
    printf("\nEnrolling failed.");
    printf("\nBad location.");
    printf("location = #");
    printf("\n%d",location);
    return 1;
  }

  delay(3000);
  printf("\n");
  printf("\n#1 Put your finger on the sensor.");
  printf("\n");
  delay(3000);
  uint8_t response = generateImage(); //scan the finger

  if(response != 0) {
    printf("\n#1 Scanning failed. Try again.");
  }
  else {
    printf("\n");
    response = generateCharacter(1);  //generate the character file from image and save to buffer 1

    if(response != 0) {
      printf("\n#1 Generation failed. Try again.");
    }
    else {
      delay(1000);
      printf("\n");
      printf("\n#2 Put your finger on the sensor.");
      delay(3000);
      printf("\n");
      response = generateImage(); //scan the finger fro second time

      if(response != 0) {
        printf("\n#2 Scanning failed. Try again.");
      }
      else {
        printf("\n");
        response = generateCharacter(2);  //generate the character file from image and save to buffer 2

        if(response != 0) {
          printf("\n#2 Generation failed. Try again.");
        }
        else {
          printf("\n");
          response = generateTemplate();  //combine the two buffers and generate a template

          if(response == 0) {
            printf("\n");
            response = saveTemplate(1, location); //save the template to the specified location in library

            if(response == 0) {
              printf("Fingerprint enrolled at ID #");
              printf("%d",location);
              printf("\n successfully.");
            }
          }
          else if(response == FPS_RESP_ENROLLMISMATCH) {
            printf("\nFingerprints do not belong to same finger. Try again.");
          }
        }
      }
    }
  }

  printf("\n");
}

//=========================================================================//
//Arduino setup function

int main() {
  init(0x16161616,0x16161616)
//   Serial.begin(115200);
  begin(115200);
  
  printf("\n");
  printf("\nR307 Fingerprint Test");
  printf("\n======================");
  printf("\n");

  //you need to verify the password before you can do anything else
  printf("\nVerifying password..");
  uint8_t response = verifyPassword(0x16161616);
  printf("\n");

  //this is optional
  printf("\nSetting new address..");
  response = setAddress(0x16161616);
  printf("\n");


//=========================================================================//
//infinite loop

while(1){
  uint8_t response = 0;
  char inputString[100];
  char commandString[10];
  char firstParam[10];
  char secondParam[10];
  char thirdParam[10];
  int i = 0;
  //send commands and parameters for each operation
  //items are separated by single whitespace
  //you can send up to 3 parameters
  if(serialDataAvail(fp.fd)) {  //monitor the serial interface
    while(x = serialGetchar(fp.fd)){
        if(x == '\n') break;	    
		inputString[i] = x;
		i++;
        }
        inputString = '\0';
    }
    //inputString = Serial.readString();  //read the contents of serial buffer as string
    printf("Command : ");
    printf("%s",inputString);
    printf("\n");

    //-------------------------------------------------------------------------//

    uint8_t posCount = 0;
    int indexOfSpace = 0;
    int temp = 0;
    while((strcspn(inputString," ") + 1) != -1) { //loop until all whitespace chars are found
      indexOfSpace = strcspn(inputString," ") + 1;  //get the position of first whitespace
      if(indexOfSpace != -1) {  //if a whitespace is found
        if(posCount == 0) //the first one will be command string
          strncpy(commandString,inputString,indexOfSpace-temp); //end char is exclusive
        if(posCount == 1) //second will be second param
          strncpy(firstParam,inputString,indexOfSpace-temp);
        if(posCount == 2) //and so on
          strncpy(secondParam, inputString, indexOfSpace-temp);
        else if(posCount == 3)
          strncpy(thirdParam, inputString, indexOfSpace-temp);
        strcpy(inputString, inputString+indexOfSpace+1);  //trim the input string
        posCount++;
        temp = indexOfSpace;
      }
    }

    //saves the last part of the string is no more whitespace is found
    if(posCount == 0) //if there's just the command
      strcpy(commandString, inputString);
    if(posCount == 1)
      strcpy(firstParam, inputString);
    if(posCount == 2)
      strcpy(secondParam, inputString);
    if(posCount == 3)
      strcpy(thirdParam, inputString);

    //-------------------------------------------------------------------------//
    //separate and print the received command and parameters

    printf("Command string = ");
    printf("%s",commandString);
    
    if(firstParam != "") {
      printf("First param = ");
      printf("%s",firstParam);
    }
    if(secondParam != "") {
      printf("Second param = ");
      printf("%s",secondParam);
    }
    if(thirdParam != "") {
      printf("Third param = ");
      printf("%s",thirdParam);
    }
    
    printf("\n");

    //-------------------------------------------------------------------------//
    //deletes all the templates in the library
    //this command has no parameters
    //eg. clrlib

    if(!strcpy(commandString,"clrlib")) {
      response = clearLibrary();
    }

    //-------------------------------------------------------------------------//
    //get templates count
    //eg. tmpcnt

    else if(!strcpy(commandString, "tmpcnt")) {
      printf("\nReading templates count..");
      response = getTemplateCount();
    }

    //-------------------------------------------------------------------------//
    //read system parameters
    //eg. readsys

    else if(commandString == "readsys") {
      response = readSysPara();
    }

    //-------------------------------------------------------------------------//
    //set data length
    //this command has a single parameter
    //value should be 32, 64, 128 or 256
    //eg. setdatlen 256

    else if(commandString == "setdatlen") {
      uint16_t length = atoi(firstParam);
      response = setDataLength(length);
    }

    //-------------------------------------------------------------------------//
    //capture and range search library
    //this command has three parameters
    //eg. capranser 3000 1 10

    else if(commandString == "capranser") {
      uint16_t timeOut = atoi(firstParam);  //first parameter in milliseconds
      uint16_t startLocation = atoi(secondParam); //second parameter
      uint16_t count = atoi(thirdParam);  //third parameter
      printf("\nCapture and range search fingerprint..");
      delay(1000);
      printf("\nPut your finger on the sensor..");
      delay(3000);
      response = captureAndRangeSearch(timeOut, startLocation, count);
    }

    //-------------------------------------------------------------------------//
    //capture and full search library
    //eg. capfulser

    else if(commandString == "capfulser") {
      printf("\nCapture and full search fingerprint..");
      delay(1000);
      printf("\nPut your finger on the sensor..");
      delay(3000);
      response = captureAndFullSearch();
    }

    //-------------------------------------------------------------------------//
    //enroll a new fingerprint
    //you need to scan the finger twice
    //follow the on-screen instructions
    //eg. enroll

    else if(commandString == "enroll") {
      uint16_t location = atoi(firstParam); //converts String object to int
      enrollFinger(location);
    }

    //-------------------------------------------------------------------------//
    //verify 4 byte password
    //password should be sent as hex string
    //eg. verpwd FF16FF16

    else if(commandString == "verpwd") {
      const char* hexString = firstParam; //convert String object to C-style string
      uint32_t password = strtol(hexString, NULL, 16);  //convert hex formatted C-style string to int value
      response = verifyPassword(password);
    }

    //-------------------------------------------------------------------------//
    //set 4 byte password sent in hex format
    //password should be sent as hex string
    //eg. setpwd FF16FF16

    else if(commandString == "setpwd") {
      const char* hexString = firstParam; //convert String object to C-style string
      uint32_t password = strtol(hexString, NULL, 16);  //convert hex formatted C-style string to int value
      response = setPassword(password);
    }

    //-------------------------------------------------------------------------//
    //set 4 byte address sent in hex format
    //address should be sent as hex string
    //eg. setaddr FF16FF16

    else if(commandString == "setaddr") {
      const char *hexString = firstParam; //convert String object to C-style string
      uint32_t address = strtol(hexString, NULL, 16);  //convert hex formatted C-style string to int value
      response = setAddress(address);
    }

    //-------------------------------------------------------------------------//
    //set baudrate
    //baudrate must be integer multiple of 96000. max is 115200
    //eg. setbaud 115200

    else if(commandString == "setbaud") {
      uint32_t baudrate = atoi(firstParam);
      response = setBaudrate(baudrate);
    }

    //-------------------------------------------------------------------------//
    //set security level
    //security level value must be 1-5
    //deafault is usually 2
    //eg. setseclvl 4

    else if(commandString == "setseclvl") {
      uint8_t level = atoi(firstParam);
      response = setSecurityLevel(level);
    }

    //-------------------------------------------------------------------------//
    //scan finger image and save to image buffer
    //eg. genimg

    else if(commandString == "genimg") {
      response = generateImage();
    }

    //-------------------------------------------------------------------------//
    //generate character file from image
    //buffer Id should be 1 or 2
    //eg. genchar 1

    else if(commandString == "genchar") {
      uint8_t bufferId = atoi(firstParam);
      response = generateCharacter(bufferId);
    }

    //-------------------------------------------------------------------------//
    //generate template from char buffers
    //template is the digital format of a fingerprint
    //generated template will be available on both buffers 1 and 2
    //eg. gentmp

    else if(commandString == "gentmp") {
      response = generateTemplate();
    }

    //-------------------------------------------------------------------------//
    //save template on buffer to library
    //buffer ID should be 1 or 2
    //location should be #1 - #10000 (don't send the "#" with command)
    //eg. savtmp 1 32

    else if(commandString == "savtmp") {
      uint8_t bufferId = atoi(firstParam);
      uint16_t location = atoi(secondParam);
      response = saveTemplate(bufferId, location);
    }

    //-------------------------------------------------------------------------//
    //load template from library to buffer 1 or 2
    //buffer ID should be 1 or 2
    //location should be #1 - #10000 (don't send the "#" with command)
    //eg. lodtmp 1 32

    else if(commandString == "lodtmp") {
      uint8_t bufferId = atoi(firstParam);
      uint16_t location = atoi(secondParam);
      response = loadTemplate(bufferId, location);
    }

    //-------------------------------------------------------------------------//
    //delete one or more templates from library
    //to delete a single template, simply send 1 as quantity or count
    //eg. deltmp 5 1

    else if(commandString == "deltmp") {
      uint16_t startLocation = atoi(firstParam);  //start location in library
      uint16_t count = atoi(secondParam); //quantity to delete
      response = deleteTemplate(startLocation, count);
    }

    //-------------------------------------------------------------------------//
    //precisely match templates on the buffers 1 and 2
    //returns match score (matchScore)
    //eg. mattmp

    else if(commandString == "mattmp") {
      response = matchTemplates();
    }

    //-------------------------------------------------------------------------//
    //search the library for content on the buffer
    //buffer ID should be 1 or 2
    //start location cane be #1 to #1000 (don't send the "#" with command)
    //eg. serlib 1 10 50

    else if(commandString == "serlib") {
      uint8_t bufferId = atoi(firstParam);
      uint16_t startLocation = atoi(secondParam);
      uint16_t count = atoi(thirdParam);
      response = searchLibrary(bufferId, startLocation, count);
    }

    //-------------------------------------------------------------------------//
    //unknown command

    else {
      printf("Invalid command : ");
      printf("%d",commandString);
    }

    printf("\n...........................");
    printf("\n");
    delay(2000);
  }
  return 0;
}

//=========================================================================//


