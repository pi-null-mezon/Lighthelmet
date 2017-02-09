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
#define SIGNALCOUNTS 999 // should be divisible by 3
// Define how long controller should wait untill start blinking by the pixels, within this time you should upload signal  
#define WAITSIGNALUPLOADMS 5000

// Setup the NeoPixel
Adafruit_NeoPixel pixels = Adafruit_NeoPixel(NUMPIXELS, PIN, NEO_GRB + NEO_KHZ800);

// Signal
byte vs[SIGNALCOUNTS]; // signal will be stored here
// Time dalay between the signal's steps
int dTms = 1; 
// Counters for the cycles
unsigned int i, j, tmp;
// Store number of color channels
byte channels = 3;
// intensities for the one channel mode
byte r = 32, g = 32, b = 32; 
typedef enum {Red, Green, Blue} ColorEnum;

void setup() {
  // This initializes the NeoPixel library
  pixels.begin(); 
  // Let's switch all LEDs off
  for(j = 0; j < NUMPIXELS; ++j)
    pixels.setPixelColor(j, 0, 0, 0);     
  pixels.show();

  // Load values from the EEPROM
  restoreColor();
  restoreChannels();
  restoreSignal();
  restoreTime();
  
  // Initialize serial port
  Serial.begin(9600);    
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
      Serial.readBytes(vs, SIGNALCOUNTS);
      Serial.println(F("Signal uploaded"));
      // Save to the EEPROM       
      storeSignal();      
      Serial.println(F("Finished!"));       
    break;

    case 'c': {
      Serial.println(F("Changing channels..."));     
      int _c = Serial.parseInt();
      if(_c == 3 || _c == 1) {
        channels = _c;
        storeChannels();
      } else {
        Serial.println(F("Invalid value!"));  
      }
    }
    break;

    case 'r': {
      Serial.println(F("Changing red level..."));     
      int _r = Serial.parseInt();
      if(_r > -1 && _r < 256) {
        r = _r;
        storeColor(ColorEnum::Red);
      } else {
        Serial.println(F("Invalid value!"));  
      }
    }
    break;

    case 'g': {
      Serial.println(F("Changing green level..."));     
      int _g = Serial.parseInt();
      if(_g > -1 && _g < 256) {
        g = _g;
        storeColor(ColorEnum::Green);
      } else {
        Serial.println(F("Invalid value!"));  
      }
    }
    break;

    case 'b': {
      Serial.println(F("Changing blue level..."));     
      int _b = Serial.parseInt();
      if(_b > -1 && _b < 256) {
        b = _b;
        storeColor(ColorEnum::Blue);
      } else {
        Serial.println(F("Invalid value!"));  
      }
    }
    break;

    case '1':
      printSignal();
    break;

    case '2':
      printTime();
    break;

    case '3':
      printChannels();
    break;

    case '4':
      printColor(ColorEnum::Red);
      printColor(ColorEnum::Green);
      printColor(ColorEnum::Blue);
    break;
  }
}

void loop() {
  if(millis() > WAITSIGNALUPLOADMS) { // This is delay for the signal upload after board reboot (on Arduino Uno/Nano it happens when serial communication is established)

    if(channels == 3) {
      
      for(i = 0; i < SIGNALCOUNTS / channels; ++i) {        
        for(j = 0; j < NUMPIXELS; ++j) {
            tmp = channels*i;
            pixels.setPixelColor(j, vs[tmp], vs[tmp+1], vs[tmp+2]);     
        }
        pixels.show();
        delay(dTms);   
      }
      
    } else {

      for(i = 0; i < SIGNALCOUNTS; ++i) {        
        for(j = 0; j < NUMPIXELS; ++j) {
            pixels.setPixelColor(j, r > 0 ? vs[i] : 0, g > 0 ? vs[i] : 0, b > 0 ? vs[i] : 0);     
        }
        pixels.show();
        delay(dTms);   
      }
     
    }
  }
}

void restoreSignal() {
  for(i = 0; i <  SIGNALCOUNTS; ++i) {
    vs[i] = EEPROM.read(i);  
  }  
}

void storeSignal() {
  // Write to EEPROM
  for(i = 0; i <  SIGNALCOUNTS; ++i) {
    EEPROM.update(i, vs[i]);  
  }
}

void printSignal() {
  Serial.println(F("Signal:"));
  for(i = 0; i <  SIGNALCOUNTS / channels; ++i) {   
    if(channels == 3) {
      tmp = channels*i;     
      Serial.print("r[");
      Serial.print(i);
      Serial.print("]: ");
      Serial.println(vs[tmp]);
      Serial.print("g[");
      Serial.print(i);
      Serial.print("]: ");
      Serial.println(vs[tmp+1]); 
      Serial.print("b[");
      Serial.print(i);
      Serial.print("]: ");
      Serial.println(vs[tmp+2]);
      Serial.println("");
    } else {
      Serial.print("[");
      Serial.print(i);
      Serial.print("]: ");
      Serial.println(vs[i]);
    }
  }
}

void printTime() {
   Serial.print(F("dTms: "));
   Serial.println(dTms);
}

void storeTime() {
  EEPROM.put(SIGNALCOUNTS, dTms);
  printTime();
}

void restoreTime() {
  EEPROM.get(SIGNALCOUNTS, dTms);
}

void storeChannels() {
  EEPROM.put(SIGNALCOUNTS + sizeof(dTms), channels);
  printChannels();
}

void restoreChannels() {
  EEPROM.get(SIGNALCOUNTS + sizeof(dTms), channels);  
}

void printChannels() {
  Serial.print(F("Channels: "));
  Serial.println(channels);  
}

void storeColor(const ColorEnum _color) {
  switch(_color) {

    case Red:
      EEPROM.update(SIGNALCOUNTS + sizeof(dTms) + sizeof(channels), r);
    break;

    case Green:
      EEPROM.update(SIGNALCOUNTS + sizeof(dTms) + sizeof(channels) + 1, g);
    break;

    case Blue:
      EEPROM.update(SIGNALCOUNTS + sizeof(dTms) + sizeof(channels) + 2, b);
    break;
  }
  printColor(_color);
}

void restoreColor() {
  r = EEPROM.read(SIGNALCOUNTS + sizeof(dTms) + sizeof(channels));
  g = EEPROM.read(SIGNALCOUNTS + sizeof(dTms) + sizeof(channels) + 1);
  b = EEPROM.read(SIGNALCOUNTS + sizeof(dTms) + sizeof(channels) + 2);
}

void printColor(const ColorEnum _color) {
  switch(_color) {
    
    case Red:
      Serial.print("Red: ");
      Serial.println(r);
    break;

    case Green:
      Serial.print("Green: ");
      Serial.println(g);
    break;

    case Blue:
      Serial.print("Blue: ");
      Serial.println(b);
    break;   
  }  
}





