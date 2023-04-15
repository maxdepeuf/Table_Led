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


byte incomingByte;
int note;
int note_old;
byte velocity;
int action=2; //0 =note off ; 1=note on ; 2= nada

void setup() {
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
        strip.setPixelColor(j + (i) * 8, strip.Color(0, 0, 50));
   }
   }
   strip.show();

}

void loop() {
   if (Serial.available() > 0) {
    // read the incoming byte:
    incomingByte = Serial.read();
    

    // wait for as status-byte, channel 1, note on or off
    if (incomingByte== 144){ // note on message starting starting
      action=1;
    }else if (incomingByte== 128){ // note off message starting
      action=0;
       }else if ( (action==0)&&(note==0) ){ // if we received a "note off", we wait for which note (databyte)
      note=incomingByte;
           ni=(note)-40;
      if (ni<0){
        ni=0;
      }
          if (ni>30){
        ni=30;
      }

  
if (velocity==0){
        for (int j = 0; j < 8; j++) {
        strip.setPixelColor(j + (ni) * 8, strip.Color(200, 0, 0));
      }
  
}else{
            for (int j = 0; j < 8; j++) {
        strip.setPixelColor(j + (ni) * 8, strip.Color(0, 0, 50));
      }
 
}
      
    ////  playNote(note, 0);
     //   Serial.print(note);
   // Serial.println(" off");
    //strip.setPixelColor(10, strip.Color(0, 50, 0));

      note=0;
      velocity=0;
      action=2;
    }else if ( (action==1)&&(note==0) ){ // if we received a "note on", we wait for the note (databyte)
      note=incomingByte;
    }else if ( (action==1)&&(note!=0) ){ // ...and then the velocity
      velocity=incomingByte;
  //                Serial.print(note);
//          Serial.println(" on");

 
        //Serial.write(144);
        //Serial.write(note);
         //Serial.write(velocity);
      note=0;
      velocity=0;
      action=0;
    }
  }
   //delay(1);   


   strip.show();
}
