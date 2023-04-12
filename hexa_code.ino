#include <MIDI.h>
struct MySettings : public midi::DefaultSettings {
  static const long BaudRate = 115200;
};

enum NoteStatus : bool {
  NOT_PLAYING = 0,
  PLAYING
};

MIDI_CREATE_CUSTOM_INSTANCE(HardwareSerial, Serial, MIDI, MySettings)
//MIDI_CREATE_DEFAULT_INSTANCE();

#include <Arduino.h>
#include <Adafruit_NeoPixel.h>
#ifdef __AVR__
#include <avr/power.h> // Required for 16 MHz Adafruit Trinket
#endif

// Which pin on the Arduino is connected to the NeoPixels?
// On a Trinket or Gemma we suggest changing this to 1:
#define LED_PIN     6

// How many NeoPixels are attached to the Arduino?
#define LED_COUNT  248

// NeoPixel brightness, 0 (min) to 255 (max)
#define BRIGHTNESS 255 // Set BRIGHTNESS to about 1/5 (max = 255)

// Declare our NeoPixel strip object:
Adafruit_NeoPixel strip(LED_COUNT, LED_PIN, NEO_GRB + NEO_KHZ800);
// Argument 1 = Number of pixels in NeoPixel strip
// Argument 2 = Arduino pin number (most are valid)
// Argument 3 = Pixel type flags, add together as needed:
//   NEO_KHZ800  800 KHz bitstream (most NeoPixel products w/WS2812 LEDs)
//   NEO_KHZ400  400 KHz (classic 'v1' (not v2) FLORA pixels, WS2811 drivers)
//   NEO_GRB     Pixels are wired for GRB bitstream (most NeoPixel products)
//   NEO_RGB     Pixels are wired for RGB bitstream (v1 FLORA pixels, not v2)
//   NEO_RGBW    Pixels are wired for RGBW bitstream (NeoPixel RGBW products)
unsigned long led_tim[31];
unsigned long t;
double t_deg = 200.0;
int led_sta[31];
int notes[31];
int states[31];
int led_sta_last[31];
int led_add[31] = {22, 23, 24, 25, 26, 27, 28, 29, 30, 31, 32, 33, 34, 35, 36, 37, 38, 39, 40, 41, 42, 43, 44, 45, 46, 47, 48, 49, 19, 20, 21};
int R, G, B;
int Br[31];
int Rr[31];
double f[31];
double a[31];
double p[31];
double f2[31];
double a2[31];
double p2[31];
int BR[31];
int ni[31];
int value;
const uint8_t velocity = 127; // How hard the key is struck (~volume)
const uint8_t instrument = 78; // Select ‘harp’ instrument
const uint8_t channel = 1; // MIDI channel 1

void setup() {
    MIDI.begin(MIDI_CHANNEL_OFF);
 // MIDI.sendProgramChange(instrument, channel);
  //MIDI.setHandleNoteOn(MyCCFunction);
  //Serial.begin(115200);
  for (byte  i = 0; i < 31; i++) {
    pinMode(led_add[i], INPUT);
  }
  // These lines are specifically to support the Adafruit Trinket 5V 16 MHz.
  // Any other board, you can remove this part (but no harm leaving it):
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
    notes[i]=40+int(i);
  }

  for (int i = 0; i < 31; i++) {
    f[i] = 0.3 * double(random(1, 5));
    a[i] = 1.0 * double(random(3, 100));
    p[i] = 0.01 * double(random(0, 100));

    f2[i] = 0.3 * double(random(1, 5));
    a2[i] = 1.0 * double(random(3, 50));
    p2[i] = 0.01 * double(random(0, 10));

    BR[i] = random(0, 2);

    ni[i] = 1;
  }


}

void loop() {
 //MIDI.read();
  for (int i = 0; i < 31; i++) {
    value = digitalRead(led_add[i]);
        if (states[i] == NOT_PLAYING) { // note is currently not playing
      if (value ==1) { // if the beam is obstructed
        states[i] = PLAYING;
       MIDI.sendNoteOn(notes[i], velocity, channel);
      }
    } else { // note is currently playing
      if (value ==0) { // if the beam is not obstructed
        states[i] = NOT_PLAYING;
       MIDI.sendNoteOff(notes[i], 0, channel);
      }
    }
    if (value == 1) {
      led_tim[i] = millis();
      led_sta[i] = 1;
    }
    if (led_sta[i] == 1) {
      if ((millis() - led_tim[i]) >= 2.0 * t_deg) {
        led_sta[i] = 0;
      } else {
        for (int j = 0; j < 8; j++) {
          G = 0;//(255.0 - 255.0 * ((millis() - led_tim[i]) / (t_deg)));
          R = (255.0 - 255.0 * ((millis() - led_tim[i]) / (t_deg)));;//0;
          B = 255.0 * ((millis() - led_tim[i]) / (t_deg));
          if (R <= 0) {
            R = 0;
          }
          if (G <= 0) {
            G = 0;
          }
          if (B <=0) {
            B = 0;
          }

          if (R >= 255) {
            R = 255;
          }
          if (G >= 255) {
            G = 255;
          }
          if (B >= 255) {
            B = (254.0 - 255.0 * ((millis() - led_tim[i]) / t_deg));
          }
          strip.setPixelColor(j + i * 8, strip.Color(R, G, B));
              f[i] = 0.3 * double(random(1, 5));
    a[i] = 1.0 * double(random(3, 100));
    p[i] = 0.01 * double(random(0, 100));

        }
      }
    } else {
      Br[i] = int(a[i] * (sin(2.0 * 3.14 * ((millis() / 1000.0) - p[i]) * f[i]) + 1.0) / 2.0);
      Rr[i] = int(a2[i] * (sin(2.0 * 3.14 * ((millis() / 1000.0) - p2[i]) * f2[i]) + 1.0) / 2.0);

      for (int j = 0; j < 8; j++) {
      strip.setPixelColor(j + i * 8, strip.Color(0, 0, Br[i]));
//strip.setPixelColor(j + i * 8, strip.Color(20,20,20));

      }
    }
   
  }


if((millis()-t)>2000){
 
  for (int i = 0; i < 31; i++) {
     if(Br[i]<3){
    //f[i] = 0.3 * double(random(1, 4));
  }
  }
   t = millis();
}
  strip.show();
}

//void MyCCFunction(byte chanel,byte noti, byte spee){
//        for (int j = 0; j < 8; j++) {
////        strip.setPixelColor(j + i * 8, strip.Color(Br[i], Br[i], Br[i]));
//        strip.setPixelColor(j + (int(noti-30)) * 8, strip.Color(0,0,20));
//
//      }
//    //MIDI.sendNoteOn(noti, spee, chanel);
//}
