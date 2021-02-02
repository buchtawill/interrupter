#ifndef __CONSTANTS_H

#include "system.h"


//minimum on-time, in microseconds
#define MIN_ON_TIME 10
#define MAX_ON_TIME 150 //to start with

#define MINIMUM_T2FREQ 62 
#define MINIMUM_T1FREQ 1
#define MAX_FREQUENCY 600


#define ON_TIME_ARRAY_LENGTH 26
static byte on_times[] =  //if the frequency of the note is higher, the on time decreases to stay around a maximum 10% duty cycle. Not implemented yet
{33, 33, 33, 33, 27, 23, 20, 20, 18, 17, 16, 15, 14, 13, 12, 11, 10, 9, 8, 8, 8, 8, 8, 8, 8, 8};

static double noteFreq[60] = {  //The timers are set with int, but I like to keep the precision for the future
  //c,      c#,     d,      d#,     e,      f,      f#,     G,      g#,     a,     a#,     b
  32.7,   34.65,  36.71,  38.89,  41.20,  43.65,  46.25,  49.0,   51.91,  55.0,  58.27,  61.74, //1's
  65.41,  69.3 ,  73.42,  77.78,  82.41,  87.31,  92.5,   98.0,   103.83, 110.0, 116.54, 123.47,//2's
  130.81, 138.59, 146.83, 155.56, 164.81, 174.61, 185.0,  196.0,  207.65, 220.0, 233.08, 246.94,//3's
  261.63, 277.18, 293.66, 311.13, 329.63, 349.23, 369.99, 392.0 , 415.3,  440.0, 466.16, 493.88,//4's
  523.25, 554.37, 587.33, 622.25, 659.26, 698.46, 739.99, 783.99, 830.61, 880.0, 932.33, 987.77 //5's
};


#define BITS_8 256
#define BITS_16 65536
#define TIMER_2_PERIOD_MAX 16000

#define __CONSTANTS_H
#endif
