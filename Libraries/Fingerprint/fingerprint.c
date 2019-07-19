#include "fingerprint.h"

R30X fp;

void init (uint32_t password, uint32_t address){
    fp.devicePassword[0] = password & 0xFFU; //these can be altered later
    fp.devicePassword[1] = (password >> 8) & 0xFFU;
    fp.devicePassword[2] = (password >> 16) & 0xFFU;
    fp.devicePassword[3] = (password >> 24) & 0xFFU;
    fp.devicePasswordL = password;

    fp.deviceAddress[0] = address & 0xFFU;
    fp.deviceAddress[1] = (address >> 8) & 0xFFU;
    fp.deviceAddress[2] = (address >> 16) & 0xFFU;
    fp.deviceAddress[3] = (address >> 24) & 0xFFU;
    fp.deviceAddressL = address;

    fp.startCode[0] = FPS_ID_STARTCODE & 0xFFU; //packet start marker
    fp.startCode[1] = (FPS_ID_STARTCODE >> 8) & 0xFFU;

    resetParameters();
}

void begin (uint32_t baudrate) {
  delay(1000);  //one second delay to let the sensor 'boot up'

  fp.deviceBaudrate = baudrate;  //save the new baudrate
  fp.fd = serialOpen(baudrate);
}

void resetParameters (void) {
  fp.deviceBaudrate = FPS_DEFAULT_BAUDRATE;  //UART speed
  fp.securityLevel = FPS_DEFAULT_SECURITY_LEVEL;  //threshold level for fingerprint matching
  fp.dataPacketLength = FPS_DEFAULT_RX_DATA_LENGTH;

  fp.txPacketType = FPS_ID_COMMANDPACKET; //type of packet
  fp.txInstructionCode = FPS_CMD_VERIFYPASSWORD; //
  fp.txPacketLength[0] = 0;
  fp.txPacketLength[1] = 0;
  fp.txPacketLengthL = 0;
  fp.txDataBuffer = NULL; //packet data buffer
  fp.txDataBufferLength = 0;
  fp.txPacketChecksum[0] = 0;
  fp.txPacketChecksum[1] = 0;
  fp.txPacketChecksumL = 0;

  fp.rxPacketType = FPS_ID_COMMANDPACKET; //type of packet
  fp.rxConfirmationCode = FPS_CMD_VERIFYPASSWORD; //
  fp.rxPacketLength[0] = 0;
  fp.rxPacketLength[1] = 0;
  fp.rxPacketLengthL = 0;
  fp.rxDataBuffer = NULL; //packet data buffer
  fp.rxDataBufferLength = 0;
  fp.rxPacketChecksum[0] = 0;
  fp.rxPacketChecksum[1] = 0;
  fp.rxPacketChecksumL = 0;

  fp.fingerId = 0; //initialize them
  fp.matchScore = 0;
  fp.templateCount = 0;
}

uint8_t sendPacket (uint8_t type, uint8_t command, uint8_t* data, uint16_t dataLength) {
  if(data != NULL) {  //sometimes there's no additional data except the command
    fp.txDataBuffer = data;
    fp.txDataBufferLength = dataLength;
  }
  else {
    fp.txDataBuffer = NULL;
    fp.txDataBufferLength = 0;
  }

  fp.txPacketType = type; //type of packet - 1 byte
  fp.txInstructionCode = command; //instruction code - 1 byte
  fp.txPacketLengthL = fp.txDataBufferLength + 3; //1 byte for command, 2 bytes for checksum
  fp.txPacketLength[0] = fp.txPacketLengthL & 0xFFU; //get lower byte
  fp.txPacketLength[1] = (fp.txPacketLengthL >> 8) & 0xFFU; //get high byte

  fp.txPacketChecksumL = fp.txPacketType + fp.txPacketLength[0] + fp.txPacketLength[1] + fp.txInstructionCode; //sum of packet ID and packet length bytes

  for(int i=0; i<fp.txDataBufferLength; i++) {
    fp.txPacketChecksumL += txDataBuffer[i]; //add rest of the data bytes
  }

  fp.txPacketChecksum[0] = fp.txPacketChecksumL & 0xFFU; //get low byte
  fp.txPacketChecksum[1] = (fp.txPacketChecksumL >> 8) & 0xFFU; //get high byte

  serialPrintf(fp.fd,,"%d",fp.startCode[1]); //high byte is sent first
  serialPrintf(fp.fd,,"%d",fp.startCode[0]);
  serialPrintf(fp.fd,,"%d",fp.deviceAddress[3]); //high byte is sent first
  serialPrintf(fp.fd,,"%d",fp.deviceAddress[2]);
  serialPrintf(fp.fd,,"%d",fp.deviceAddress[1]);
  serialPrintf(fp.fd,,"%d",fp.deviceAddress[0]);
  serialPrintf(fp.fd,,"%d",fp.txPacketType);
  serialPrintf(fp.fd,,"%d",fp.txPacketLength[1]); //high byte is sent first
  serialPrintf(fp.fd,,"%d",fp.txPacketLength[0]);
  serialPrintf(fp.fd,,"%d",fp.txInstructionCode);

  for(int i=(fp.txDataBufferLength-1); i>=0; i--) {
    serialPrintf(fp.fd,,"%d",fp.txDataBuffer[i]); //send high byte first
  }

  serialPrintf(fp.fd,,"%d",fp.txPacketChecksum[1]);
  serialPrintf(fp.fd,,"%d",fp.txPacketChecksum[0]);

  #ifdef FPS_DEBUG
  printf("Sent packet = ");
    printf("%0#10x",fp.startCode[1]); //high byte is sent first
    printf("-");
    printf("%0#10x",fp.startCode[0]);
    printf("-");
    printf("%0#10x",fp.deviceAddress[3]); //high byte is sent first
    printf("-");
    printf("%0#10x",fp.deviceAddress[2]);
    printf("-");
    printf("%0#10x",fp.deviceAddress[1]);
    printf("-");
    printf("%0#10x",fp.deviceAddress[0]);
    printf("-");
    printf("%0#10x",fp.txPacketType);
    printf("-");
    printf("%0#10x",fp.txPacketLength[1]); //high byte is sent first
    printf("-");
    printf("%0#10x",fp.txPacketLength[0]);
    printf("-");
    printf("%0#10x",fp.txInstructionCode);
    printf("-");

    for(int i=(fp.txDataBufferLength-1); i>=0; i--) {
      printf("%0#10x",fp.txDataBuffer[i]); //send high byte first
      printf("-");
    }

    printf("%0#10x",fp.txPacketChecksum[1]);
    printf("-");
    printf("%0#10x",fp.txPacketChecksum[0]);
    printf("txInstructionCode = ");
    printf("%0#10x",fp.txInstructionCode);
    printf("txDataBufferLength = ");
    printf("%0#10x",fp.txDataBufferLength);
    printf("txPacketLengthL = ");
    printf(fp.txPacketLengthL);
    
  #endif

  return FPS_RX_OK;
}

uint8_t receivePacket (uint32_t timeout) {
  uint8_t* dataBuffer;
  if(dataPacketLength < 64) { //data buffer length should be at least 64 bytes
    dataBuffer = new uint8_t[64](); //conatains only the data
  }
  else {
    dataBuffer = new uint8_t[FPS_DEFAULT_RX_DATA_LENGTH]();
  }

  rxDataBuffer = dataBuffer;
  uint8_t serialBuffer[FPS_DEFAULT_SERIAL_BUFFER_LENGTH] = {0}; //serialBuffer will store high byte at the start of the array
  uint16_t serialBufferLength = 0;
  uint8_t byteBuffer = 0;

  #ifdef FPS_DEBUG
    printf("\n");
    printf("Reading response.");
  #endif

  while (timeout > 0) {
    if(serialDataAvail(fp.fd)) {
      byteBuffer = serialGetchar(fp.fd);
      #ifdef FPS_DEBUG
        // printf("Response byte found = ");
        // printf("%0#10x",byteBuffer);
      #endif
      serialBuffer[serialBufferLength] = byteBuffer;
      serialBufferLength++;
    }

    timeout--;
    delay(1);
  }

  if(serialBufferLength == 0) {
    #ifdef FPS_DEBUG
      printf("Serial timed out.");
      printf("This usually means the baud rate is not correct.");
    #endif
    return FPS_RX_TIMEOUT;
  }

  if(serialBufferLength < 10) {
    #ifdef FPS_DEBUG
      printf("Received bad packet with length < 10");
    #endif
    return FPS_RX_BADPACKET;
  }

  uint16_t token = 0;

  while(true) {
    switch (token) {
      case 0: //test packet start codes
        if(serialBuffer[token] == fp.startCode[1])
          break;
        else {
          #ifdef FPS_DEBUG //enable it to get debug info
            printf("Error at 0 : Start Code");
            printf("Received packet = ");
            for(int i=0; i < serialBufferLength; i++) {
              printf("%0#10x",serialBuffer[i]);
              if(i != (serialBufferLength - 1)) {
                printf("-");
              }
            }
            printf("\n");
          #endif

          return FPS_RX_BADPACKET;
        }

      case 1:
        if(serialBuffer[token] == startCode[0])
          break;
        else {
          #ifdef FPS_DEBUG
            printf("Error at 1 : Start Code");
            printf("Received packet = ");
            for(int i=0; i < serialBufferLength; i++) {
              printf("%0#10x",serialBuffer[i]);
              if(i != (serialBufferLength - 1)) {
                printf("-");
              }
            }
            printf("\n");
          #endif

          return FPS_RX_BADPACKET;
        }

      case 2: //test device address
        if(serialBuffer[token] == fp.deviceAddress[3])
          break;
        else {
          #ifdef FPS_DEBUG
            printf("Error at 2 : Device Address");
            printf("Received packet = ");
            for(int i=0; i < serialBufferLength; i++) {
              printf("%0#10x",serialBuffer[i]);
              if(i != (serialBufferLength - 1)) {
                printf("-");
              }
            }
            printf("\n");
          #endif

          return FPS_RX_BADPACKET;
        }
      
      case 3:
        if(serialBuffer[token] == fp.deviceAddress[2])
          break;
        else {
          #ifdef FPS_DEBUG
            printf("Error at 3 : Device Address");
            printf("Received packet = ");
            for(int i=0; i < serialBufferLength; i++) {
              printf("%0#10x",serialBuffer[i]);
              if(i != (serialBufferLength - 1)) {
                printf("-");
              }
            }
      
          #endif

          return FPS_RX_BADPACKET;
        }

      case 4:
        if(serialBuffer[token] == fp.deviceAddress[1])
          break;
        else {
          #ifdef FPS_DEBUG
            printf("Error at 4 : Device Address");
            printf("Received packet = ");
            for(int i=0; i < serialBufferLength; i++) {
              printf("%0#10x",serialBuffer[i]);
              if(i != (serialBufferLength - 1)) {
                printf("-\n");
              }
            }
          #endif

          return FPS_RX_BADPACKET;
        }
      
      case 5:
        if(serialBuffer[token] == fp.deviceAddress[0])
          break;
        else {
          #ifdef FPS_DEBUG
            printf("Error at 5 : Device Address");
            printf("Received packet = ");
            for(int i=0; i < serialBufferLength; i++) {
              printf("%0#10x",serialBuffer[i]);
              if(i != (serialBufferLength - 1)) {
                printf("-");
              }
            }
            
          #endif

          return FPS_RX_BADPACKET;
        }

      case 6: //test for valid packet type
        if((serialBuffer[token] == FPS_ID_COMMANDPACKET) || (serialBuffer[token] == FPS_ID_DATAPACKET) || (serialBuffer[token] == FPS_ID_ACKPACKET) || (serialBuffer[token] == FPS_ID_ENDDATAPACKET)) {
          fp.rxPacketType = serialBuffer[token]; //store the packet ID to class variable
          break;
        }
        else {
          #ifdef FPS_DEBUG
            printf("Error at 6 : Unknown Response");
            printf("Received packet = ");
            for(int i=0; i < serialBufferLength; i++) {
              printf("%0#10x",serialBuffer[i]);
              if(i != (serialBufferLength - 1)) {
                printf("-");
              }
            }
            printf("\n");
          #endif

          return FPS_RX_WRONG_RESPONSE;
        }

      case 7: //read packet data length
        if((serialBuffer[token] > 0) || (serialBuffer[token + 1] > 0)) {
          fp.rxPacketLength[0] = serialBuffer[token + 1];  //lower byte
          fp.rxPacketLength[1] = serialBuffer[token];  //higher byte
          fp.rxPacketLengthL = (uint16_t)(fp.rxPacketLength[1] << 8) + fp.rxPacketLength[0]; //calculate the full length value
          fp.rxDataBufferLength = fp.rxPacketLengthL - 3; //subtract 2 for checksum and 1 for command
          token++; //because we read one additional bytes here
          break;
        }

        else {
         #ifdef FPS_DEBUG
            printf("Error at 7 : Unknown Response");
            printf("Received packet = ");
            for(int i=0; i < serialBufferLength; i++) {
              printf("%0#10x",serialBuffer[i]);
              if(i != (serialBufferLength - 1)) {
                printf("-");
              }
            }
            printf("\n");
          #endif

          return FPS_RX_WRONG_RESPONSE;
        }

      case 9: //read confirmation or instruction code
        fp.rxConfirmationCode = serialBuffer[token]; //the first byte of data will be either instruction or confirmation code
        break;

      case 10: //read data
        for(int i=0; i < fp.rxDataBufferLength; i++) {
          fp.rxDataBuffer[(fp.rxDataBufferLength - 1) - i] = serialBuffer[token + i]; //store low values at start of the rxDataBuffer array
        }
        break;
      
      case 11: //read checksum
        if(fp.rxDataBufferLength == 0) { //sometimes there's no data other than the confirmation code
          fp.rxPacketChecksum[0] = serialBuffer[token]; //lower byte
          fp.rxPacketChecksum[1] = serialBuffer[token - 1]; //high byte
          fp.rxPacketChecksumL = (uint16_t)(fp.rxPacketChecksum[1] << 8) + fp.rxPacketChecksum[0]; //calculate L value

          uint16_t tempSum = 0; //temp checksum 

          tempSum = fp.rxPacketType + fp.rxPacketLength[0] + fp.rxPacketLength[1] + fp.rxConfirmationCode;

          if(fp.rxPacketChecksumL == tempSum) { //check if the calculated checksum matches the received one
            #ifdef FPS_DEBUG
              printf("Checksums match success.");
              printf("Received = ");
              printf("%0#10x",fp.rxPacketChecksum[1]);
              printf("-");
              printf("%0#10x",fp.rxPacketChecksum[0]);
              printf("Received L = " );
              printf("%0#10x",fp.rxPacketChecksumL);
              printf("Calculated = ");
              printf("%0#10x",(byte)(tempSum >> 8));
              printf("-");
              printf("%0#10x",(byte)(tempSum & 0xFFU));
              printf("Calculated L = ");
              printf("%0#10x",tempSum);
              printf("Received packet = ");

              for(int i=0; i < serialBufferLength; i++) {
                printf("%0#10x",serialBuffer[i]);
                if(i != (serialBufferLength - 1)) {
                  printf("-");
                }
              }
            
              printf("Data stream = none");

          
              printf("rxConfirmationCode = ");
              printf("%0#10x",fp.rxConfirmationCode);
              printf("rxDataBufferLength = ");
              printf("%0#10x",fp.rxDataBufferLength);
              printf("rxPacketLengthL = ");
              printf("%d",fp.rxPacketLengthL);
              printf("rxPacketLength[] = ");
              printf("%0#10x",fp.rxPacketLength[1]);
              printf("-");
              printf("%0#10x",fp.rxPacketLength[0]);
            #endif

            return FPS_RX_OK; //packet read success
          }

          else { //if the checksums do not match
            #ifdef FPS_DEBUG
              printf("Checksums match fail.");
              printf("Received = ");
              printf("%0#10x",fp.rxPacketChecksum[1]);
              printf("-");
              printf("%0#10x",fp.rxPacketChecksum[0]);
              printf("Received L = " );
              printf("%0#10x",fp.rxPacketChecksumL);
              printf("Calculated = ");
              printf("%0#10x",(byte)(tempSum >> 8));
              printf("-");
              printf("%0#10x",(byte)(tempSum & 0xFFU));
              printf("Calculated L = ");
              printf("%0#10x",tempSum);
              printf("Received packet = ");

              for(int i=0; i < serialBufferLength; i++) {
                printf("%0#10x",serialBuffer[i]);
                if(i != (serialBufferLength - 1)) {
                  printf("-");
                }
              }
              
              printf("Data stream = none");
              
              
              printf("rxConfirmationCode = ");
              printf("%0#10x",fp.rxConfirmationCode);
              printf("rxDataBufferLength = ");
              printf("%0#10x",fp.rxDataBufferLength);
              printf("rxPacketLengthL = ");
              printf("%0#10x",fp.rxPacketLengthL);
              printf("rxPacketLength[] = ");
              printf("%0#10x",fp.rxPacketLength[1]);
              printf("-");
              printf("%0#10x",fp.rxPacketLength[0]);
            #endif

            return FPS_RX_BADPACKET;  //then that's an error
          }
          break;
        }

        //-------------------------------------------------------------------------//

        else if((serialBuffer[token + (fp.rxDataBufferLength-1)] > 0) || ((serialBuffer[token + 1 + (fp.rxDataBufferLength-1)] > 0))) {
          fp.rxPacketChecksum[0] = serialBuffer[token + 1 + (fp.rxDataBufferLength-1)]; //lower byte
          fp.rxPacketChecksum[1] = serialBuffer[token + (fp.rxDataBufferLength-1)]; //high byte
          fp.rxPacketChecksumL = (uint16_t)(fp.rxPacketChecksum[1] << 8) + fp.rxPacketChecksum[0]; //calculate L value

          uint16_t tempSum = 0; //temp checksum 

          tempSum = fp.rxPacketType + fp.rxPacketLength[0] + fp.rxPacketLength[1] + fp.rxConfirmationCode;

          for(int i=0; i < fp.rxDataBufferLength; i++) {
            tempSum += fp.rxDataBuffer[i]; //calculate data checksum
          }

          if(fp.rxPacketChecksumL == tempSum) { //check if the calculated checksum matches the received one
            #ifdef FPS_DEBUG
              printf("Checksums match success.");
              printf("Received = ");
              printf("%0#10x",fp.rxPacketChecksum[1]);
              printf("-");
              printf("%0#10x",fp.rxPacketChecksum[0]);
              printf("Received L = " );
              printf("%0#10x",fp.rxPacketChecksumL);
              printf("Calculated = ");
              printf("%0#10x",(byte)(tempSum >> 8));
              printf("-");
              printf("%0#10x",(byte)(tempSum & 0xFFU));
              printf("Calculated L = ");
              printf("%0#10x",tempSum);
              printf("Received packet = ");

              for(int i=0; i < serialBufferLength; i++) {
                printf("%0#10x",serialBuffer[i]);
                if(i != (serialBufferLength - 1)) {
                  printf("-");
                }
              }
              printf("\n");
              printf("Data stream = ");

              for(int i=0; i < fp.rxDataBufferLength; i++) {
                printf("%0#10x",fp.rxDataBuffer[(fp.rxDataBufferLength-1) - i]);
                if(i != (fp.rxDataBufferLength - 1)) {
                  printf("-");
                }
              }

              
              printf("rxConfirmationCode = ");
              printf("%0#10x",fp.rxConfirmationCode);
              printf("rxDataBufferLength = ");
              printf("%0#10x",fp.rxDataBufferLength);
              printf("rxPacketLengthL = ");
              printf("%0#10x",fp.rxPacketLengthL);
              printf("rxPacketLength[] = ");
              printf("%0#10x",fp.rxPacketLength[1]);
              printf("-");
              printf("%0#10x",fp.rxPacketLength[0]);
            #endif

            return FPS_RX_OK; //packet read success
          }

          else { //if the checksums do not match
            #ifdef FPS_DEBUG
              printf("Checksums match fail.");
              printf("Received = ");
              printf("%0#10x",fp.rxPacketChecksum[1]);
              printf("-");
              printf("%0#10x",fp.rxPacketChecksum[0]);
              printf("Received L = " );
              printf("%0#10x",fp.rxPacketChecksumL);
              printf("Calculated = ");
              printf("%0#10x",(byte)(tempSum >> 8));
              printf("-");
              printf("%0#10x",(byte)(tempSum & 0xFFU));
              printf("Calculated L = ");
              printf("%0#10x",tempSum);
              printf("Received packet = ");

              for(int i=0; i < serialBufferLength; i++) {
                printf("%0#10x",serialBuffer[i]);
                if(i != (serialBufferLength - 1)) {
                  printf("-");
                }
              }
              printf("\n");
              printf("Data stream = ");

              for(int i=0; i < fp.rxDataBufferLength; i++) {
                printf("%0#10x",fp.rxDataBuffer[(fp.rxDataBufferLength-1) - i]);
                if(i != (fp.rxDataBufferLength - 1)) {
                  printf("-");
                }
              }
              
              
              printf("rxConfirmationCode = ");
              printf("%0#10x",fp.rxConfirmationCode);
              printf("rxDataBufferLength = ");
              printf("%0#10x",fp.rxDataBufferLength);
              printf("rxPacketLengthL = ");
              printf("%0#10x",fp.rxPacketLengthL);
              printf("rxPacketLength[] = ");
              printf("%0#10x",fp.rxPacketLength[1]);
              printf("-");
              printf("%0#10x",fp.rxPacketLength[0]);
            #endif

            return FPS_RX_BADPACKET;  //then that's an error
          }
          break;
        }

        //-------------------------------------------------------------------------//

        else { //if the checksum received is 0
          #ifdef FPS_DEBUG
            printf("Error at 12 : Checksum");
            printf("Received packet = ");
            for(int i=0; i < serialBufferLength; i++) {
              printf("%0#10x",serialBuffer[i]);
              if(i != (serialBufferLength - 1)) {
                printf("-");
              }
            }
            printf("\n");
          #endif

          return FPS_RX_BADPACKET;  //that too an error
        }
        break;
    
      default:
        break;
    }
    token++; //increment to progressively scan the packet
  }
}

uint8_t verifyPassword (uint32_t password) {
  uint8_t passwordArray[4] = {0};
  passwordArray[0] = password & 0xFFU;
  passwordArray[1] = (password >> 8) & 0xFFU;
  passwordArray[2] = (password >> 16) & 0xFFU;
  passwordArray[3] = (password >> 24) & 0xFFU;

  sendPacket(FPS_ID_COMMANDPACKET, FPS_CMD_VERIFYPASSWORD, passwordArray, 4); //send the command and data
  uint8_t response = receivePacket(); //read response
  if(response == FPS_RX_OK) { //if the response packet is valid
    if(fp.rxConfirmationCode == FPS_RESP_OK) {
      fp.devicePasswordL = password;
      fp.devicePassword[0] = passwordArray[0]; //save the new password as array
      fp.devicePassword[1] = passwordArray[1];
      fp.devicePassword[2] = passwordArray[2];
      fp.devicePassword[3] = passwordArray[3];

      #ifdef FPS_DEBUG
        printf("Password is correct.");
        printf("Current Password = ");
        printf("%0#10x",fp.devicePasswordL);
      #endif

      return FPS_RESP_OK; //password is correct
    }
    else {
      #ifdef FPS_DEBUG
        printf("Password is not correct.");
        printf("Current Password = ");
        printf("%0#10x",fp.devicePasswordL);
        printf("rxConfirmationCode = ");
        printf("%0#10x",fp.rxConfirmationCode);
      #endif

      return fc.rxConfirmationCode;  //password is not correct and so send confirmation code
    }
  }
  else {
    return response; //return packet receive error code
  }
}

uint8_t setPassword (uint32_t password) {
  uint8_t passwordArray[4] = {0};
  passwordArray[0] = password & 0xFFU;
  passwordArray[1] = (password >> 8) & 0xFFU;
  passwordArray[2] = (password >> 16) & 0xFFU;
  passwordArray[3] = (password >> 24) & 0xFFU;

  sendPacket(FPS_ID_COMMANDPACKET, FPS_CMD_SETPASSWORD, passwordArray, 4); //send the command and data
  uint8_t response = receivePacket(); //read response

  if(response == FPS_RX_OK) { //if the response packet is valid
    if(fp.rxConfirmationCode == FPS_RESP_OK) { //the confrim code will be saved when the response is received
      fp.devicePasswordL = password; //save the new password (Long)
      fp.devicePassword[0] = passwordArray[0]; //save the new password as array
      fp.devicePassword[1] = passwordArray[1];
      fp.devicePassword[2] = passwordArray[2];
      fp.devicePassword[3] = passwordArray[3];

      #ifdef FPS_DEBUG
        printf("New password = ");
        printf("%0#10x",fp.devicePasswordL);
      #endif

      return FPS_RESP_OK; //password setting complete
    }
    else {
      #ifdef FPS_DEBUG
        printf("Setting password failed.");
        printf("rxConfirmationCode = ");
        printf("%0#10x",fp.rxConfirmationCode);
      #endif
      return fp.rxConfirmationCode;  //setting was unsuccessful and so send confirmation code
    }
  }
  else {
    return response; //return packet receive error code
  }
}

uint8_t setAddress (uint32_t address) {
  uint8_t addressArray[4] = {0}; //just so that we do not need to alter the existing address before successfully changing it
  addressArray[0] = address & 0xFF;
  addressArray[1] = (address >> 8) & 0xFF;
  addressArray[2] = (address >> 16) & 0xFF;
  addressArray[3] = (address >> 24) & 0xFF;

  sendPacket(FPS_ID_COMMANDPACKET, FPS_CMD_SETDEVICEADDRESS, addressArray, 4); //send the command and data

  fp.deviceAddressL = address; //save the new address (Long)
  fp.deviceAddress[0] = addressArray[0]; //save the new address as array
  fp.deviceAddress[1] = addressArray[1];
  fp.deviceAddress[2] = addressArray[2];
  fp.deviceAddress[3] = addressArray[3];

  uint8_t response = receivePacket(); //read response

  if(response == FPS_RX_OK) { //if the response packet is valid
    if((fp.rxConfirmationCode == FPS_RESP_OK) || (fp.rxConfirmationCode == 0x20U)) { //the confrim code will be saved when the response is received
      #ifdef FPS_DEBUG
        printf("Setting address success.");
        printf("New address = ");
        printf("%0#10x",fp.deviceAddressL);
      #endif

      return FPS_RESP_OK; //address setting complete
    }
    else {
      #ifdef FPS_DEBUG
        printf("Setting address failed.");
        printf("rxConfirmationCode = ");
        printf("%0#10x",fp.rxConfirmationCode);
      #endif
      return fp.rxConfirmationCode;  //setting was unsuccessful and so send confirmation code
    }
  }
  else {
    return response; //return packet receive error code
  }
}

uint8_t setBaudrate (uint32_t baud) {
  uint8_t baudNumber = baud / 9600; //check is the baudrate is a multiple of 9600
  uint8_t dataArray[2] = {0};

  if((baudNumber > 0) && (baudNumber < 13)) { //should be between 1 (9600bps) and 12 (115200bps)
    dataArray[0] = baudNumber;  //low byte
    dataArray[1] = 4; //the code for the system parameter number, 4 means baudrate

    sendPacket(FPS_ID_COMMANDPACKET, FPS_CMD_SETSYSPARA, dataArray, 2); //send the command and data
    uint8_t response = receivePacket(); //read response

    if(response == FPS_RX_OK) { //if the response packet is valid
      if(fp.rxConfirmationCode == FPS_RESP_OK) { //the confirm code will be saved when the response is received
        fp.deviceBaudrate = baud;
        
        if (fp.fd) { //if using hardware serial
          serialClose(fp.fd);  //end the existing serial port
          serialOpen(fp.fd,fp.deviceBaudrate);  //restart the port with new baudrate
        }

        #ifdef FPS_DEBUG
          printf("Setting baudrate success.");
        #endif
        return FPS_RESP_OK; //baudrate setting complete
      }
      else {
        #ifdef FPS_DEBUG
          printf("Setting baudrate failed.");
          printf("rxConfirmationCode = ");
          printf("%0#10x",fp.rxConfirmationCode);
        #endif
        return fp.rxConfirmationCode;  //setting was unsuccessful and so send confirmation code
      }
    }
    else {
      return response; //return packet receive error code
    }
  }
  else {
    #ifdef FPS_DEBUG
      printf("Bad baudrate value.");
      printf("Setting baudrate failed.");
    #endif
    return FPS_BAD_VALUE;
  }
}

uint8_t setSecurityLevel (uint8_t level) {
  uint8_t dataArray[2] = {0};

  if((level > 0) && (level < 6)) { //should be between 1 and 5
    dataArray[0] = level;  //low byte
    dataArray[1] = 5; //the code for the system parameter number, 5 means security level

    sendPacket(FPS_ID_COMMANDPACKET, FPS_CMD_SETSYSPARA, dataArray, 2); //send the command and data
    uint8_t response = receivePacket(); //read response

    if(response == FPS_RX_OK) { //if the response packet is valid
      if(fp.rxConfirmationCode == FPS_RESP_OK) { //the confirm code will be saved when the response is received
        #ifdef FPS_DEBUG
          printf("Setting new security level success.");
          printf("Old value = ");
          printf("%0#10x",fp.securityLevel);
          printf("New value = ");
          printf("%0#10x",level);
        #endif
        fp.securityLevel = level;  //save new value
        return FPS_RESP_OK; //security level setting complete
      }
      else {
        #ifdef FPS_DEBUG
          printf("Setting security level failed.");
          printf("Current value = ");
          printf("%0#10x",fp.securityLevel);
          printf("rxConfirmationCode = ");
          printf("%0#10x",fp.rxConfirmationCode);
        #endif
        return fp.rxConfirmationCode;  //setting was unsuccessful and so send confirmation code
      }
    }
    else {
      return response; //return packet receive error code
    }
  }
  else {
    #ifdef FPS_DEBUG
      printf("Bad security level value.");
      printf("Setting security level failed.");
    #endif
    return FPS_BAD_VALUE; //the received parameter is invalid
  }
}

uint8_t setDataLength (uint16_t length) {
   #ifdef FPS_DEBUG
    printf("Setting new data length..");
  #endif

  uint8_t dataArray[2] = {0};

  if((length == 32) || (length == 64) || (length == 128) || (length == 256)) { //should be 32, 64, 128 or 256 bytes
    if(length == 32)
      dataArray[0] = 0;  //low byte
    else if(length == 64)
      dataArray[0] = 1;  //low byte
    else if(length == 128)
      dataArray[0] = 2;  //low byte
    else if(length == 256)
      dataArray[0] = 3;  //low byte

    dataArray[1] = 6; //the code for the system parameter number
    sendPacket(FPS_ID_COMMANDPACKET, FPS_CMD_SETSYSPARA, dataArray, 2); //send the command and data
    uint8_t response = receivePacket(); //read response

    if(response == FPS_RX_OK) { //if the response packet is valid
      if(fp.rxConfirmationCode == FPS_RESP_OK) { //the confirm code will be saved when the response is received
        fp.dataPacketLength = length;  //save the new data length

        #ifdef FPS_DEBUG
          printf("Setting data length success.");
          printf("dataPacketLength = ");
          printf("%d",fp.dataPacketLength);
        #endif

        return FPS_RESP_OK; //length setting complete
      }
      else {
        #ifdef FPS_DEBUG
          printf("Setting data length failed.");
          printf("rxConfirmationCode = ");
          printf("%0#10x",fp.rxConfirmationCode);
        #endif
        return fp.rxConfirmationCode;  //setting was unsuccessful and so send confirmation code
      }
    }
    else {
      return response; //return packet receive error code
    }
  }
  else {
    #ifdef FPS_DEBUG
      printf("Bad data length value.");
      printf("Setting data length failed.");
    #endif
    return FPS_BAD_VALUE; //the received parameter is invalid
  }
}

uint8_t portControl (uint8_t value) {
  #ifdef FPS_DEBUG
    if(value == 1)
      printf("Turing port on..");
    else
      printf("Turing port off..");
  #endif

  uint8_t dataArray[1] = {0};

  if((value == 0) || (value == 1)) { //should be either 1 or 0
    dataArray[0] = value;
    sendPacket(FPS_ID_COMMANDPACKET, FPS_CMD_PORTCONTROL, dataArray, 1); //send the command and data
    uint8_t response = receivePacket(); //read response

    if(response == FPS_RX_OK) { //if the response packet is valid
      if(fp.rxConfirmationCode == FPS_RESP_OK) { //the confirm code will be saved when the response is received
        #ifdef FPS_DEBUG
          if(value == 1)
            printf("Turing port on success.");
          else
            printf("Turing port off success.");
        #endif
        return FPS_RESP_OK; //port setting complete
      }
      else {
        #ifdef FPS_DEBUG
          printf("Turning port on/off failed.");
          printf("rxConfirmationCode = ");
          printf("%0#10x",fp.rxConfirmationCode);
        #endif
        return fp.rxConfirmationCode;  //setting was unsuccessful and so send confirmation code
      }
    }
    else {
      return response; //return packet receive error code
    }
  }
  else {
    return FPS_BAD_VALUE; //the received parameter is invalid
  }
}

uint8_t readSysPara() {
  #ifdef FPS_DEBUG
    printf("Reading system parameters..");
  #endif

  sendPacket(FPS_ID_COMMANDPACKET, FPS_CMD_READSYSPARA); //send the command, there's no additional data
  uint8_t response = receivePacket(); //read response

  if(response == FPS_RX_OK) { //if the response packet is valid
    if(fp.rxConfirmationCode == FPS_RESP_OK) { //the confirm code will be saved when the response is received
      fp.statusRegister = uint16_t(fp.rxDataBuffer[15] << 8) + fp.rxDataBuffer[14];  //high byte + low byte
      fp.securityLevel = dp.rxDataBuffer[8];

      if(fp.rxDataBuffer[2] == 0)
        fp.dataPacketLength = 32;
      else if(rxDataBuffer[2] == 1)
        fp.dataPacketLength = 64;
      else if(rxDataBuffer[2] == 2)
        fp.dataPacketLength = 128;
      else if(rxDataBuffer[2] == 3)
        fp.dataPacketLength = 256;

      fp.deviceBaudrate = fp.rxDataBuffer[0] * 9600;  //baudrate is retrieved as a number

      #ifdef FPS_DEBUG
        printf("Reading system parameters success.");
        printf("statusRegister = ");
        printf("%d",fp.statusRegister);
        printf("securityLevel = ");
        printf("%d",fp.securityLevel);
        printf("dataPacketLength = ");
        printf("%d",fp.dataPacketLength);
        printf("deviceBaudrate = ");
        printf("%d",fp.deviceBaudrate);
      #endif

      return FPS_RESP_OK;
    }
    else {
      #ifdef FPS_DEBUG
        printf("Reading system parameters failed.");
        printf("rxConfirmationCode = ");
        printf("%0#10x",fp.rxConfirmationCode);
      #endif
      return fp.rxConfirmationCode;  //setting was unsuccessful and so send confirmation code
    }
  }
  else {
    return response; //return packet receive error code
  }
}

uint8_t getTemplateCount() {
  #ifdef FPS_DEBUG
    printf("Reading template count..");
  #endif

  sendPacket(FPS_ID_COMMANDPACKET, FPS_CMD_TEMPLATECOUNT); //send the command, there's no additional data
  uint8_t response = receivePacket(); //read response

  if(response == FPS_RX_OK) { //if the response packet is valid
    if(fp.rxConfirmationCode == FPS_RESP_OK) { //the confirm code will be saved when the response is received
      fp.templateCount = (uint16_t)(fp.rxDataBuffer[1] << 8) + fp.rxDataBuffer[0];  //high byte + low byte

      #ifdef FPS_DEBUG
        printf("Reading template count success.");
        printf("templateCount = ");
        printf("%d",fp.templateCount);
      #endif

      return FPS_RESP_OK;
    }
    else {
      #ifdef FPS_DEBUG
        printf("Reading template count failed.");
        printf("rxConfirmationCode = ");
        printf("%0#10x",fp.rxConfirmationCode);
      #endif
      return fp.rxConfirmationCode;  //setting was unsuccessful and so send confirmation code
    }
  }
  else {
    return response; //return packet receive error code
  }
}

uint8_t captureAndRangeSearch (uint16_t captureTimeout, uint16_t startLocation, uint16_t count) {
  if(captureTimeout > 25500) { //if range overflows
    #ifdef FPS_DEBUG
      printf("Capture and range search failed.");
      printf("Bad capture timeout.");
      printf("captureTimeout = ");
      printf("%d",captureTimeout);
    #endif
    return FPS_BAD_VALUE;
  }

  if((startLocation > 1000) || (startLocation < 1)) { //if not in range (0-999)
    #ifdef FPS_DEBUG
      printf("Capture and range search failed.");
      printf("Bad start ID");
      printf("startId = #");
      printf("%d",startLocation);
    #endif

    return FPS_BAD_VALUE;
  }

  if((startLocation + count) > 1001) { //if range overflows
    #ifdef FPS_DEBUG
      printf("Capture and range search failed.");
      printf("startLocation + count can't be greater than 1001.");
      printf("startLocation = #");
      printf("%d",startLocation);
      printf("count = ");
      printf("%d",count);
      printf("startLocation + count = ");
      printf("%d",startLocation + count);
    #endif
    return FPS_BAD_VALUE;
  }

  uint8_t dataArray[5] = {0}; //need 5 bytes here

  //generate the data array
  dataArray[4] = (uint8_t)(captureTimeout / 140);  //this byte is sent first
  dataArray[3] = ((startLocation-1) >> 8) & 0xFFU;  //high byte
  dataArray[2] = (uint8_t)((startLocation-1) & 0xFFU);  //low byte
  dataArray[1] = (count >> 8) & 0xFFU; //high byte
  dataArray[0] = (uint8_t)(count & 0xFFU); //low byte

  #ifdef FPS_DEBUG
    printf("Starting capture and range search.");
    printf("captureTimeout = ");
    printf("%d",captureTimeout);
    printf("startLocation = #");
    printf("%d",startLocation);
    printf("count = ");
    printf("%d",count);
    printf("startLocation + count = ");
    printf("%d",startLocation + count);
  #endif

  sendPacket(FPS_ID_COMMANDPACKET, FPS_CMD_GETANDRANGESEARCH, dataArray, 5); //send the command, there's no additional data
  uint8_t response = receivePacket(captureTimeout + 100); //read response

  if(response == FPS_RX_OK) { //if the response packet is valid
    if(fp.rxConfirmationCode == FPS_RESP_OK) { //the confirm code will be saved when the response is received
      fp.fingerId = (uint16_t)(fp.rxDataBuffer[3] << 8) + fp.rxDataBuffer[2];  //high byte + low byte
      fp.fingerId += 1;  //because IDs start from #1
      fp.matchScore = (uint16_t)(fp.rxDataBuffer[1] << 8) + fp.rxDataBuffer[0];  //data length will be 4 here

      #ifdef FPS_DEBUG
        printf("Capture and range search success.");
        printf("fingerId = #");
        printf("%d",fp.fingerId);
        printf("matchScore = ");
        printf("%d",fp.matchScore);
      #endif

      return FPS_RESP_OK;
    }
    else {
      fp.fingerId = 0;
      fp.matchScore = 0;

      #ifdef FPS_DEBUG
        printf("Fingerprint not found.");
        printf("rxConfirmationCode = ");
        printf("%0#10x",fp.rxConfirmationCode);
      #endif

      return fp.rxConfirmationCode;  //setting was unsuccessful and so send confirmation code
    }
  }
  else {
    return response; //return packet receive error code
  }
}

//=========================================================================//
//scans the fingerprint and finds a match within the full range of library

uint8_t captureAndFullSearch () {
  #ifdef FPS_DEBUG
    printf("Starting capture and full search.");
  #endif

  sendPacket(FPS_ID_COMMANDPACKET, FPS_CMD_GETANDFULLSEARCH); //send the command, there's no additional data
  uint8_t response = receivePacket(3000); //read response

  if(response == FPS_RX_OK) { //if the response packet is valid
    if(fp.rxConfirmationCode == FPS_RESP_OK) { //the confirm code will be saved when the response is received
      fp.fingerId = (uint16_t)(fp.rxDataBuffer[3] << 8) + fp.rxDataBuffer[2];  //high byte + low byte
      fp.fingerId += 1;  //because IDs start from #1
      fp.matchScore = (uint16_t)(fp.rxDataBuffer[1] << 8) + fp.rxDataBuffer[0];  //data length will be 4 here

      #ifdef FPS_DEBUG
        printf("Capture and full search success.");
        printf("fingerId = #");
        printf("%d",fp.fingerId);
        printf("matchScore = ");
        printf("%d",fp.matchScore);
      #endif

      return FPS_RESP_OK;
    }
    else {
      fp.fingerId = 0;
      fp.matchScore = 0;

      #ifdef FPS_DEBUG
        printf("Fingerprint not found.");
        printf("rxConfirmationCode = ");
        printf("%0#10x",fp.rxConfirmationCode);
      #endif

      return fp.rxConfirmationCode;  //setting was unsuccessful and so send confirmation code
    }
  }
  else {
    return response; //return packet receive error code
  }
}

//=========================================================================//
//scan the fingerprint, generate an image and store it on the buffer

uint8_t generateImage () {
  #ifdef FPS_DEBUG
    printf("Generating fingerprint image..");
  #endif

  sendPacket(FPS_ID_COMMANDPACKET, FPS_CMD_GENIMAGE); //send the command, there's no additional data
  uint8_t response = receivePacket(); //read response

  if(response == FPS_RX_OK) { //if the response packet is valid
    if(fp.rxConfirmationCode == FPS_RESP_OK) { //the confirm code will be saved when the response is received
      #ifdef FPS_DEBUG
        printf("Image saved to buffer successfully.");
      #endif
      return FPS_RESP_OK; //just the confirmation code only
    }
    else {
      #ifdef FPS_DEBUG
        printf("Generating fingerprint failed.");
        printf("rxConfirmationCode = ");
        printf("%0#10x",fp.rxConfirmationCode);
      #endif
      return fp.rxConfirmationCode;  //setting was unsuccessful and so send confirmation code
    }
  }
  else {
    return response; //return packet receive error code
  }
}

//=========================================================================//
//get the image stored in the image buffer
//this is not completely implemented

uint8_t downloadImage () {
  sendPacket(FPS_ID_COMMANDPACKET, FPS_CMD_DOWNLOADIMAGE); //send the command, there's no additional data
  uint8_t response = receivePacket(); //read response

  if(response == FPS_RX_OK) { //if the response packet is valid
    if(fp.rxConfirmationCode == FPS_RESP_OK) { //the confirm code will be saved when the response is received
      return FPS_RESP_OK; //just the confirmation code only
    }
    else {
      return fp.rxConfirmationCode;  //setting was unsuccessful and so send confirmation code
    }
  }
  else {
    return response; //return packet receive error code
  }
}

//=========================================================================//
//get the image stored in the image buffer
//this is not completely implemented

uint8_t uploadImage (uint8_t* dataBuffer) {
  sendPacket(FPS_ID_COMMANDPACKET, FPS_CMD_UPLOADIMAGE, dataBuffer, 64); //send the command, there's no additional data
  uint8_t response = receivePacket(); //read response

  if(response == FPS_RX_OK) { //if the response packet is valid
    if(fp.rxConfirmationCode == FPS_RESP_OK) { //the confirm code will be saved when the response is received
      return FPS_RESP_OK; //just the confirmation code only
    }
    else {
      return fp.rxConfirmationCode;  //setting was unsuccessful and so send confirmation code
    }
  }
  else {
    return response; //return packet receive error code
  }
}

//=========================================================================//
//generate character file from image stored in image buffer and store it on
//one of the two character buffers

uint8_t generateCharacter (uint8_t bufferId) {
  if(!((bufferId > 0) && (bufferId < 3))) { //if the value is not 1 or 2
    #ifdef FPS_DEBUG
      printf("Generating character file failed.");
      printf("Bad value. bufferId can only be 1 or 2.");
      printf("bufferId = ");
      printf("%d",bufferId);
    #endif

    return FPS_BAD_VALUE;
  }
  uint8_t dataBuffer[1] = {bufferId}; //create data array

  #ifdef FPS_DEBUG
    printf("Generating character file..");
    printf("Character bufferId = ");
    printf("%d",bufferId);
  #endif
  
  sendPacket(FPS_ID_COMMANDPACKET, FPS_CMD_UPLOADIMAGE, dataBuffer, 1); //send the command, there's no additional data
  uint8_t response = receivePacket(); //read response

  if(response == FPS_RX_OK) { //if the response packet is valid
    if(fp.rxConfirmationCode == FPS_RESP_OK) { //the confirm code will be saved when the response is received
      #ifdef FPS_DEBUG
        printf("Generating character file successful.");
      #endif
      return FPS_RESP_OK; //just the confirmation code only
    }
    else {
      #ifdef FPS_DEBUG
        printf("Generating character file failed.");
        printf("rxConfirmationCode = ");
        printf("%0#10x",fp.rxConfirmationCode);

        if(fp.rxConfirmationCode == FPS_RESP_OVERDISORDERFAIL2) {
          printf("Character file overly disordered.");
        }

        if(fp.rxConfirmationCode == FPS_RESP_FEATUREFAIL) {
          printf("Character file feature fail.");
        }

        if(fp.rxConfirmationCode == FPS_RESP_IMAGEGENERATEFAIL) {
          printf("Valid image not available.");
        }

      #endif

      return fp.rxConfirmationCode;  //setting was unsuccessful and so send confirmation code
    }
  }
  else {
    return response; //return packet receive error code
  }
}

//=========================================================================//
//combine the two character files and generate a template

uint8_t generateTemplate () {
  #ifdef FPS_DEBUG
    printf("Generating template from char buffers..");
  #endif

  sendPacket(FPS_ID_COMMANDPACKET, FPS_CMD_UPLOADIMAGE); //send the command, there's no additional data
  uint8_t response = receivePacket(); //read response

  if(response == FPS_RX_OK) { //if the response packet is valid
    if(fp.rxConfirmationCode == FPS_RESP_OK) { //the confirm code will be saved when the response is received
      #ifdef FPS_DEBUG
        printf("Generating template success.");
      #endif
      return FPS_RESP_OK; //just the confirmation code only
    }
    else {
      #ifdef FPS_DEBUG
        printf("Generating template failed.");
        printf("rxConfirmationCode = ");
        printf("%0#10x",fp.rxConfirmationCode);
      #endif
      return fp.rxConfirmationCode;  //setting was unsuccessful and so send confirmation code
    }
  }
  else {
    return response; //return packet receive error code
  }
}

//=========================================================================//
//combine the two character files and generate a template
//this is not completely implemented

uint8_t downloadCharacter (uint8_t bufferId) {
  uint8_t dataBuffer[1] = {bufferId}; //create data array
  sendPacket(FPS_ID_COMMANDPACKET, FPS_CMD_UPLOADIMAGE); //send the command, there's no additional data
  uint8_t response = receivePacket(); //read response

  if(response == FPS_RX_OK) { //if the response packet is valid
    if(fp.rxConfirmationCode == FPS_RESP_OK) { //the confirm code will be saved when the response is received
      return FPS_RESP_OK; //just the confirmation code only
    }
    else {
      return fp.rxConfirmationCode;  //setting was unsuccessful and so send confirmation code
    }
  }
  else {
    return response; //return packet receive error code
  }
}

//=========================================================================//
//combine the two character files and generate a template
//this is not completely implemented

uint8_t uploadCharacter (uint8_t bufferId, uint8_t* dataBuffer) {
  uint8_t dataArray[sizeof(dataBuffer)+1] = {0}; //create data array
  dataArray[sizeof(dataBuffer)];
  sendPacket(FPS_ID_COMMANDPACKET, FPS_CMD_UPLOADIMAGE); //send the command, there's no additional data
  uint8_t response = receivePacket(); //read response

  if(response == FPS_RX_OK) { //if the response packet is valid
    if(fp.rxConfirmationCode == FPS_RESP_OK) { //the confirm code will be saved when the response is received
      return FPS_RESP_OK; //just the confirmation code only
    }
    else {
      return fp.rxConfirmationCode;  //setting was unsuccessful and so send confirmation code
    }
  }
  else {
    return response; //return packet receive error code
  }
}

//=========================================================================//
//store the contents of one of the two template (character) buffers to a
//location on the fingerprint library

uint8_t saveTemplate (uint8_t bufferId, uint16_t location) {
  if(!((bufferId > 0) && (bufferId < 3))) { //if the value is not 1 or 2
    #ifdef FPS_DEBUG
      printf("Storing template failed.");
      printf("Bad value. bufferId can only be 1 or 2.");
      printf("bufferId = ");
      printf("%d",bufferId);
    #endif

    return FPS_BAD_VALUE;
  }

  if((location > 1000) || (location < 1)) { //if the value is not in range
    #ifdef FPS_DEBUG
      printf("Generating template failed.");
      printf("Bad value. location must be #1 to #1000.");
      printf("location = ");
      printf("%d",location);
    #endif

    return FPS_BAD_VALUE;
  }

  #ifdef FPS_DEBUG
    printf("Saving template..");
  #endif

  uint8_t dataArray[3] = {0}; //create data array
  dataArray[2] = bufferId;  //highest byte
  dataArray[1] = ((location-1) >> 8) & 0xFFU; //high byte of location
  dataArray[0] = ((location-1) & 0xFFU); //low byte of location

  sendPacket(FPS_ID_COMMANDPACKET, FPS_CMD_STORETEMPLATE, dataArray, 3); //send the command and data
  uint8_t response = receivePacket(); //read response

  if(response == FPS_RX_OK) { //if the response packet is valid
    if(fp.rxConfirmationCode == FPS_RESP_OK) { //the confirm code will be saved when the response is received
      #ifdef FPS_DEBUG
        printf("Storing template successful.");
        printf("Saved to #");
        printf("%d",location);
      #endif
      return FPS_RESP_OK; //just the confirmation code only
    }
    else {
      #ifdef FPS_DEBUG
        printf("Storing template failed.");
        printf("rxConfirmationCode = ");
        printf("%0#10x",fp.rxConfirmationCode);
      #endif
      return fp.rxConfirmationCode;  //setting was unsuccessful and so send confirmation code
    }
  }
  else {
    return response; //return packet receive error code
  }
}

//=========================================================================//
//load the contents from a location on the library to one of the two
//template/character buffers

uint8_t loadTemplate (uint8_t bufferId, uint16_t location) {
  if(!((bufferId > 0) && (bufferId < 3))) { //if the value is not 1 or 2
    #ifdef FPS_DEBUG
      printf("Loading template failed.");
      printf("Bad value. bufferId can only be 1 or 2.");
      printf("bufferId = ");
      printf("%d",bufferId);
    #endif

    return FPS_BAD_VALUE;
  }

  if((location > 1000) || (location < 1)) { //if the value is not in range
    #ifdef FPS_DEBUG
      printf("Loading template failed.");
      printf("Bad value. location must be #1 to #1000.");
      printf("location = ");
      printf("%d",location);
    #endif

    return FPS_BAD_VALUE;
  }

  uint8_t dataArray[3] = {0}; //create data array
  dataArray[2] = bufferId;  //highest byte
  dataArray[1] = ((location-1) >> 8) & 0xFFU; //high byte of location
  dataArray[0] = ((location-1) & 0xFFU); //low byte of location

  #ifdef FPS_DEBUG
    printf("Loading template..");
  #endif

  sendPacket(FPS_ID_COMMANDPACKET, FPS_CMD_LOADTEMPLATE, dataArray, 3); //send the command and data
  uint8_t response = receivePacket(); //read response

  if(response == FPS_RX_OK) { //if the response packet is valid
    if(fp.rxConfirmationCode == FPS_RESP_OK) { //the confirm code will be saved when the response is received
      #ifdef FPS_DEBUG
        printf("Loading template successful.");
        printf("Loaded #");
        printf("%d",location);
        printf(" to buffer ");
        printf("%d",bufferId);
      #endif

      return FPS_RESP_OK; //just the confirmation code only
    }
    else {
      #ifdef FPS_DEBUG
        printf("Loading template failed.");
        printf("rxConfirmationCode = ");
        printf("%0#10x",fp.rxConfirmationCode);
      #endif
      return fp.rxConfirmationCode;  //setting was unsuccessful and so send confirmation code
    }
  }
  else {
    return response; //return packet receive error code
  }
}

uint8_t deleteTemplate (uint16_t startLocation, uint16_t count) {
  if((startLocation > 1000) || (startLocation < 1)) { //if the value is not 1 or 2
    #ifdef FPS_DEBUG
      printf("Deleting template failed.");
      printf("Bad value. Start location must be #1 to #1000.");
      printf("startLocation = ");
      printf("%d",startLocation);
    #endif

    return FPS_BAD_VALUE;
  }

  if((count + startLocation) > 1001) { //if the value is not in range
    #ifdef FPS_DEBUG
      printf("Deleting template failed.");
      printf("Bad value. Sum of startLocation and count can't be greater than 1001.");
      printf("startLocation + count = ");
      printf("%d",startLocation + count);
    #endif

    return FPS_BAD_VALUE;
  }

  uint8_t dataArray[4] = {0}; //create data array
  dataArray[3] = ((startLocation-1) >> 8) & 0xFFU; //high byte of location
  dataArray[2] = ((startLocation-1) & 0xFFU); //low byte of location
  dataArray[1] = (count >> 8) & 0xFFU; //high byte of total no. of templates to delete
  dataArray[0] = (count & 0xFFU); //low byte of count

  #ifdef FPS_DEBUG
    printf("Deleting template..");
  #endif

  sendPacket(FPS_ID_COMMANDPACKET, FPS_CMD_DELETETEMPLATE, dataArray, 4); //send the command and data
  uint8_t response = receivePacket(); //read response

  if(response == FPS_RX_OK) { //if the response packet is valid
    if(fp.rxConfirmationCode == FPS_RESP_OK) { //the confirm code will be saved when the response is received
     #ifdef FPS_DEBUG
        printf("Deleting template successful.");
        printf("From #");
        printf("%d",startLocation);
        printf(" to #");
        printf("%d",startLocation + count - 1);
      #endif
      return FPS_RESP_OK; //just the confirmation code only
    }
    else {
      #ifdef FPS_DEBUG
        printf("Deleting template failed.");
        printf("rxConfirmationCode = ");
        printf("%0#10x",fp.rxConfirmationCode);
      #endif
      return fp.rxConfirmationCode;  //setting was unsuccessful and so send confirmation code
    }
  }
  else {
    return response; //return packet receive error code
  }
}

//=========================================================================//
//deletes all the templates stored in the library

uint8_t clearLibrary () {
  #ifdef FPS_DEBUG
    printf("Clearing library..");
  #endif

  sendPacket(FPS_ID_COMMANDPACKET, FPS_CMD_CLEARLIBRARY); //send the command
  uint8_t response = receivePacket(); //read response

  if(response == FPS_RX_OK) { //if the response packet is valid
    if(fp.rxConfirmationCode == FPS_RESP_OK) { //the confirm code will be saved when the response is received
      #ifdef FPS_DEBUG
        printf("Clearing library success.");
      #endif
      return FPS_RESP_OK; //just the confirmation code only
    }
    else {
      #ifdef FPS_DEBUG
        printf("Clearing library failed.");
        printf("rxConfirmationCode = ");
        printf("%0#10x",fp.rxConfirmationCode);
      #endif
      return fp.rxConfirmationCode;  //setting was unsuccessful and so send confirmation code
    }
  }
  else {
    return response; //return packet receive error code
  }
}

//=========================================================================//
//deletes all the templates stored in the library

uint8_t matchTemplates () {
  #ifdef FPS_DEBUG
    printf("Matching templates..");
  #endif

  sendPacket(FPS_ID_COMMANDPACKET, FPS_CMD_MATCHTEMPLATES); //send the command
  uint8_t response = receivePacket(); //read response

  if(response == FPS_RX_OK) { //if the response packet is valid
    if(fp.rxConfirmationCode == FPS_RESP_OK) { //the confirm code will be saved when the response is received
      #ifdef FPS_DEBUG
        printf("Matching templates success.");
      #endif

      fp.matchScore = (uint16_t)(rxDataBuffer[1] << 8) + rxDataBuffer[0];
      return FPS_RESP_OK; //just the confirmation code only
    }
    else {
      #ifdef FPS_DEBUG
        printf("The templates do no match.");
        printf("rxConfirmationCode = ");
        printf("%0#10x",fp.rxConfirmationCode);
      #endif
      return fp.rxConfirmationCode;  //setting was unsuccessful and so send confirmation code
    }
  }
  else {
    return response; //return packet receive error code
  }
}

//=========================================================================//
//searches the contents of one of the two char buffers for a match on the
//fingerprint library throughout a range

uint8_t searchLibrary (uint8_t bufferId, uint16_t startLocation, uint16_t count) {
  if(!((bufferId > 0) && (bufferId < 3))) { //if the value is not 1 or 2
    #ifdef FPS_DEBUG
      printf("Searching library failed.");
      printf("Bad value. bufferId can only be 1 or 2.");
      printf("bufferId = ");
      printf("%d",bufferId);
    #endif
    return FPS_BAD_VALUE;
  }

  if((startLocation > 1000) || (startLocation < 1)) { //if not in range (0-999)
    #ifdef FPS_DEBUG
      printf("Searching library failed.");
      printf("Bad start ID");
      printf("startId = #");
      printf("%d",startLocation);
    #endif
    return FPS_BAD_VALUE;
  }

  if((startLocation + count) > 1001) { //if range overflows
    #ifdef FPS_DEBUG
      printf("Searching library failed.");
      printf("startLocation + count can't be greater than 1001.");
      printf("startLocation = #");
      printf("%d",startLocation);
      printf("count = ");
      printf("%d",count);
      printf("startLocation + count = ");
      printf("%d",startLocation + count);
    #endif
    return FPS_BAD_VALUE;
  }

  uint8_t dataArray[5] = {0};
  dataArray[4] = bufferId;
  dataArray[3] = (startLocation >> 8) & 0xFFU;  //high byte
  dataArray[2] = (startLocation & 0xFFU); //low byte
  dataArray[1] = (count >> 8) & 0xFFU; //high byte
  dataArray[0] = (count & 0xFFU); //low byte

  #ifdef FPS_DEBUG
    printf("Starting searching library for buffer content.");
    printf("bufferId = ");
    printf("%d",bufferId);
    printf("startLocation = #");
    printf("%d",startLocation);
    printf("count = ");
    printf("%d",count);
    printf("startLocation + count = ");
    printf("%d",startLocation + count);
  #endif

  sendPacket(FPS_ID_COMMANDPACKET, FPS_CMD_SEARCHLIBRARY, dataArray, 5); //send the command
  uint8_t response = receivePacket(); //read response

  if(response == FPS_RX_OK) { //if the response packet is valid
    if(fp.rxConfirmationCode == FPS_RESP_OK) { //the confirm code will be saved when the response is received
      fp.fingerId = (uint16_t)(fp.rxDataBuffer[3] << 8) + fp.rxDataBuffer[2];  //add high byte and low byte
      fp.fingerId += 1;  //because IDs start from #1
      fp.matchScore = (uint16_t)(fp.rxDataBuffer[1] << 8) + fp.rxDataBuffer[0];  //add high byte and low byte
      
      #ifdef FPS_DEBUG
        printf("Buffer content found in library.");
        printf("fingerId = #");
        printf("%d",fp.fingerId);
        printf("matchScore = ");
        printf("%d",fp.matchScore);
      #endif

      return FPS_RESP_OK; //just the confirmation code only
    }
    else {
      //fingerId = 0 doesn't mean the match was found at location 0
      //instead it means an error. check the confirmation code to determine the problem
      fp.fingerId = 0;
      fp.matchScore = 0;

      #ifdef FPS_DEBUG
        printf("Fingerprint not found.");
        printf("rxConfirmationCode = ");
        printf("%0#10x",fp.rxConfirmationCode);
      #endif
      
      return fp.rxConfirmationCode;
    }
  }
  else {
    return response; //return packet receive error code
  }
}