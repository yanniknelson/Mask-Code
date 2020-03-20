/*********************************************************************
 This is an example for our nRF51822 based Bluefruit LE modules

 Pick one up today in the adafruit shop!

 Adafruit invests time and resources providing this open source code,
 please support Adafruit and open-source hardware by purchasing
 products from Adafruit!

 MIT license, check LICENSE for more information
 All text above, and the splash screen below must be included in
 any redistribution
*********************************************************************/

#include <Arduino.h>
#include <SPI.h>
#include "Adafruit_BLE.h"
#include "Adafruit_BluefruitLE_SPI.h"
#include "Adafruit_BluefruitLE_UART.h"

#include "BluefruitConfig.h"

#if SOFTWARE_SERIAL_AVAILABLE
  #include <SoftwareSerial.h>
#endif

#include <Wire.h>
#include "Adafruit_LEDBackpack.h"
#include "Adafruit_GFX.h"

Adafruit_8x16matrix left_eye[2] = {Adafruit_8x16matrix(), Adafruit_8x16matrix()};
Adafruit_8x16matrix right_eye[2] = {Adafruit_8x16matrix(), Adafruit_8x16matrix()};


static const uint8_t PROGMEM

X_bmp[4][8] =
{ { B00000111,B00001111,B00011100,B00111000,B01110000,B11100000,B11000000,B10000000
  },

  { B11100000,B11110000,B00111000,B00011100,B00001110,B00000111,B00000011,B00000001
  },

  { B10000000,B11000000,B11100000,B01110000,B00111000,B00011100,B00001111,B00000111
  },

  { B00000001,B00000011,B00000111,B00001110,B00011100,B00111000,B11110000,B11100000
  }
},
At_bmp[4][8] =
{ { B11011000,B01001000,B01001000,B01000100,B00110010,B00011001,B00001000,B00000111
  },
  { B00001101,B00001101,B00001001,B00011011,B00110110,B11000100,B00011100,B11110000
  },
  { B00000111,B00001000,B00010000,B00000000,B00101000,B01101111,B01001101,B11001000
  },
  { B10000000,B01100010,B00010011,B00010001,B00001001,B00001101,B11111101,B00011001
  }
},
O_L_bmp[4][8] =
{ { B00011000,
    B00011000,
    B00011000,
    B00011000,
    B00001000,
    B00001110,
    B00000111,
    B00000001
  },
  { B00110000,B00010000,B00011000,B00011000,B00010000,B01110000,B11100000,B10000000
  },
  { B00000000,B00000000,B00000000,B00000000,B00000000,B00000000,B00000111,B00001111
  },
  { B00000000,B00000000,B00000000,B00000000,B00000000,B00000000,B11100000,B11100000
  }
},

O_R_bmp[4][8] =
{ { B01000000,
    B01100000,
    B00111111,
    B00001111,
    B00000000,
    B00000000,
    B00000000,
    B00000000
  },
  { B00010000,B00010000,B11100000,B11000000,B00000000,B00000000,B00000000,B00000000
  },
  { B00000000,B00111111,B01111111,B01000000,B11000000,B11000000,B11000000,B11000000
  },
  { B00000000,B11100000,B11100000,B00100000,B00110000,B00010000,B00011000,B00011000
  }
},


Hash_bmp[4][8] =
{ { B11111100,B00001111,B00001111,B00001100,B00001100,B00001100,B00001100,B00001100
  },
  { B00010000,B11110000,B11110000,B00011111,B00010011,B00010000,B00010000,B00010000
  },
  { B00001100,B00001100,B11111100,B11111111,B00001111,B00001100,B00001100,B11101100
  },
  { B00010000,B00010000,B00010000,B11111000,B11111111,B00010011,B00010000,B00010000
  }
}
,
Caret_bmp[4][8] =
{ { B01110000,B00111100,B00001111,B00000011,B00000000,B00000000,B00000000,B00000000
  },
  { B00000000,B00000000,B10000000,B10000000,B10000000,B00000000,B00000000,B00000000
  },
  { B00000000,B00000000,B00000000,B00000000,B00000011,B00001111,B00111100,B01110000
  },
  { B00000000,B00000000,B00000000,B10000000,B10000000,B10000000,B00000000,B00000000
  }
},


Tilde_bmp[4][8] =
{ { B00000110,B00000110,B00001100,B00001100,B00001110,B00000110,B00000000,B00000000
  },
  { B00000000,B00000000,B00000000,B00000000,B00000000,B00000000,B00000000,B00000000
  },
  { B00000000,B00000000,B00000000,B00001110,B00000110,B00000011,B00000111,B00000111
  },
  { B00000000,B00000000,B00000000,B00000000,B00000000,B00000000,B00000000,B00000000,
  }
}
,

Asterisk_bmp[4][8] =
{ { B00111111,B00000001,B00000011,B00000111,B00000110,B00000000,B00000000,B00000000
  },
  { B11000000,B11000000,B01100000,B01110000,B00110000,B00000000,B00000000,B00000000
  },
  { B00000000,B00000000,B00000000,B00000110,B00000111,B00000011,B00000001,B00111111
  },
  { B00000000,B00000000,B00000000,B00000000,B00110000,B01110000,B01100000,B11000000
  }
},

Backslash_bmp[4][8] =
{ { B00000011,B00000111,B00001110,B00011100,B00111000,B01110000,B11100000,B11000000
  },
  { B10000000,B00000000,B00000000,B00000000,B00000000,B00000000,B00000000,B00000000
  },
  { B00000000,B00000000,B00000000,B00000000,B00000000,B00000000,B00000000,B00000001
  },
  { B00000011,B00000111,B00001110,B00011100,B00111000,B01110000,B11100000,B11000000
  }
},

Slash_bmp[4][8] =
{ { B00000001,B00000000,B00000000,B00000000,B00000000,B00000000,B00000000,B00000000
  },
  { B11000000,B11100000,B01110000,B00111000,B00011100,B00001110,B00000111,B00000011
  },
  { B11000000,B11100000,B01110000,B00111000,B00011100,B00001110,B00000111,B00000011
  },
  { B00000000,B00000000,B00000000,B00000000,B00000000,B00000000,B00000000,B10000000
  }
},

Equal_bmp[4][8] =
{ { B00001110,B00001110,B00001110,B00001110,B00001110,B00001110,B00001110,B00001110
  },
  { B01110000,B01110000,B01110000,B01110000,B01110000,B01110000,B01110000,B01110000
  },
  { B00001110,B00001110,B00001110,B00001110,B00001110,B00001110,B00001110,B00001110
  },
  { B01110000,B01110000,B01110000,B01110000,B01110000,B01110000,B01110000,B01110000
  }
},

Nine_bmp[4][8] =
{ { B11000001,B11000011,B11000011,B01111110,B01111110,B00000000,B00000000,B00000000
  },
  { B00000011,B00000010,B00000010,B00001110,B00001100,B00000000,B00000000,B00000000
  },
  { B00000000,B00000000,B00000000,B01111111,B01111111,B11000011,B11000011,B11000001
  },
  { B00000000,B00000000,B00000000,B11110000,B11111100,B00001110,B00000010,B00000011
  }
},

Apostrophe_L_bmp[4][8] =
{ { B01111000,B00111000,B00011000,B00001000,B00000000,B00000000,B00000000,B00000000
  },
  { B00000000,
    B00000000,
    B00000000,
    B00000000,
    B00000000,
    B00000000,
    B00000000,
    B00000000
  },
  { B00000000,B00000000,B00000000,B00000000,B00000000,B01000000,B01100000,B01110000
  },
  { B00000000,
    B00000000,
    B00000000,
    B00000000,
    B00000000,
    B00000000,
    B00000000,
    B00000000
  }
},

Apostrophe_R_bmp[4][8] =
{
  { B01110000,B01100000,B01000000,B00000000,B00000000,B00000000,B00000000,B00000000
  },
  { B00000000,B00000000,B00000000,B00000000,B00000000,B00000000,B00000000,B00000000
  },
  { B00000000,B00000000,B00000000,B00000000,B00001000,B00011000,B00111000,B01111000
  },
  { B00000000,B00000000,B00000000,B00000000,B00000000,B00000000,B00000000,B00000000
  }
},
Guillemet_L_bmp[4][8] =
{ { B00000111,
B00000011,
B00000011,
B00000001,
B00000000,
B00000000,
B00000000,
B00000000
  },
  { B11100000,
  B11000000,
  B11000000,
  B10000000,
  B00000000,
  B00000000,
  B00000000,
  B00000000
  },
  { B11100000,
  B01110000,
  B01110000,
  B00111000,
  B00111100,
  B00011100,
  B00001110,
  B00000111
  },
  { B00000111,
  B00001110,
  B00001110,
  B00011100,
  B00111100,
  B00111000,
  B01110000,
  B11100000
  }
},

Guillemet_R_bmp[4][8] =
{ { B00000111,
B00001110,
B00011100,
B00111100,
B00111000,
B01110000,
B01110000,
B11100000
  },
  { B11100000,
  B01110000,
  B00111000,
  B00111100,
  B00011100,
  B00001110,
  B00001110,
  B00000111
  },
  { B00000000,
  B00000000,
  B00000000,
  B00000000,
  B00000001,
  B00000011,
  B00000011,
  B00000111
  },
  { B00000000,
  B00000000,
  B00000000,
  B00000000,
  B10000000,
  B11000000,
  B11000000,
  B11100000
  }
},
Question_bmp[4][8] =
{ { B11000000,
B11000000,
B11000000,
B01100000,
B01100000,
B00111010,
B00011110,
B00001100
  },
  { B00011011,
  B00001011,
  B00000000,
  B00000000,
  B00000000,
  B00000000,
  B00000000,
  B00000000
  },
  { B00011000,
  B00111110,
  B00111111,
  B01100111,
  B01000001,
  B11000000,
  B11000000,
  B11000000
  },
  { B00000000,
  B00000000,
  B00000000,
  B00000000,
  B10000000,
  B11000000,
  B11111011,
  B01111011
  }
},

Z_bmp[4][8] =
{ { B11000000,
B11000000,
B11000000,
B11000000,
B11000000,
B11000000,
B11000000,
B11000000
  },
  { B10000011,
  B11000011,
  B11100011,
  B01110011,
  B00111011,
  B00011111,
  B00001111,
  B00000111
  },
  { B11100000,
  B11110000,
  B11111000,
  B11011100,
  B11001110,
  B11000111,
  B11000011,
  B11000001
  },
  { B00000011,
  B00000011,
  B00000011,
  B00000011,
  B00000011,
  B00000011,
  B00000011,
  B00000011
  }
},


Semicolon_bmp[4][8] =
{ { B00110000,
B00110000,
B00000000,
B00000000,
B00000000,
B00000000,
B00000000,
B00000000
  },
  { B01100100,
  B01100110,
  B00000000,
  B00000000,
  B00000000,
  B00000000,
  B00000000,
  B00000000
  },
  { B00000000,
    B00000000,
    B00000000,
    B00000000,
    B00000000,
    B00000000,
    B00110000,
    B00110000
  },
  { B00000000,
    B00000000,
    B00000000,
    B00000000,
    B00000000,
    B00000000,
    B01111000,
    B01111100
  }
},

Exclamation_bmp[4][8] =
{ { B11111111,
    B11111111,
    B00000000,
    B00000000,
    B00000000,
    B00000000,
    B00000000,
    B00000000
  },
  { B11100110,
    B10000110,
    B00000000,
    B00000000,
    B00000000,
    B00000000,
    B00000000,
    B00000000
  },
  { B00000000,
    B00000000,
    B00000000,
    B00000000,
    B00000000,
    B00000000,
    B11111110,
    B11111111
  },
  { B00000000,
    B00000000,
    B00000000,
    B00000000,
    B00000000,
    B00000000,
    B00000110,
    B11000110
  }
}

;

void clearEyes(Adafruit_8x16matrix *left_matrix, Adafruit_8x16matrix *right_matrix) {
  left_matrix[0].clear();
  left_matrix[1].clear();
  left_matrix[0].writeDisplay();
  left_matrix[1].writeDisplay();
  

  right_matrix[0].clear();
  right_matrix[1].clear();
  right_matrix[0].writeDisplay();
  right_matrix[1].writeDisplay();
}

void drawEye(const uint8_t (*bitmap)[8], Adafruit_8x16matrix *matrix) {
  matrix[0].clear();
  matrix[1].clear();

  matrix[0].drawBitmap(0, 0, bitmap[0], 8, 8, LED_ON);
  matrix[0].drawBitmap(8, 0, bitmap[1], 8, 8, LED_ON);
  matrix[1].drawBitmap(0, 0, bitmap[2], 8, 8, LED_ON);
  matrix[1].drawBitmap(8, 0, bitmap[3], 8, 8, LED_ON);

  matrix[0].writeDisplay();
  matrix[1].writeDisplay();
}


/*=========================================================================
    APPLICATION SETTINGS

    FACTORYRESET_ENABLE       Perform a factory reset when running this sketch
   
                              Enabling this will put your Bluefruit LE module
                              in a 'known good' state and clear any config
                              data set in previous sketches or projects, so
                              running this at least once is a good idea.
   
                              When deploying your project, however, you will
                              want to disable factory reset by setting this
                              value to 0.  If you are making changes to your
                              Bluefruit LE device via AT commands, and those
                              changes aren't persisting across resets, this
                              is the reason why.  Factory reset will erase
                              the non-volatile memory where config data is
                              stored, setting it back to factory default
                              values.
       
                              Some sketches that require you to bond to a
                              central device (HID mouse, keyboard, etc.)
                              won't work at all with this feature enabled
                              since the factory reset will clear all of the
                              bonding data stored on the chip, meaning the
                              central device won't be able to reconnect.
    MINIMUM_FIRMWARE_VERSION  Minimum firmware version to have some new features
    MODE_LED_BEHAVIOUR        LED activity, valid options are
                              "DISABLE" or "MODE" or "BLEUART" or
                              "HWUART"  or "SPI"  or "MANUAL"
    -----------------------------------------------------------------------*/
    #define FACTORYRESET_ENABLE         1
    #define MINIMUM_FIRMWARE_VERSION    "0.6.6"
    #define MODE_LED_BEHAVIOUR          "MODE"
/*=========================================================================*/

// Create the bluefruit object, either software serial...uncomment these lines
/*
SoftwareSerial bluefruitSS = SoftwareSerial(BLUEFRUIT_SWUART_TXD_PIN, BLUEFRUIT_SWUART_RXD_PIN);

Adafruit_BluefruitLE_UART ble(bluefruitSS, BLUEFRUIT_UART_MODE_PIN,
                      BLUEFRUIT_UART_CTS_PIN, BLUEFRUIT_UART_RTS_PIN);
*/

/* ...or hardware serial, which does not need the RTS/CTS pins. Uncomment this line */
// Adafruit_BluefruitLE_UART ble(BLUEFRUIT_HWSERIAL_NAME, BLUEFRUIT_UART_MODE_PIN);

/* ...hardware SPI, using SCK/MOSI/MISO hardware SPI pins and then user selected CS/IRQ/RST */
Adafruit_BluefruitLE_SPI ble(BLUEFRUIT_SPI_CS, BLUEFRUIT_SPI_IRQ, BLUEFRUIT_SPI_RST);

/* ...software SPI, using SCK/MOSI/MISO user-defined SPI pins and then user selected CS/IRQ/RST */
//Adafruit_BluefruitLE_SPI ble(BLUEFRUIT_SPI_SCK, BLUEFRUIT_SPI_MISO,
//                             BLUEFRUIT_SPI_MOSI, BLUEFRUIT_SPI_CS,
//                             BLUEFRUIT_SPI_IRQ, BLUEFRUIT_SPI_RST);


// A small helper
void error(const __FlashStringHelper*err) {
  Serial.println(err);
  while (1);
}

/**************************************************************************/
/*!
    @brief  Sets up the HW an the BLE module (this function is called
            automatically on startup)
*/
/**************************************************************************/
  
void setup(void)
{
  //while (!Serial);  // required for Flora & Micro
  delay(500);

  Serial.begin(115200);
  Serial.println(F("Adafruit Bluefruit Command <-> Data Mode Example"));
  Serial.println(F("------------------------------------------------"));

  int rotation = 3;


  
  left_eye[0].setRotation(rotation);   //We war using the displays vertically.
  left_eye[1].setRotation(rotation);
  left_eye[0].begin(0x70);
  left_eye[1].begin(0x71 );

  right_eye[0].setRotation(rotation);
  right_eye[1].setRotation(rotation);
  right_eye[0].begin(0x74);
  right_eye[1].begin(0x72 );
  clearEyes(left_eye, right_eye);

  /* Initialise the module */
  Serial.print(F("Initialising the Bluefruit LE module: "));

  if ( !ble.begin(VERBOSE_MODE) )
  {
    error(F("Couldn't find Bluefruit, make sure it's in CoMmanD mode & check wiring?"));
  }
  Serial.println( F("OK!") );

  if ( FACTORYRESET_ENABLE )
  {
    /* Perform a factory reset to make sure everything is in a known state */
    Serial.println(F("Performing a factory reset: "));
    if ( ! ble.factoryReset() ){
      error(F("Couldn't factory reset"));
    }
  }

  /* Disable command echo from Bluefruit */
  ble.echo(false);

  Serial.println("Requesting Bluefruit info:");
  /* Print Bluefruit information */
  ble.info();

  Serial.println(F("Please use Adafruit Bluefruit LE app to connect in UART mode"));
  Serial.println(F("Then Enter characters to send to Bluefruit"));
  Serial.println();

  ble.verbose(false);  // debug info is a little annoying after this point!

  /* Wait for connection */
  while (! ble.isConnected()) {
      delay(500);
  }

  Serial.println(F("******************************"));

  // LED Activity command is only supported from 0.6.6
  if ( ble.isVersionAtLeast(MINIMUM_FIRMWARE_VERSION) )
  {
    // Change Mode LED Activity
    Serial.println(F("Change LED activity to " MODE_LED_BEHAVIOUR));
    ble.sendCommandCheckOK("AT+HWModeLED=" MODE_LED_BEHAVIOUR);
  }

  // Set module to DATA mode
  Serial.println( F("Switching to DATA mode!") );
  ble.setMode(BLUEFRUIT_MODE_DATA);

  Serial.println(F("******************************"));
}

/**************************************************************************/
/*!
    @brief  Constantly poll for new command or response data
*/
/**************************************************************************/
void loop(void)
{
  // Check for user input
  char n, inputs[BUFSIZE+1];
  int Emote;
  Emote = 20;

  if (Serial.available())
  {
    n = Serial.readBytes(inputs, BUFSIZE);
    inputs[n] = 0;
    // Send characters to Bluefruit
    Serial.print("Sending: ");
    Serial.println(inputs);

    // Send input data to host via Bluefruit
    ble.print(inputs);
  }

  // Echo received data
  while ( ble.available() )
  {
    Emote = ble.readString().toInt();
    Serial.println(Emote);
    // set the brightness of the LED:
    switch (Emote) {
      case 0:    // your hand is on the sensor
        drawEye(X_bmp, left_eye);
        drawEye(X_bmp, right_eye);
        Serial.println('X');
        break;
      case 1:    // your hand is on the sensor
        drawEye(At_bmp, left_eye);
        drawEye(At_bmp, right_eye);
        Serial.println('@');
        break;

      case 2:    // your hand is on the sensor
        drawEye(O_L_bmp, left_eye);
        drawEye(O_R_bmp, right_eye);
        Serial.println('O');
        break;
      case 3:    // your hand is on the sensor
        drawEye(Hash_bmp, left_eye);
        drawEye(Hash_bmp, right_eye);
        Serial.println('#');
        break;
      case 4:    // your hand is on the sensor
        drawEye(Caret_bmp, left_eye);
        drawEye(Tilde_bmp, right_eye);
        Serial.println('~');
        break;
      case 5:    // your hand is on the sensor
        drawEye(Asterisk_bmp, left_eye);
        drawEye(Asterisk_bmp, right_eye);
        Serial.println('*');
        break;
      case 6:    // your hand is on the sensor
        drawEye(Caret_bmp, left_eye);
        drawEye(Caret_bmp, right_eye);
        Serial.println('^');
        break;
      case 7:    // your hand is on the sensor
        drawEye(Backslash_bmp, left_eye);
        drawEye(Slash_bmp, right_eye);
        Serial.println('/');
        break;
      case 8:    // your hand is on the sensor
        drawEye(Slash_bmp, left_eye);
        drawEye(Backslash_bmp, right_eye);
        Serial.println('/');
        break;
      case 9:    // your hand is on the sensor
        drawEye(Equal_bmp, left_eye);
        drawEye(Equal_bmp, right_eye);
        Serial.println('=');
        break;
      case 10:    // your hand is on the sensor
        drawEye(Nine_bmp, left_eye);
        drawEye(Nine_bmp, right_eye);
        Serial.println('9');
        break;
      case 11:    // your hand is on the sensor
        drawEye(Apostrophe_L_bmp, left_eye);
        drawEye(Apostrophe_R_bmp, right_eye);
        Serial.println(',');
        break;
      case 12:    // your hand is on the sensor
        drawEye(Guillemet_L_bmp, left_eye);
        drawEye(Guillemet_R_bmp, right_eye);
        Serial.println('^');
        break;
      case 13:    // your hand is on the sensor
        drawEye(Question_bmp, left_eye);
        drawEye(Question_bmp, right_eye);
        Serial.println('X');
        break;
      case 14:    // your hand is on the sensor
        drawEye(Z_bmp, left_eye);
        drawEye(Z_bmp, right_eye);
        Serial.println('Z');
        break;
      case 15:    // your hand is on the sensor
        drawEye(Semicolon_bmp, left_eye);
        drawEye(Semicolon_bmp, right_eye);
        Serial.println(';');
        break;
      case 16:    // your hand is on the sensor
        drawEye(Exclamation_bmp, left_eye);
        drawEye(Exclamation_bmp, right_eye);
        break;

      default:
        clearEyes(left_eye, right_eye);
    }
    delay(5);
  }
}
