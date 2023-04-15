#include <Arduino.h>
#include <Adafruit_NeoPixel.h>
#ifdef __AVR__
#include <avr/power.h> // Required for 16 MHz Adafruit Trinket
#endif

#define LED_PIN     6
#define LED_COUNT  248
#define BRIGHTNESS 255 
Adafruit_NeoPixel strip(LED_COUNT, LED_PIN, NEO_GRB + NEO_KHZ800);

unsigned long led_tim[31];
unsigned long t;
double t_deg=1000.0;
int led_sta[31];
//String note[31]={"do","re","mi","fa","sol","la","si"," "," "," "," "," "," "," "," "," "," "," "," "," "," "," "," "," "," "," "," "," "," "," "};
int led_sta_last[31];
int led_add[31] = {22, 23, 24, 25, 26, 27, 28, 29, 30, 31, 32, 33, 34, 35, 36, 37, 38, 39, 40, 41, 42, 43, 44, 45, 46, 47, 48, 49, 19, 20, 21};
int R,G,B;
int Br[31];
int ni;
double f[31];
double a[31];
double p[31];
double f2[31];
double a2[31];
double p2[31];
int BR[31];

byte statusByte;
byte ControllerNumber;
byte ControllerValue;
//cc midi message channel 1  1011 0000
byte ccmidi = 176;
int note;

void setup(){
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
  for (byte i = 0; i < 31; i++) {
    led_tim[i] = millis();
    led_sta[i] = 0;
  }
  // put your setup code here, to run once:
  Serial.begin(115200); // opens serial port, sets data rate to 9600 bps
   for (int i = 0; i < 31; i++) {
   for (int j = 0; j < 8; j++) {
        strip.setPixelColor(j + (i) * 8, strip.Color(0, 0, 0));
   }
   }
   strip.show();
}
void loop(){
  //waite for a midi message
  while(Serial.available() <3);
  //run the function that will control the leds
  readmidi();
 for (byte i = 0; i < 31; i++) {
if((millis()-led_tim[i])>1000){
              for (int j = 0; j < 8; j++) {
        //strip.setPixelColor(j + (ni) * 8, strip.Color(0, 0, 0));
      }
}
 }
  
   strip.show(); 
  }
void readmidi(){
  
  do{
    //sheck if there is any message
    if (Serial.available()){
      //store the data into the variables 
      statusByte = Serial.read();
      ControllerNumber = Serial.read();
      ControllerValue = Serial.read(); 
      //check if the message is a cc midi message
      if (statusByte==144){
    note=ControllerNumber;
           ni=(note)-40;
      if (ni<0){
        ni=0;
      }
          if (ni>30){
        ni=30;
      }
        //controll the leds acording to the data bytes

      led_tim[ni]=millis();

              for (int j = 0; j < 8; j++) {
        strip.setPixelColor(j + (ni) * 8, strip.Color(50, 0, 0));
      }


      
      }

            if (ControllerValue==0||statusByte==128){

        //controll the leds acording to the data bytes
           for (int j = 0; j < 8; j++) {
        strip.setPixelColor(j + (ni) * 8, strip.Color(0, 0, 0));
      }
      
      }
    }
  }
  //repeat until there is no message
  while (Serial.available() > 2);
}
