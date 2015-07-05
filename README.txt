
Simple keyboard for Arduino Boards with ATmega32U4 chip. 

Configurable with the inbuild virtual serial port (USB). 

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
key3    5             18    keyboardTableSelectPin (push-button)
key4    6             15    LED3
key5    7             14    LED2
key6    8             16    LED1
key7    9             10    LED0



Available commands:

 s0 s1 s2 s3
 list or l
 k[keynumber] [keycode]       e.g. k0 65
 write or w
 raw   or r
 factory
 help  or h
 


...
                     Key  0 - 7
                     Keycode/Character 


Set 0:   97/a    115/s    119/w    122/z    101/e    100/d    32/     176/�    

Set 1:   49/1    50/2    51/3    52/4    53/5    54/6    55/7    56/8    

Set 2:   65/A    66/B    67/C    68/D    69/E    70/F    71/G    72/H    

Set 3:   194/�    195/�    196/�    197/�    198/�    199/�    200/�    201/�    


 Selected keyboard table: 0
...

k0 65
...

2015-02-26  initial release
