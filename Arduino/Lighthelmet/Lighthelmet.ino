// Released under the GPLv3 license to match the rest of the AdaFruit NeoPixel library
#include <EEPROM.h>
#include <Adafruit_NeoPixel.h>
#ifdef __AVR__
  #include <avr/power.h>
#endif

//---------------------------------------------------------------------------
// Which pin on the Arduino is connected to the NeoPixels?
#define PIN 6
// How many NeoPixels are attached to the Arduino?
#define NUMPIXELS 2
//---------------------------------------------------------------------------

// Limited by the Atmega328P EEPROM size which is 1024 bytes, remember about 3 color channels
#define SIGNALCOUNTSPERCHANNEL 333
// Define how long controller should wait untill start blink by the pixels, within this time you should upload signal  
#define WAITSIGNALUPLOADMS 1000

// Setup the NeoPixel
Adafruit_NeoPixel pixels = Adafruit_NeoPixel(NUMPIXELS, PIN, NEO_GRB + NEO_KHZ800);

// Signal
byte vred[SIGNALCOUNTSPERCHANNEL]; // signal will be stored here
byte vgreen[SIGNALCOUNTSPERCHANNEL]; // signal will be stored here
byte vblue[SIGNALCOUNTSPERCHANNEL]; // signal will be stored here
// Time dalay between the signal's steps
int dTms = 1; 
// Counters for the cycles
unsigned int i, j; 


void setup() {
  restoreSignal();
  restoreTime();
  Serial.begin(9600);
  pixels.begin(); // This initializes the NeoPixel library
  for(j = 0; j < NUMPIXELS; ++j) {
    pixels.setPixelColor(j, 0, 0, 0);     
  }
  pixels.show();
}

/*
  SerialEvent occurs whenever a new data comes in the
 hardware serial RX.  This routine is run between each
 time loop() runs, so using delay inside loop can delay
 response.  Multiple bytes of data may be available.
 */
void serialEvent() {  

  int code = Serial.read();

  switch(code) {
    
    case 't':
      Serial.println(F("Changing timestep..."));
      dTms = Serial.parseInt();
      storeTime();
    break;
     
    case 's': 
      Serial.println(F("Uploading signal... "));      
      Serial.readBytes(vred, SIGNALCOUNTSPERCHANNEL);
      Serial.println(F("Red ready"));       
      Serial.readBytes(vgreen, SIGNALCOUNTSPERCHANNEL);
      Serial.println(F("Green ready"));            
      Serial.readBytes(vblue, SIGNALCOUNTSPERCHANNEL);
      Serial.println(F("Blue ready"));  
      storeSignal();      
      Serial.println(F("Finished!"));       
    break;

    case '1':
      printSignal();
    break;

    case '2':
      printTime();
    break;
  }
}

void loop() {
  if(millis() > WAITSIGNALUPLOADMS) {
    for(i = 0; i < SIGNALCOUNTSPERCHANNEL; ++i) {
      for(j = 0; j < NUMPIXELS; ++j) {
          pixels.setPixelColor(j, vred[i], vgreen[i], vblue[i]);     
      }
      pixels.show();
      delay(dTms);   
    }
  }
}

void restoreSignal() {
  // Read red part
  for(i = 0; i <  SIGNALCOUNTSPERCHANNEL; ++i) {
    vred[i] = EEPROM.read(i);  
  }
  // Read green part
  for(i = 0; i <  SIGNALCOUNTSPERCHANNEL; ++i) {
    vgreen[i] = EEPROM.read(SIGNALCOUNTSPERCHANNEL + i);  
  }
  // Read blue part
  for(i = 0; i <  SIGNALCOUNTSPERCHANNEL; ++i) {
    vblue[i] = EEPROM.read(2*SIGNALCOUNTSPERCHANNEL + i); 
  }   
}

void storeSignal() {
  // Write to EEPROM
  for(i = 0; i <  SIGNALCOUNTSPERCHANNEL; ++i) {
    EEPROM.update(i, vred[i]);  
  }
  // Write to EEPROM
  for(i = 0; i <  SIGNALCOUNTSPERCHANNEL; ++i) {
    EEPROM.update(SIGNALCOUNTSPERCHANNEL + i, vgreen[i]);  
  }
  // Write to EEPROM
  for(i = 0; i <  SIGNALCOUNTSPERCHANNEL; ++i) {
    EEPROM.update(2*SIGNALCOUNTSPERCHANNEL + i, vblue[i]);  
  } 
}

void printSignal() {
  Serial.println(F("Red part:"));
  for(i = 0; i <  SIGNALCOUNTSPERCHANNEL; ++i) {
    Serial.print("r[");
    Serial.print(i);
    Serial.print("]: ");
    Serial.println(vred[i]);  
  }
  Serial.println(F("Green part:"));
  for(i = 0; i <  SIGNALCOUNTSPERCHANNEL; ++i) {
    Serial.print("g[");
    Serial.print(i);
    Serial.print("]: ");
    Serial.println(vgreen[i]);  
  }
  Serial.println(F("Blue part:"));
  for(i = 0; i <  SIGNALCOUNTSPERCHANNEL; ++i) {
    Serial.print("b[");
    Serial.print(i);
    Serial.print("]: ");
    Serial.println(vblue[i]);  
  }   
}

void printTime() {
   Serial.print(F("dTms: "));
   Serial.println(dTms);
}

void storeTime() {
  EEPROM.put(3*SIGNALCOUNTSPERCHANNEL, dTms);
  printTime();
}

void restoreTime() {
  EEPROM.get(3*SIGNALCOUNTSPERCHANNEL, dTms);
}


