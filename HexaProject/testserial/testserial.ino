#include <Arduino.h>
#include <Adafruit_NeoPixel.h>
#ifdef __AVR__
#include <avr/power.h> // Required for 16 MHz Adafruit Trinket
#endif

#define LED_PIN     6
#define LED_COUNT  248
#define BRIGHTNESS 255 
Adafruit_NeoPixel strip(LED_COUNT, LED_PIN, NEO_GRB + NEO_KHZ800);

int incomingByte = 0; // for incoming serial data
int led_add[31] = {22, 23, 24, 25, 26, 27, 28, 29, 30, 31, 32, 33, 34, 35, 36, 37, 38, 39, 40, 41, 42, 43, 44, 45, 46, 47, 48, 49, 19, 20, 21};

void setup() {
  Serial.begin(115200); // opens serial port, sets data rate to 9600 bps
     Serial2.begin(115200); // opens serial port, sets data rate to 9600 bps

   for (byte  i = 0; i < 31; i++) {
    pinMode(led_add[i], INPUT);
  }
#if defined(__AVR_ATtiny85__) && (F_CPU == 16000000)
  clock_prescale_set(clock_div_1);
#endif
  // END of Trinket-specific code.

  strip.begin();           // INITIALIZE NeoPixel strip object (REQUIRED)
  strip.show();            // Turn OFF all pixels ASAP
  strip.setBrightness(BRIGHTNESS);


}

void loop() {
//  // send data only when you receive data:
 if (Serial.available() > 0) {
//    // read the incoming byte:
 incomingByte = Serial.read();
  // if (Serial.available() > 0) {
  //   // read the incoming byte:
  //   incomingByte = Serial.read();
    
  //   num = map(incomingByte, 50, 105, 0, 65);
  
  //   leds[num] = CRGB ( random(0,255), random(0,255), random(0,255));
  //   //random function can read random variable random(min,max);
  //   FastLED.show();  // this row should be write when you turn on led
  // }
//if(incomingByte==60){;
//strip.setPixelColor(10, strip.Color(0, 50, 0));
//}else{
//strip.setPixelColor(incomingByte, strip.Color(0, 0, 60));
//}
////  digitalWrite(13,1);
////}
  // Serial.write(incomingByte);
 Serial2.write(incomingByte);
//strip.show();
//delay(10);

 }
}
