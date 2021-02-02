#ifndef __SYSTEM_H

#include <Arduino.h>
//#include <avr/io.h>
//#include <avr/interrupt.h>
#include <avr/delay.h>

#define TIMER1_MAX BITS_16  //timer1 counter overflows at 2^16
#define TIMER2_MAX BITS_8   //timer2                      2^8
#define CPU_CLOCK 16000000

#define led1On() PORTD |= (1 << 2)
#define led1Off() PORTD &= ~(1 << 2)
#define led2On() PORTD |= (1 << 3)
#define led2Off() PORTD &= ~(1 << 3)

#define setPrescaler1_8() TCCR1B = (1 << CS11) | (1 << WGM12)               //set prescalers for timer interrupts
#define setPrescaler1_64() TCCR1B = (1 << CS11) | (1 << CS10) | (1 << WGM12)
#define setPrescaler1_256() TCCR1B = (1 << CS12) | (1 << WGM12)
#define setPrescaler2_1() TCCR2B = (1 << CS20)
#define setPrescaler2_8() TCCR2B = (1 << CS21)
#define setPrescaler2_32() TCCR2B = (1 << CS21) | (1 << CS20)
#define setPrescaler2_64() TCCR2B = (1 << CS22)
#define setPrescaler2_128() TCCR2B = (1 << CS22) | (1 << CS20)
#define setPrescaler2_256() TCCR2B = (1 << CS22) | (1 << CS21)
#define setPrescaler2_1024() TCCR2B = (1 << CS22) | (1 << CS21) | (1 << CS20)

#define disableTimer1() TIMSK1 &= ~(1 << OCIE1A)
#define disableTimer2() TIMSK2 &= ~(1 << OCIE2A)


#define __SYSTEM_H
#endif
