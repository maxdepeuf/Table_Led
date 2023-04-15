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
String note[31]={"do","re","mi","fa","sol","la","si"," "," "," "," "," "," "," "," "," "," "," "," "," "," "," "," "," "," "," "," "," "," "," "};
int led_sta_last[31];
int led_add[31] = {22, 23, 24, 25, 26, 27, 28, 29, 30, 31, 32, 33, 34, 35, 36, 37, 38, 39, 40, 41, 42, 43, 44, 45, 46, 47, 48, 49, 19, 20, 21};
int R,G,B;
int Br[31];
double f[31];
double a[31];
double p[31];
double f2[31];
double a2[31];
double p2[31];
int BR[31];
int ni[31];


byte commandByte;
byte noteByte;
byte velocityByte;

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

  for (int i = 0; i < 31; i++) {
  f[i]=0.1*double(random(1,4));
  a[i]=1.0*double(random(3,20));
  p[i]=0.01*double(random(0,100));

  f2[i]=0.1*double(random(1,4));
  a2[i]=1.0*double(random(2,10));
  p2[i]=0.1*double(random(0,10));

  BR[i]=random(0,2);

  ni[i]=1;
}
  
  Serial.begin(115200);
  
  cli();//stop interrupts

  //set timer2 interrupt every 128us
  TCCR2A = 0;// set entire TCCR2A register to 0
  TCCR2B = 0;// same for TCCR2B
  TCNT2  = 0;//initialize counter value to 0
  // set compare match register for 7.8khz increments
  OCR2A = 255;// = (16*10^6) / (7812.5*8) - 1 (must be <256)
  // turn on CTC mode
  TCCR2A |= (1 << WGM21);
  // Set CS11 bit for 8 prescaler
  TCCR2B |= (1 << CS11);   
  // enable timer compare interrupt
  TIMSK2 |= (1 << OCIE2A);
  
  sei();//allow interrupts
  
}


ISR(TIMER2_COMPA_vect) {//checks for incoming midi every 128us
  do{
    if (Serial.available()){
      commandByte = Serial.read();//read first byte
      noteByte = Serial.read();//read next byte
      velocityByte = Serial.read();//read final byte
      if (noteByte==60){
strip.setPixelColor(10, strip.Color(0, 50, 0));
  strip.show();
      }
    }
  }
  while (Serial.available() > 2);//when at least three bytes available
}

void loop(){
 
}
