/*
   Will Buchta
   Last modified 1/16/2021
   Adapted from oneTesla's SD card interrupter
   onetesla.com
   http://aaron.headwai.com/ra/MIDI/MIDI%20Message%20Table%201.pdf
*/

//a midi message has three bytes
//first byte nybble one is command; on is 9, 8 is off. all other commands are filtered out by this program
//first byte nybble two is what channel the note is on; 0-15
//second byte is the note byte, 12-72 or something like that. 60 is C4
//third byte is note velocity, how hard the piano key was pressed. 0-127
//example:
// 10010000 00111100 01100100
//   9  0      60       100
// turn on note 60 on channel 0 with a velocity of 100

#include "constants.h"
#include "system.h"
#include "timers.h"

#include <SoftwareSerial.h>
SoftwareSerial MIDI(6, 5); //rx,tx. We only care about rx. A serial port is needed in software as I used the hardware serial pins to upload code
//may configure programmer in future
int command = 0, channelByte = 0, noteByte = 0, velocityByte = 0;
int frequency = 0;
int noteOn = 0, prevNote = 0, ch2noteOn = 0;  //ints used instead of bytes to prevent overflow errors
boolean ignoreData = false, debug = false; //for filtering data

void setup() {
  if (debug)
    Serial.begin(2000000);
  MIDI.begin(31250);  //standard midi baud rate is 31250
  DDRD |= (1 << 2) | (1 << 3); //PD4 and 5, pins 6 and 11 of ATMEGA
  //setTimer1(20);
  //setTimer2(270);
}
void loop() {
  int volume = findVol(); //takes ~100us
  setOnTime1(volume);
  setOnTime2(volume);
  if (MIDI.available()) { //if there is data in the serial buffer
    byte b = MIDI.read();
    command = (b & 0xf0) >> 4;  //first nybble
    channelByte = b & 0x0f;     //second nybble
    if (command == 9 || command == 8) { // on or off
      delayMicroseconds(200); //the next two bytes take time to enter buffer. precision of 4 us. Fixed value thru trial and error
      noteByte = MIDI.read();
      velocityByte = MIDI.read();
      if (velocityByte > 127 || velocityByte < 0) { //several other midi commands can get sent and picked up in velocityByte.
        //MIDIClear();                              //ignore all of these other commands
        ignoreData = true;
      }
      frequency = (int)round(noteFreq[noteByte - 24]);  //find the frequency of the note

      if (debug) {
        String spc = " ";
        //String str = command + spc + channelByte + spc + noteByte + spc + velocityByte;
        String str = command + spc + channelByte + spc + velocityByte;
        if (ignoreData) {
          Serial.print(command + spc + velocityByte + spc); //debug
          Serial.println(velocityByte, BIN);
        }
      }
      if (channelByte == 0) {           //channel in question
        if (command == 9 && !ignoreData) {
          noteOn = frequency;
          setTimer1(frequency); //set timer 1 to the frequency of the note
        }
        if (command == 8) {
          //if (frequency == noteOn)
          disableTimer1();  //if the command is 'off', turn off
        }
      }
      else if (channelByte == 1) {
        if (command == 9 && !ignoreData) {
          ch2noteOn = frequency;
          setTimer2(frequency);
        }
        if (command == 8) {
          //if (frequency == ch2noteOn)
          disableTimer2();
        }
      }
      if (ignoreData) {
        //disableTimer1();  //debug
        //disableTimer2();
        ignoreData = false;
      }
    }
    else {  //clear buffer if channel nybble is not 0 or 1
      MIDIClear();
      disableTimer1();
      disableTimer2();
    }
  }
}

int findVol() {
  int val = analogRead(A1); // pin 24
  return map(val, 0, 1024, 10, 150);
}
void MIDIClear() {
  while (MIDI.available() > 0)
    char j = MIDI.read();  //clear the next three bytes
}
