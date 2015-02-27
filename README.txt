
Simple keyboard for Arduino Boards with ATmega32U4 chips. 

Configurable with inbuild serial port (USB). 

4 Keysets a 8 keys available.

e.g. Pro Micro or Leonardo
https://www.sparkfun.com/products/12640
http://arduino.cc/en/Main/ArduinoBoardLeonardo


      Pro Micro   AtMega32U4

            USB-CON
        1             RAW
        0             GND   GND
        GND           RST
        GND           VCC
key0    2             21
key1    3             20
key2    4             19
key3    5             18    keyboardTableSelectPin
key4    6             15    LED3
key5    7             14    LED2
key6    8             16    LED1
key7    9             10    LED0



Available commands:

 s0 s1 s2 s3
 set+ or +
 set- or -
 list or l
 k[keynumber] [keycode]       e.g. k0 65
 write or w
 raw   or r
 factory
 help  or h
 



2015-02-26  fist release
