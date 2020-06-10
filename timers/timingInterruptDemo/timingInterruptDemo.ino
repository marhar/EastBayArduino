// ref: https://sites.google.com/site/qeewiki/books/avr-guide/timers-on-the-atmega328
//      https://medium.com/@reefwing/programming-the-atmega328p-registers-and-interrupts-dfc25ca22c18
//
// Setting clock scaling
//  TCCR0B |=  0;                             // TCC disabled
//  TCCR0B |=                   (1<<CS00);    // clock/1 (no prescaling
//  TCCR0B |=          (1<<CS01);             // clock/8
//  TCCR0B |=          (1<<CS01)|(1<<CS00);   // clock/64
//  TCCR0B |= (1<<CS02);                      // clock/256
//  TCCR0B |= (1<<CS02)|         (1<<CS00);   // clock/1024
//  TCCR0B |= (1<<CS02)|(1<<CS01);            // external clock, falling edge
//  TCCR0B |= (1<<CS02)|(1<<CS01)|(1<<CS00);  // external clock, rising edge
//
// Finding match:
//  int clockspeed = 16000000;
//  int hz = 2000;
//  int prescaler = 64;
//  int matchRegister = clockspeed/(hz * prescaler) - 1;

#include <ArrbotMonitor.h>

void setup(){
  Serial.begin(115200);
  cli();
  
  // timer0: 2 kHz
  // 1024,6,2230 256,30,2015 64,124,1998 8,499,---
  TIMSK0 |= (1 << OCIE0A);           // enable CTC
  TCCR0A = TCCR0B = TCNT0 = 0;       // start with clean registers
  TCCR0A |= (1 << WGM01);            // turn on CTC mode
  TCCR0B |= (1 << CS01)|(1 << CS00); // prescale: 64
  OCR0A = 124;                       // 16000000/(2000*256) - 1
  
  // timer1: 1 Hz
  TCCR1A = TCCR1B = TCNT1  = 0;         // clean registers
  TCCR1B |= (1 << CS12) | (1 << CS10);  // prescaler: 1024
  OCR1A = 15624;                        // 16000000/(1*1024) - 1
  TCCR1B |= (1 << WGM12);               // CTC mode
  TIMSK1 |= (1 << OCIE1A);              // enable CTC

  // timer2: 8 KHz
  TCCR2A = TCCR2B = TCNT2  = 0;        // start off clean
  TCCR2B |= (1 << CS21);               // prescale: 8
  OCR2A = 249;                         // 16000000/(8000*8) - 1
  TCCR2A |= (1 << WGM21);              // CTC mode
  TIMSK2 |= (1 << OCIE2A);             // enable CTC

  sei();
}

volatile uint32_t x0;
volatile uint32_t x1;
volatile uint32_t x2;
volatile uint32_t print;
ISR(TIMER0_COMPA_vect){ x0++; }
ISR(TIMER1_COMPA_vect){ x1++; print++;}  
ISR(TIMER2_COMPA_vect){ x2++; }

void loop(){
  if (print >= 10) {
    MONITOR(x0); MONITOR(x1); MONITOR(x2); MONITOR_ENDL();
    x0 = x1 = x2 = print = 0;
  }
}
