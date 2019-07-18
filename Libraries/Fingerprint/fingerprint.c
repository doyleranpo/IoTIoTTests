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
    txDataBuffer = data;
    txDataBufferLength = dataLength;
  }
  else {
    txDataBuffer = NULL;
    txDataBufferLength = 0;
  }

  txPacketType = type; //type of packet - 1 byte
  txInstructionCode = command; //instruction code - 1 byte
  txPacketLengthL = txDataBufferLength + 3; //1 byte for command, 2 bytes for checksum
  txPacketLength[0] = txPacketLengthL & 0xFFU; //get lower byte
  txPacketLength[1] = (txPacketLengthL >> 8) & 0xFFU; //get high byte

  txPacketChecksumL = txPacketType + txPacketLength[0] + txPacketLength[1] + txInstructionCode; //sum of packet ID and packet length bytes

  for(int i=0; i<txDataBufferLength; i++) {
    txPacketChecksumL += txDataBuffer[i]; //add rest of the data bytes
  }

  txPacketChecksum[0] = txPacketChecksumL & 0xFFU; //get low byte
  txPacketChecksum[1] = (txPacketChecksumL >> 8) & 0xFFU; //get high byte

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
    printf();
    printf("txInstructionCode = ");
    printf("%0#10x",fp.txInstructionCode);
    printf("txDataBufferLength = ");
    printf("%0#10x",fp.txDataBufferLength);
    printf("txPacketLengthL = ");
    printf(fp.txPacketLengthL);
    // printf("rxPacketLength[] = ");
    // printf(rxPacketLength[1]);
    // printf("-");
    // printfln(rxPacketLength[0]);
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
    print("\n");
    printf("Reading response.");
  #endif

  while (timeout > 0) {
    if(serialDataAvail(fp.fd)) {
      byteBuffer = serialGetchar(fp.fd);
      #ifdef FPS_DEBUG
        // debugPort.print("Response byte found = ");
        // debugPort.println(byteBuffer, HEX);
      #endif
      serialBuffer[serialBufferLength] = byteBuffer;
      serialBufferLength++;
    }

    timeout--;
    delay(1);
  }

  if(serialBufferLength == 0) {
    #ifdef FPS_DEBUG
      debugPort.println("Serial timed out.");
      debugPort.println("This usually means the baud rate is not correct.");
    #endif
    return FPS_RX_TIMEOUT;
  }

  if(serialBufferLength < 10) {
    #ifdef FPS_DEBUG
      debugPort.println("Received bad packet with length < 10");
    #endif
    return FPS_RX_BADPACKET;
  }

  uint16_t token = 0;

  while(true) {
    switch (token) {
      case 0: //test packet start codes
        if(serialBuffer[token] == startCode[1])
          break;
        else {
          #ifdef FPS_DEBUG //enable it to get debug info
            debugPort.println("Error at 0 : Start Code");
            debugPort.print("Received packet = ");
            for(int i=0; i < serialBufferLength; i++) {
              debugPort.print(serialBuffer[i], HEX);
              if(i != (serialBufferLength - 1)) {
                debugPort.print("-");
              }
            }
            debugPort.println();
          #endif

          return FPS_RX_BADPACKET;
        }

      case 1:
        if(serialBuffer[token] == startCode[0])
          break;
        else {
          #ifdef FPS_DEBUG
            debugPort.println("Error at 1 : Start Code");
            debugPort.print("Received packet = ");
            for(int i=0; i < serialBufferLength; i++) {
              debugPort.print(serialBuffer[i], HEX);
              if(i != (serialBufferLength - 1)) {
                debugPort.print("-");
              }
            }
            debugPort.println();
          #endif

          return FPS_RX_BADPACKET;
        }

      case 2: //test device address
        if(serialBuffer[token] == deviceAddress[3])
          break;
        else {
          #ifdef FPS_DEBUG
            debugPort.println("Error at 2 : Device Address");
            debugPort.print("Received packet = ");
            for(int i=0; i < serialBufferLength; i++) {
              debugPort.print(serialBuffer[i], HEX);
              if(i != (serialBufferLength - 1)) {
                debugPort.print("-");
              }
            }
            debugPort.println();
          #endif

          return FPS_RX_BADPACKET;
        }
      
      case 3:
        if(serialBuffer[token] == deviceAddress[2])
          break;
        else {
          #ifdef FPS_DEBUG
            debugPort.println("Error at 3 : Device Address");
            debugPort.print("Received packet = ");
            for(int i=0; i < serialBufferLength; i++) {
              debugPort.print(serialBuffer[i], HEX);
              if(i != (serialBufferLength - 1)) {
                debugPort.print("-");
              }
            }
            debugPort.println();
          #endif

          return FPS_RX_BADPACKET;
        }

      case 4:
        if(serialBuffer[token] == deviceAddress[1])
          break;
        else {
          #ifdef FPS_DEBUG
            debugPort.println("Error at 4 : Device Address");
            debugPort.print("Received packet = ");
            for(int i=0; i < serialBufferLength; i++) {
              debugPort.print(serialBuffer[i], HEX);
              if(i != (serialBufferLength - 1)) {
                debugPort.print("-");
              }
            }
            debugPort.println();
          #endif

          return FPS_RX_BADPACKET;
        }
      
      case 5:
        if(serialBuffer[token] == deviceAddress[0])
          break;
        else {
          #ifdef FPS_DEBUG
            debugPort.println("Error at 5 : Device Address");
            debugPort.print("Received packet = ");
            for(int i=0; i < serialBufferLength; i++) {
              debugPort.print(serialBuffer[i], HEX);
              if(i != (serialBufferLength - 1)) {
                debugPort.print("-");
              }
            }
            debugPort.println();
          #endif

          return FPS_RX_BADPACKET;
        }

      case 6: //test for valid packet type
        if((serialBuffer[token] == FPS_ID_COMMANDPACKET) || (serialBuffer[token] == FPS_ID_DATAPACKET) || (serialBuffer[token] == FPS_ID_ACKPACKET) || (serialBuffer[token] == FPS_ID_ENDDATAPACKET)) {
          rxPacketType = serialBuffer[token]; //store the packet ID to class variable
          break;
        }
        else {
          #ifdef FPS_DEBUG
            debugPort.println("Error at 6 : Unknown Response");
            debugPort.print("Received packet = ");
            for(int i=0; i < serialBufferLength; i++) {
              debugPort.print(serialBuffer[i], HEX);
              if(i != (serialBufferLength - 1)) {
                debugPort.print("-");
              }
            }
            debugPort.println();
          #endif

          return FPS_RX_WRONG_RESPONSE;
        }

      case 7: //read packet data length
        if((serialBuffer[token] > 0) || (serialBuffer[token + 1] > 0)) {
          rxPacketLength[0] = serialBuffer[token + 1];  //lower byte
          rxPacketLength[1] = serialBuffer[token];  //higher byte
          rxPacketLengthL = uint16_t(rxPacketLength[1] << 8) + rxPacketLength[0]; //calculate the full length value
          rxDataBufferLength = rxPacketLengthL - 3; //subtract 2 for checksum and 1 for command
          token++; //because we read one additional bytes here
          break;
        }

        else {
         #ifdef FPS_DEBUG
            debugPort.println("Error at 7 : Unknown Response");
            debugPort.print("Received packet = ");
            for(int i=0; i < serialBufferLength; i++) {
              debugPort.print(serialBuffer[i], HEX);
              if(i != (serialBufferLength - 1)) {
                debugPort.print("-");
              }
            }
            debugPort.println();
          #endif

          return FPS_RX_WRONG_RESPONSE;
        }

      case 9: //read confirmation or instruction code
        rxConfirmationCode = serialBuffer[token]; //the first byte of data will be either instruction or confirmation code
        break;

      case 10: //read data
        for(int i=0; i < rxDataBufferLength; i++) {
          rxDataBuffer[(rxDataBufferLength - 1) - i] = serialBuffer[token + i]; //store low values at start of the rxDataBuffer array
        }
        break;
      
      case 11: //read checksum
        if(rxDataBufferLength == 0) { //sometimes there's no data other than the confirmation code
          rxPacketChecksum[0] = serialBuffer[token]; //lower byte
          rxPacketChecksum[1] = serialBuffer[token - 1]; //high byte
          rxPacketChecksumL = uint16_t(rxPacketChecksum[1] << 8) + rxPacketChecksum[0]; //calculate L value

          uint16_t tempSum = 0; //temp checksum 

          tempSum = rxPacketType + rxPacketLength[0] + rxPacketLength[1] + rxConfirmationCode;

          if(rxPacketChecksumL == tempSum) { //check if the calculated checksum matches the received one
            #ifdef FPS_DEBUG
              debugPort.println("Checksums match success.");
              debugPort.print("Received = ");
              debugPort.print(rxPacketChecksum[1], HEX);
              debugPort.print("-");
              debugPort.println(rxPacketChecksum[0], HEX);
              debugPort.print("Received L = " );
              debugPort.println(rxPacketChecksumL, HEX);
              debugPort.print("Calculated = ");
              debugPort.print(byte(tempSum >> 8), HEX);
              debugPort.print("-");
              debugPort.println(byte(tempSum & 0xFFU), HEX);
              debugPort.print("Calculated L = ");
              debugPort.println(tempSum, HEX);
              debugPort.print("Received packet = ");

              for(int i=0; i < serialBufferLength; i++) {
                debugPort.print(serialBuffer[i], HEX);
                if(i != (serialBufferLength - 1)) {
                  debugPort.print("-");
                }
              }
              debugPort.println();
              debugPort.print("Data stream = none");

              debugPort.println();
              debugPort.print("rxConfirmationCode = ");
              debugPort.println(rxConfirmationCode, HEX);
              debugPort.print("rxDataBufferLength = ");
              debugPort.println(rxDataBufferLength, HEX);
              debugPort.print("rxPacketLengthL = ");
              debugPort.println(rxPacketLengthL);
              debugPort.print("rxPacketLength[] = ");
              debugPort.print(rxPacketLength[1], HEX);
              debugPort.print("-");
              debugPort.println(rxPacketLength[0], HEX);
            #endif

            return FPS_RX_OK; //packet read success
          }

          else { //if the checksums do not match
            #ifdef FPS_DEBUG
              debugPort.println("Checksums match fail.");
              debugPort.print("Received = ");
              debugPort.print(rxPacketChecksum[1], HEX);
              debugPort.print("-");
              debugPort.println(rxPacketChecksum[0], HEX);
              debugPort.print("Received L = " );
              debugPort.println(rxPacketChecksumL, HEX);
              debugPort.print("Calculated = ");
              debugPort.print(byte(tempSum >> 8), HEX);
              debugPort.print("-");
              debugPort.println(byte(tempSum & 0xFFU), HEX);
              debugPort.print("Calculated L = ");
              debugPort.println(tempSum, HEX);
              debugPort.print("Received packet = ");

              for(int i=0; i < serialBufferLength; i++) {
                debugPort.print(serialBuffer[i], HEX);
                if(i != (serialBufferLength - 1)) {
                  debugPort.print("-");
                }
              }
              debugPort.println();
              debugPort.print("Data stream = none");
              
              debugPort.println();
              debugPort.print("rxConfirmationCode = ");
              debugPort.println(rxConfirmationCode, HEX);
              debugPort.print("rxDataBufferLength = ");
              debugPort.println(rxDataBufferLength, HEX);
              debugPort.print("rxPacketLengthL = ");
              debugPort.println(rxPacketLengthL, HEX);
              debugPort.print("rxPacketLength[] = ");
              debugPort.print(rxPacketLength[1], HEX);
              debugPort.print("-");
              debugPort.println(rxPacketLength[0], HEX);
            #endif

            return FPS_RX_BADPACKET;  //then that's an error
          }
          break;
        }

        //-------------------------------------------------------------------------//

        else if((serialBuffer[token + (rxDataBufferLength-1)] > 0) || ((serialBuffer[token + 1 + (rxDataBufferLength-1)] > 0))) {
          rxPacketChecksum[0] = serialBuffer[token + 1 + (rxDataBufferLength-1)]; //lower byte
          rxPacketChecksum[1] = serialBuffer[token + (rxDataBufferLength-1)]; //high byte
          rxPacketChecksumL = uint16_t(rxPacketChecksum[1] << 8) + rxPacketChecksum[0]; //calculate L value

          uint16_t tempSum = 0; //temp checksum 

          tempSum = rxPacketType + rxPacketLength[0] + rxPacketLength[1] + rxConfirmationCode;

          for(int i=0; i < rxDataBufferLength; i++) {
            tempSum += rxDataBuffer[i]; //calculate data checksum
          }

          if(rxPacketChecksumL == tempSum) { //check if the calculated checksum matches the received one
            #ifdef FPS_DEBUG
              debugPort.println("Checksums match success.");
              debugPort.print("Received = ");
              debugPort.print(rxPacketChecksum[1], HEX);
              debugPort.print("-");
              debugPort.println(rxPacketChecksum[0], HEX);
              debugPort.print("Received L = " );
              debugPort.println(rxPacketChecksumL, HEX);
              debugPort.print("Calculated = ");
              debugPort.print(byte(tempSum >> 8), HEX);
              debugPort.print("-");
              debugPort.println(byte(tempSum & 0xFFU), HEX);
              debugPort.print("Calculated L = ");
              debugPort.println(tempSum, HEX);
              debugPort.print("Received packet = ");

              for(int i=0; i < serialBufferLength; i++) {
                debugPort.print(serialBuffer[i], HEX);
                if(i != (serialBufferLength - 1)) {
                  debugPort.print("-");
                }
              }
              debugPort.println();
              debugPort.print("Data stream = ");

              for(int i=0; i < rxDataBufferLength; i++) {
                debugPort.print(rxDataBuffer[(rxDataBufferLength-1) - i], HEX);
                if(i != (rxDataBufferLength - 1)) {
                  debugPort.print("-");
                }
              }

              debugPort.println();
              debugPort.print("rxConfirmationCode = ");
              debugPort.println(rxConfirmationCode, HEX);
              debugPort.print("rxDataBufferLength = ");
              debugPort.println(rxDataBufferLength, HEX);
              debugPort.print("rxPacketLengthL = ");
              debugPort.println(rxPacketLengthL, HEX);
              debugPort.print("rxPacketLength[] = ");
              debugPort.print(rxPacketLength[1], HEX);
              debugPort.print("-");
              debugPort.println(rxPacketLength[0], HEX);
            #endif

            return FPS_RX_OK; //packet read success
          }

          else { //if the checksums do not match
            #ifdef FPS_DEBUG
              debugPort.println("Checksums match fail.");
              debugPort.print("Received = ");
              debugPort.print(rxPacketChecksum[1], HEX);
              debugPort.print("-");
              debugPort.println(rxPacketChecksum[0], HEX);
              debugPort.print("Received L = " );
              debugPort.println(rxPacketChecksumL, HEX);
              debugPort.print("Calculated = ");
              debugPort.print(byte(tempSum >> 8), HEX);
              debugPort.print("-");
              debugPort.println(byte(tempSum & 0xFFU), HEX);
              debugPort.print("Calculated L = ");
              debugPort.println(tempSum, HEX);
              debugPort.print("Received packet = ");

              for(int i=0; i < serialBufferLength; i++) {
                debugPort.print(serialBuffer[i], HEX);
                if(i != (serialBufferLength - 1)) {
                  debugPort.print("-");
                }
              }
              debugPort.println();
              debugPort.print("Data stream = ");

              for(int i=0; i < rxDataBufferLength; i++) {
                debugPort.print(rxDataBuffer[(rxDataBufferLength-1) - i], HEX);
                if(i != (rxDataBufferLength - 1)) {
                  debugPort.print("-");
                }
              }
              
              debugPort.println();
              debugPort.print("rxConfirmationCode = ");
              debugPort.println(rxConfirmationCode, HEX);
              debugPort.print("rxDataBufferLength = ");
              debugPort.println(rxDataBufferLength, HEX);
              debugPort.print("rxPacketLengthL = ");
              debugPort.println(rxPacketLengthL, HEX);
              debugPort.print("rxPacketLength[] = ");
              debugPort.print(rxPacketLength[1], HEX);
              debugPort.print("-");
              debugPort.println(rxPacketLength[0], HEX);
            #endif

            return FPS_RX_BADPACKET;  //then that's an error
          }
          break;
        }

        //-------------------------------------------------------------------------//

        else { //if the checksum received is 0
          #ifdef FPS_DEBUG
            debugPort.println("Error at 12 : Checksum");
            debugPort.print("Received packet = ");
            for(int i=0; i < serialBufferLength; i++) {
              debugPort.print(serialBuffer[i], HEX);
              if(i != (serialBufferLength - 1)) {
                debugPort.print("-");
              }
            }
            debugPort.println();
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
