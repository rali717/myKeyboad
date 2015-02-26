
//==============================================================================
//====   myKeyboard   ====  2015-02  ===== Rainer Lietz ===== license: CC0 =====
//==============================================================================

#include <EEPROM.h>

//====  Hardware I/O  settings  ================================================

int const keyPin[8] = {2, 3, 4, 5, 6, 7, 8, 9}; //Set logical input pin (micro Pro)

int const keyboardTableSelectPin = 18;  // for selecting one of the 4 possible keyboard sets

int const led0 = 10;
int const led1 = 16;
int const led2 = 14;
int const led3 = 15;

//==============================================================================

byte keyboardTable = 0;      // 0-3   4 Keyboard Sets available

int const key[8] = {0, 1, 2, 3, 4, 5, 6, 7};    // Index for arrays = the key number

bool keyPressed[8] = {0, 0, 0, 0, 0, 0, 0, 0};  //keyPressed:  set all set to false


byte keyChar[4][8] = {						// Keytables
  {'a', 's', 'w', 'z', 'e', 'd', ' ', 176},
  {'1', '2', '3', '4', '5', '6', '7', '8'},
  {'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H'},
  {194, 195, 196, 197, 198, 199, 200, 201}       //  F1 - F8
};



byte factoryKeyTable[4][8] = {
  {'a', 's', 'w', 'z', 'e', 'd', ' ', 176},
  {'1', '2', '3', '4', '5', '6', '7', '8'},
  {'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H'},
  {194, 195, 196, 197, 198, 199, 200, 201}       //  F1 - F8
};

String inData;  // for serial-in  commands
String stringTmp;

int iKey;
int iKeycode;



//==============================================================================
//====   setup   ===============================================================
//==============================================================================


void setup() {

  pinMode(keyPin[0], INPUT_PULLUP);
  pinMode(keyPin[1], INPUT_PULLUP);
  pinMode(keyPin[2], INPUT_PULLUP);
  pinMode(keyPin[3], INPUT_PULLUP);
  pinMode(keyPin[4], INPUT_PULLUP);
  pinMode(keyPin[5], INPUT_PULLUP);
  pinMode(keyPin[6], INPUT_PULLUP);
  pinMode(keyPin[7], INPUT_PULLUP);

  pinMode(keyboardTableSelectPin, INPUT_PULLUP);

  pinMode(led0, OUTPUT);
  pinMode(led1, OUTPUT);
  pinMode(led2, OUTPUT);
  pinMode(led3, OUTPUT);


  Serial.begin(115200);

  Keyboard.begin();


  digitalWrite(led0, HIGH);
  digitalWrite(led1, HIGH);
  digitalWrite(led2, HIGH);
  digitalWrite(led3, HIGH);
  delay (200);
  digitalWrite(led0, LOW);
  digitalWrite(led1, LOW);
  digitalWrite(led2, LOW);
  digitalWrite(led3, LOW);

  eepromReadKeyTable();

}//===  end of setup  ==========================================================



//==============================================================================
//====   loop   ================================================================
//==============================================================================


void loop() {

  readKey(keyboardTable, key[0]);
  readKey(keyboardTable, key[1]);
  readKey(keyboardTable, key[2]);
  readKey(keyboardTable, key[3]);
  readKey(keyboardTable, key[4]);
  readKey(keyboardTable, key[5]);
  readKey(keyboardTable, key[6]);
  readKey(keyboardTable, key[7]);

  checkSelectPin();

  readcommand ();


}//====  end of loop  ==========================================================


//==============================================================================
//====   readKey   =============================================================
//==============================================================================


void readKey(byte set, char key) {

  if (!digitalRead(keyPin[key]))
  {
    keyPressed[key] = true;
    Keyboard.press (keyChar[set][key]);
  }

  else  {

    if (keyPressed[key] == true)
    {
      keyPressed[key] = false;
      Keyboard.release(keyChar[set][key]);
    }
  }
}//====  end of readKey  =======================================================


//==============================================================================
//====   checkSelectPin  =======================================================
//==============================================================================


void checkSelectPin () {
  //digitalWrite(led0, HIGH);
  if (!digitalRead(keyboardTableSelectPin))  {
    keyboardTable++;

    if (keyboardTable > 3) {
      keyboardTable = 0;
    }

    Serial.print(" Selected keyboard table: ");
    Serial.println(keyboardTable, DEC);

    delay (180);
    if (keyboardTable > 3) {
      keyboardTable = 0;
    }
  }


  if (keyboardTable == 0)
  {
    digitalWrite(led0, HIGH);
    digitalWrite(led1, LOW);
    digitalWrite(led2, LOW);
    digitalWrite(led3, LOW);
  }

  if (keyboardTable == 1)
  {
    digitalWrite(led0, LOW);
    digitalWrite(led1, HIGH);
    digitalWrite(led2, LOW);
    digitalWrite(led3, LOW);
  }


  if (keyboardTable == 2)
  {
    digitalWrite(led0, LOW);
    digitalWrite(led1, LOW);
    digitalWrite(led2, HIGH);
    digitalWrite(led3, LOW);
  }


  if (keyboardTable == 3)
  {
    digitalWrite(led0, LOW);
    digitalWrite(led1, LOW);
    digitalWrite(led2, LOW);
    digitalWrite(led3, HIGH);
  }

}//====  end of checkSelectPin  ==============================================


//==============================================================================
//====   serial commands   =====================================================
//==============================================================================


void readcommand () {

  while (Serial.available() > 0)
  {
    char recieved = Serial.read();
    inData += recieved;

    // Process message when new line character is recieved
    if (recieved == '\n')
    {
      Serial.print(inData);
      inData.trim();
      //---------------------------------------------------------
      if (inData == "help" | inData == "h") {

        Serial.println("\n s0 s1 s2 s3");
        Serial.println(" set+ or +");
        Serial.println(" set- or -");
        Serial.println(" list or l");
        Serial.println(" k[keynumber] [keycode]       e.g. k0 65");
        Serial.println(" write or w");
        Serial.println(" raw   or r");
        Serial.println(" factory");
        Serial.println(" help  or h\n");
      }
      //---------------------------------------------------------
      if (inData == "set+" | inData == "+") {
        keyboardTable++;
        if (keyboardTable > 3) {
          keyboardTable = 0;
        }
        Serial.print("\n Selected keyboardTable: ");
        Serial.println(keyboardTable, DEC);
        Serial.println("");
      }
      //---------------------------------------------------------
      if (inData == "set-" | inData == "-") {
        if (keyboardTable == 0) {
          keyboardTable = 4;
        }
        keyboardTable--;
        Serial.print("\n Selected keyboardTable: ");
        Serial.println(keyboardTable, DEC);
        Serial.println("");
      }
      //---------------------------------------------------------
      if (inData == "list" | inData == "l") {


        Serial.println(" ASCII Table ~ Character Map");

        for (int thisByte = 33; thisByte <= 63; thisByte++) {

          Serial.write("\n  ");
          Serial.write(thisByte);
          Serial.print("   KeyCode: ");
          Serial.print(thisByte);
          Serial.write("               ");
          Serial.write(thisByte + ((126 - 33) / 3));
          Serial.print("   KeyCode: ");
          Serial.print(thisByte + ((126 - 33) / 3));
          Serial.write("               ");
          Serial.write(thisByte + (2 * (126 - 33) / 3));
          Serial.print("   KeyCode: ");
          Serial.print(thisByte + (2 * (126 - 33) / 3));
        }

        Serial.println("\n                                                              ~   KeyCode: 126\n");

        Serial.println(" KEY_LEFT_CTRL         128");
        Serial.println(" KEY_LEFT_SHIFT        129");
        Serial.println(" KEY_LEFT_ALT          130");
        Serial.println(" KEY_LEFT_GUI          131");
        Serial.println(" KEY_RIGHT_CTRL        132");
        Serial.println(" KEY_RIGHT_SHIFT       133");
        Serial.println(" KEY_RIGHT_ALT         134");
        Serial.println(" KEY_RIGHT_GUI         135");
        Serial.println(" KEY_UP_ARROW          218");
        Serial.println(" KEY_DOWN_ARROW        217");
        Serial.println(" KEY_LEFT_ARROW        216");
        Serial.println(" KEY_RIGHT_ARROW       215");
        Serial.println(" KEY_BACKSPACE         178");
        Serial.println(" KEY_TAB               179");
        Serial.println(" KEY_RETURN            176");
        Serial.println(" KEY_ESC               177");
        Serial.println(" KEY_INSERT            209");
        Serial.println(" KEY_DELETE            212");
        Serial.println(" KEY_PAGE_UP           211");
        Serial.println(" KEY_PAGE_DOWN         214");
        Serial.println(" KEY_HOME              210");
        Serial.println(" KEY_END               213");
        Serial.println(" KEY_CAPS_LOCK         193");
        Serial.println(" KEY_F1                194");
        Serial.println(" KEY_F2                195");
        Serial.println(" KEY_F3                196");
        Serial.println(" KEY_F4                197");
        Serial.println(" KEY_F5                198");
        Serial.println(" KEY_F6                199");
        Serial.println(" KEY_F7                200");
        Serial.println(" KEY_F8                201");
        Serial.println(" KEY_F9                202");
        Serial.println(" KEY_F10               203");
        Serial.println(" KEY_F11               204");
        Serial.println(" KEY_F12               205");


        Serial.print("\n\n                     Key  0 - 7\n");
        Serial.print("                     Keycode/Character \n");
        for (int s = 0; s <= 3 ; s++) {

          Serial.print("\n\n Set ");
          Serial.print(s, DEC);
          Serial.print(":    ");

          for (int i = 0; i <= 7 ; i++) {
            Serial.print(keyChar[s][i], DEC);
            Serial.print("/");
            Serial.write(keyChar[s][i]);
            Serial.print("    ");
          }
        }
        Serial.print("\n\n\n Selected keyboard table: ");
        Serial.println(keyboardTable, DEC);


        Serial.println("\n k[keynumber] [keycode]");
        Serial.println(" e.g.  k0 65\n");



      }
      //---------------------------------------------------------
      //---------------------------------------------------------
      if (inData == "s0") {
        keyboardTable = 0;

        Serial.print("\n Selected keyboard table: ");
        Serial.println(keyboardTable, DEC);
        Serial.println("");
      }
      //---------------------------------------------------------
      if (inData == "s1") {
        keyboardTable = 1;

        Serial.print("\n Selected keyboard table: ");
        Serial.println(keyboardTable, DEC);
        Serial.println("");
      }
      //---------------------------------------------------------
      if (inData == "s2") {
        keyboardTable = 2;

        Serial.print("\n Selected keyboard table: ");
        Serial.println(keyboardTable, DEC);
        Serial.println("");
      }
      //---------------------------------------------------------
      if (inData == "s3") {
        keyboardTable = 3;

        Serial.print("\n Selected keyboard table: ");
        Serial.println(keyboardTable, DEC);
        Serial.println("");
      }
      //---------------------------------------------------------
      if (inData == "raw" | inData == "r") {
        for (int i = 0; i <= 3; i++) {
          Serial.write(keyChar[i][0]);
          Serial.write(keyChar[i][1]);
          Serial.write(keyChar[i][2]);
          Serial.write(keyChar[i][3]);
          Serial.write(keyChar[i][4]);
          Serial.write(keyChar[i][5]);
          Serial.write(keyChar[i][6]);
          Serial.write(keyChar[i][7]);
        }
        Serial.write(keyboardTable);
      }
      //---------------------------------------------------------


      if (inData == "write" | inData == "w") {
        for (int i = 0; i <= 7; i++)
        {
          EEPROM.write(i, keyChar[0][i]);
          delay(5);
          EEPROM.write(i + 8, keyChar[1][i]);
          delay(5);
          EEPROM.write(i + 16, keyChar[2][i]);
          delay(5);
          EEPROM.write(i + 24, keyChar[3][i]);
          delay(5);
        }
        Serial.println("\n Try to store keytable in eeprom\n");


        bool eepromOK = true;
        for (int i = 0; i <= 7; i++)
        {
          if (!(EEPROM.read(i) == keyChar[0][i])) {
            eepromOK = false;
          };
          if (!(EEPROM.read(i + 8) == keyChar[1][i])) {
            eepromOK = false;
          };
          if (!(EEPROM.read(i + 16) == keyChar[2][i])) {
            eepromOK = false;
          };
          if (!(EEPROM.read(i + 24) == keyChar[3][i])) {
            eepromOK = false;
          };
        }
        if (eepromOK) {
          Serial.println(" EEPROM-WRITE successful\n");
        }
        else  {
          Serial.println(" EEPROM-WRITE failed\n");
        }


        for (int i = 0; i <= 7; i++)
        {
          keyChar[0][i] = EEPROM.read(i);
          keyChar[1][i] = EEPROM.read(i + 8);
          keyChar[2][i] = EEPROM.read(i + 16);
          keyChar[3][i] = EEPROM.read(i + 24);
        }
        Serial.println("\n Read values from EEPROM\n");
      }

      //---------------------------------------------------------

      if (inData == "factory") {

        for (int i = 0; i <= 7; i++)
        {
          keyChar[0][i] = factoryKeyTable[0][i];
          keyChar[1][i] = factoryKeyTable[1][i];
          keyChar[2][i] = factoryKeyTable[2][i];
          keyChar[3][i] = factoryKeyTable[3][i];
        }
        Serial.println("\n Set factory values\n");
      }
      //---------------------------------------------------------


      // parse  e.g.  k0 120


      if ((inData.substring(0, 1) == "k") &&
          (inData.substring(2, 3) == " ") &&
          (inData.length() <= 6))
      {

        stringTmp = inData.substring(1, 2);

        Serial.print("\nFound key: ");
        Serial.println(stringTmp);
        Serial.println(stringTmp.toInt());
        iKey = stringTmp.toInt();

        Serial.println("\n Stringlengh: ");
        Serial.println(inData.length());

        stringTmp = inData.substring(3, inData.length());
        Serial.println(stringTmp.toInt());

        keyChar[keyboardTable][iKey] = stringTmp.toInt();

      }

      //---------------------------------------------------------
      inData = ""; // Clear recieved buffer
    }
  }

}//====   end of serial commands   ===============================================


void eepromReadKeyTable () {
  for (int i = 0; i <= 7; i++)
  {
    keyChar[0][i] = EEPROM.read(i);
    keyChar[1][i] = EEPROM.read(i + 8);
    keyChar[2][i] = EEPROM.read(i + 16);
    keyChar[3][i] = EEPROM.read(i + 24);
  }
  Serial.println("\n Read all Keytables from EEPROM\n");

}


/*
ASCII Table ~ Character Map

  !   KeyCode: 33               @   KeyCode: 64               _   KeyCode: 95
  "   KeyCode: 34               A   KeyCode: 65               `   KeyCode: 96
  #   KeyCode: 35               B   KeyCode: 66               a   KeyCode: 97
  $   KeyCode: 36               C   KeyCode: 67               b   KeyCode: 98
  %   KeyCode: 37               D   KeyCode: 68               c   KeyCode: 99
  &   KeyCode: 38               E   KeyCode: 69               d   KeyCode: 100
  '   KeyCode: 39               F   KeyCode: 70               e   KeyCode: 101
  (   KeyCode: 40               G   KeyCode: 71               f   KeyCode: 102
  )   KeyCode: 41               H   KeyCode: 72               g   KeyCode: 103
  *   KeyCode: 42               I   KeyCode: 73               h   KeyCode: 104
  +   KeyCode: 43               J   KeyCode: 74               i   KeyCode: 105
  ,   KeyCode: 44               K   KeyCode: 75               j   KeyCode: 106
  -   KeyCode: 45               L   KeyCode: 76               k   KeyCode: 107
  .   KeyCode: 46               M   KeyCode: 77               l   KeyCode: 108
  /   KeyCode: 47               N   KeyCode: 78               m   KeyCode: 109
  0   KeyCode: 48               O   KeyCode: 79               n   KeyCode: 110
  1   KeyCode: 49               P   KeyCode: 80               o   KeyCode: 111
  2   KeyCode: 50               Q   KeyCode: 81               p   KeyCode: 112
  3   KeyCode: 51               R   KeyCode: 82               q   KeyCode: 113
  4   KeyCode: 52               S   KeyCode: 83               r   KeyCode: 114
  5   KeyCode: 53               T   KeyCode: 84               s   KeyCode: 115
  6   KeyCode: 54               U   KeyCode: 85               t   KeyCode: 116
  7   KeyCode: 55               V   KeyCode: 86               u   KeyCode: 117
  8   KeyCode: 56               W   KeyCode: 87               v   KeyCode: 118
  9   KeyCode: 57               X   KeyCode: 88               w   KeyCode: 119
  :   KeyCode: 58               Y   KeyCode: 89               x   KeyCode: 120
  ;   KeyCode: 59               Z   KeyCode: 90               y   KeyCode: 121
  <   KeyCode: 60               [   KeyCode: 91               z   KeyCode: 122
  =   KeyCode: 61               \   KeyCode: 92               {   KeyCode: 123
  >   KeyCode: 62               ]   KeyCode: 93               |   KeyCode: 124
  ?   KeyCode: 63               ^   KeyCode: 94               }   KeyCode: 125
                                                              ~   KeyCode: 126


 KEY_LEFT_CTRL 	 	128
 KEY_LEFT_SHIFT 	129
 KEY_LEFT_ALT 	 	130
 KEY_LEFT_GUI 	 	131
 KEY_RIGHT_CTRL 	132
 KEY_RIGHT_SHIFT 	133
 KEY_RIGHT_ALT 	 	134
 KEY_RIGHT_GUI 	 	135
 KEY_UP_ARROW 	 	218
 KEY_DOWN_ARROW 	217
 KEY_LEFT_ARROW 	216
 KEY_RIGHT_ARROW 	215
 KEY_BACKSPACE 	 	178
 KEY_TAB 	 	179
 KEY_RETURN 	 	176
 KEY_ESC 	 	177
 KEY_INSERT 	 	209
 KEY_DELETE 	 	212
 KEY_PAGE_UP 	 	211
 KEY_PAGE_DOWN 	 	214
 KEY_HOME 	 	210
 KEY_END 	 	213
 KEY_CAPS_LOCK 	 	193
 KEY_F1 	 	194
 KEY_F2 	 	195
 KEY_F3 	 	196
 KEY_F4 	 	197
 KEY_F5 	 	198
 KEY_F6 	 	199
 KEY_F7 	 	200
 KEY_F8 	 	201
 KEY_F9 	 	202
 KEY_F10 	 	203
 KEY_F11 	 	204
 KEY_F12 	 	205
*/
/*

      Pro Micro   ATmega32U4

            USB-CONs
        1             RAW
        0             GND   GND
        GND           RST
        GND           VCC
key0    2             21
key1    3             20
key2    4             19
key3    5             18    Pin for select keyboard table
key4    6             15    LED3
key5    7             14    LED2
key6    8             16    LED1
key7    9             10    LED0

*/
//

