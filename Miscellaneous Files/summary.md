**Microcontrollers:**

* A microcontroller is a device which contains a processor, memory and peripherals allowing it’s usage for embedded systems.
* There are various types :-
* 8 bit
* 32 bit
* Each microcontroller requires a timer/counter
* It counts the clock cycles of the system clock
* Timers are usually configurable as 8,24 or 32 bit
* Prescalars are used to determine when the counter needs to be updated.

**Watchdog Timers**

* Special failsafe mechanism
* Counter starts at a particular value and counts down to 0
* At 0 the system is reset

**ADC:**

* ADC converts analog voltage on a pin to its equivalent digital value 
* The value is calculated using the following formula
* V=(ADC resoulution/(Reference voltage*ADC Value)
* The resolution of ADC depends on the size of the ADC
* For example an 8 bit ADC has 256 discrete levels, a 10 bit has 1024 levels.
          The generalised formula is 2n
![adc](/uploads/ef8f17c7df6df9f8f154b9d4a808b152/adc.png)

**General Purpose I/O pins:**


* General pins which can be controlled by the user
* They arranged in ports.
* The total no. of pins assigned to every port depends on the architecture!

**Serial Communication**

* To communicate with other attached devices the MCU uses various techniques
* These techniques need to follow a set of rules or protocols
* Commonly used protocols include 
* UART
* SPI
* I2C

*Universal Asynchronous Receiver/Transmitter*

* Data tranfer is done without a clock signal
* It requires 2 lines (RX and TX)
* Data is to be framed consisting of: Start bit : Data Bits : Parity Bit : Stop bit
* Both sides need to have a set transfer speed
![uart](/uploads/20a1d4d658b0fcf8c6d5f8df3514515b/uart.png)

*Serial Preipheral Interface*

* It is synchronous and requires a clock signal
* It utilizes 4 lines for communication
![spi1](/uploads/9412e103c40a3b5bf8c15f99cbc1b2fb/spi1.png)

* It is full  duplex and supports clock upto 10MHz
* Can have multiple slaves but each slave requires a seperate Slave Select(SS) line
![spi2](/uploads/b5b4c921de1015c82993c9dc36c93334/spi2.png)

*Inter-integrated circuit*
* Consists of 2 lines i.e. SCL and SDA
* It is half duplex and can support clock speeds upto 400kHz
* The diagram shows transmission of 7 bits using I2C
![i2c](/uploads/6d95b8146ed0f52e551552ecb1791f40/i2c.png)

**Polling and Interrupts**

* Two ways a MCU reads any peripheral
* Polling is a method used to perform tasks
* The CPU constantly monitors a variable/register to check if it needs to perform a particular task.
* Interrupts are signals which take the CPU away from the main code to perform a particular task/code stored in a branch called Interrupt Service Routine.
* Interrupts are more efficient than polling as they don’t require constant monitoring.

**Pulse width modulation**

* It is used to generate a varying dc signal using a digital source by changing the ON time of the pulse. 
* Increasing ON time increases average voltage. 
* The TON/Total Time period is called the Duty Cycle
![pwm](/uploads/58e1d29d4215916be546aded7d3509bf/pwm.png)

**Embedded C Programming**

* C programming on embedded devices
* The data types used are shown in the image below
![datatypes](/uploads/fbdc2d53150319d380a5fcd39ce50cdb/datatypes.png)


*Endianess*

* Used to define the way in which data is stored
* Big endian: MSB occupy the lower address 
* Little endian: LSB occupy the lower address
          
*Hexadecimal notation*

* Generally in a microcontroller the registers are 8,16 or 32 bit.
* Thus hexadecimal notation is used to write to these registers
* Other notations include Binary and Octal

**Useful Keywords**

*typedef*: It is used to explitly associate a data type with an identifier.
* Eg. typdef signed short int16_t;
* Here signed short can now be referred to using int16_t.

*Struct*: It is used for declaring a structure which can hold variables of different data types under a single name.
* The .(dot) and -> operators are used to access member variables.
