#include "constants.h"
#include "system.h"

#define TIMER2_MAX 256
#define TIMER1_MAX 65536

int onTime1 = 0, onTime2 = 0;

void setTimer1(int frequency) {
  if (frequency < MINIMUM_T1FREQ)frequency = MINIMUM_T1FREQ;
  else if (frequency > MAX_FREQUENCY)frequency = MAX_FREQUENCY;
  
  cli();
  unsigned long ticks = CPU_CLOCK / (8 * frequency); //prescaler starts at 8. Using 1 is unnecessary 
  TCCR1A = 0;
  TCCR1B = (1 << WGM12); //clear prescalers and turn on CTC mode
  TCNT1 = 0;
  if (ticks <= TIMER1_MAX) setPrescaler1_8();
  else if ((ticks /= 8) <= TIMER1_MAX) setPrescaler1_64();
  else if ((ticks /= 4) <= TIMER1_MAX) setPrescaler1_256(); //set the prescaler accordingly. At low frequencies, 'ticks' is initially way over 2^16
  ticks--;                                                  //minimum frequency is gonna be 1 Hz, which with 256 prescaler is < 2^16
  OCR1A = ticks; //the counter value at which ISR will be triggered. max value is 2^16, 65536
  TIMSK1 |= (1 << OCIE1A); //enable interrupt
  sei();
}


void setTimer2(int frequency) {
  if (frequency < MINIMUM_T2FREQ)frequency = MINIMUM_T2FREQ;
  else if (frequency > MAX_FREQUENCY)frequency = MAX_FREQUENCY;
  
  cli();
  unsigned long ticks = CPU_CLOCK / (frequency);
  TCCR2A = (1 << WGM21); //clear register and turn on CTC mode
  TCCR2B = 0; //clear prescalers
  TCNT2  = 0; //set the tick counter to 0
  if (ticks <= TIMER2_MAX) setPrescaler2_1();
  else if ((ticks /= 8) <= TIMER2_MAX) setPrescaler2_8();
  else if ((ticks /= 4) <= TIMER2_MAX) setPrescaler2_32();
  else if ((ticks /= 2) <= TIMER2_MAX) setPrescaler2_64();
  else if ((ticks /= 2) <= TIMER2_MAX) setPrescaler2_128();
  else if ((ticks /= 2) <= TIMER2_MAX) setPrescaler2_256(); //higher prescalers for lower frequencies
  else if ((ticks /= 4) <= TIMER2_MAX) setPrescaler2_1024();
  //timer 2 cannot go below 61 Hz. 16MHz/(1024*256) = 61
  ticks--;
  OCR2A = ticks; //the counter value at which ISR will be triggered. max value is 2^8, 256
  TIMSK2 |= (1 << OCIE2A); //enable interrupt
  sei();
}

int findOnTime(int freq, int vol){  //maintain duty cycle < 10%. not yet implemented (obviously)
  
}

void setOnTime1(int onTime){
  onTime1 = onTime;
}
void setOnTime2(int onTime){
  onTime2 = onTime;
}

ISR(TIMER1_COMPA_vect) {
  if(onTime1<MIN_ON_TIME) onTime1 = MIN_ON_TIME;
  else if (onTime1>MAX_ON_TIME) onTime1 = MAX_ON_TIME;
  led1On();
  delayMicroseconds(onTime1);
  led1Off();
}
ISR(TIMER2_COMPA_vect) {
  if(onTime2<MIN_ON_TIME) onTime2 = MIN_ON_TIME;
  else if (onTime2>MAX_ON_TIME) onTime2 = MAX_ON_TIME;
  led2On();
  delayMicroseconds(onTime2);
  led2Off();
}
