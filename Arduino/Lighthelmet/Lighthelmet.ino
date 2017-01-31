// Released under the GPLv3 license to match the rest of the AdaFruit NeoPixel library

#include <Adafruit_NeoPixel.h>
#ifdef __AVR__
  #include <avr/power.h>
#endif

#define SIGNALCOUNTS 1280

// Which pin on the Arduino is connected to the NeoPixels?
#define PIN 6

// How many NeoPixels are attached to the Arduino?
#define NUMPIXELS 8

// Setup the NeoPixel
Adafruit_NeoPixel pixels = Adafruit_NeoPixel(NUMPIXELS, PIN, NEO_GRB + NEO_KHZ800);


//--------------------------------------
// Setup color channels brightness 
int greenlvl = 10;
int bluelvl = 10;
int redlvl = 10;

// Time dalay between the signal's steps
int dTms = 0; 

// Signal
byte vs[SIGNALCOUNTS]; // signal will be stored here

// Serial comand code
byte cmdcode = 0;

// Counters for the cycles
unsigned int i, j; 


void setup() {
  Serial.begin(115200);
  pixels.begin(); // This initializes the NeoPixel library.
}

/*
  SerialEvent occurs whenever a new data comes in the
 hardware serial RX.  This routine is run between each
 time loop() runs, so using delay inside loop can delay
 response.  Multiple bytes of data may be available.
 */
void serialEvent() {
  
  cmdcode = Serial.read();

  switch(cmdcode) {
    
    case 's':
      Serial.print(F("Upload signal... "));
      Serial.readBytes(vs, SIGNALCOUNTS);
      Serial.println(F("Finished"));      
      break;

    case 'c':
      Serial.print(F("Changing levels..."));
      redlvl = Serial.parseInt();
      Serial.print(F(" R:"));
      Serial.print(redlvl);
      greenlvl = Serial.parseInt();
      Serial.print(F(" G:"));
      Serial.print(greenlvl);
      bluelvl = Serial.parseInt();
      Serial.print(F(" B:"));
      Serial.print(bluelvl);
      Serial.println(F(" Finished")); 
      break;

    case 't':
      Serial.print(F("Changing timestep..."));
      dTms = Serial.parseInt();
      Serial.print(F(" dTms:"));
      Serial.print(dTms);
      Serial.println(F(" Finished"));
      break;

    default:
      Serial.print(F("Echo "));
      Serial.write(cmdcode);
      Serial.write('\n');
      break;    
  }
}

void loop() {

 for(i = 0; i < SIGNALCOUNTS; ++i) {

    for(j = 0; j < NUMPIXELS; ++j) {
        pixels.setPixelColor(j, redlvl > 0 ? vs[i] : 0, greenlvl > 0 ? vs[i] : 0, bluelvl > 0 ? vs[i] : 0);     
    }
    pixels.show();
    if(dTms > 0) {
      delay(dTms);
    }
 }

}

// Uploads signal counts from the PC
void uploadSignal() { 
  Serial.readBytes(vs, SIGNALCOUNTS);
  Serial.print("New data have been recieved #");  
}

