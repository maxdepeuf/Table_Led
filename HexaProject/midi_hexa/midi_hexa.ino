#include <MIDI.h>
struct MySettings : public midi::DefaultSettings {
  static const long BaudRate = 115200;
};

enum NoteStatus : bool {
  NOT_PLAYING = 0,
  PLAYING
};

MIDI_CREATE_CUSTOM_INSTANCE(HardwareSerial, Serial, MIDI, MySettings)

const size_t numberOfNotes = 12;

const uint8_t sensorPins[numberOfNotes] = {A0, A1, A2, A3, A4, A5, A6, A7, A8, A9, A10, A11};
const uint8_t notes[numberOfNotes] = {60, 61, 62, 63, 64, 65, 66, 67, 68 , 69, 70, 71};

const int activateThreshold = 400;
const int deactivateThreshold = 500;

const uint8_t velocity = 127; // How hard the key is struck (~volume)
const uint8_t instrument = 46; // Select ‘harp’ instrument
const uint8_t channel = 1; // MIDI channel 1

NoteStatus states[numberOfNotes] = {}; // keep track of which notes are currently playing (zero-initialized = NON_PLAYING)

void setupcolor=#000000[/color] {
  MIDI.begincolor=#000000[/color];
  MIDI.sendProgramChange(instrument, channel);
}

void loopcolor=#000000[/color] {
  for (uint8_t i = 0; i < numberOfNotes; ++i) {
    int value = analogReadcolor=#000000[/color];
    if (states[i] == NOT_PLAYING) { // note is currently not playing
      if (value < activateThreshold) { // if the beam is obstructed
        states[i] = PLAYING;
        MIDI.sendNoteOn(notes[i], velocity, channel);
      }
    } else { // note is currently playing
      if (value > deactivateThreshold) { // if the beam is not obstructed
        states[i] = NOT_PLAYING;
        MIDI.sendNoteOff(notes[i], velocity, channel);
      }
    }
  }
}
