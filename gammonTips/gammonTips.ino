// A few tips from http://www.gammon.com.au/tips.

void setup() {
  // put your setup code here, to run once:

}

void loop() {
  // put your main code here, to run repeatedly:

}

//  Serial processing tips
//  ----------------------
//  http://www.gammon.com.au/serial
//  
//  Wrong! ...
//  if (Serial.available ()) {
//    char a = Serial.read ();
//    char b = Serial.read ();
//  }


//  Trap #24: Problems with Interrupt Services Routines (ISRs)
//  ----------------------------------------------------------
//    - Do not turn interrupts on or off inside an ISR (as a general rule)
//    - Variables shared between an ISR and main code should be declared volatile
//    - Access to shared variables (volatile variables) in your main code should be protected by noInterrupts() / interrupts() function calls.

//  Alternatives to Serial.print
//  ----------------------------
//    - save values in variables for printing later.
//    - save values in EEPROM until you can read them later.
//    - blink or beep messages in binary or Morse code
//    - use another Arduino via I2C or SPI. http://www.gammon.com.au/forum/?id=11329

//  direct port access to toggle a pin.
//  -----------------------------------
// This only takes 2 clock cycles on a 16 MHz processor (125 nS).
// For the Atmega328 (Uno, Duemilanove, Nano, Mini, Pro Mini, Fio, etc.)
// the appropriate code (for each pin) is:
//
//    // toggle digital pins (need to be output)
//    PIND = bit (0); // toggle D0
//    PIND = bit (1); // toggle D1
//    PIND = bit (2); // toggle D2
//    PIND = bit (3); // toggle D3
//    PIND = bit (4); // toggle D4
//    PIND = bit (5); // toggle D5
//    PIND = bit (6); // toggle D6
//    PIND = bit (7); // toggle D7
//    PINB = bit (0); // toggle D8
//    PINB = bit (1); // toggle D9
//    PINB = bit (2); // toggle D10
//    PINB = bit (3); // toggle D11
//    PINB = bit (4); // toggle D12
//    PINB = bit (5); // toggle D13
//    
//    // toggle analog pins (need to be output)
//    PINC = bit (0); // toggle A0
//    PINC = bit (1); // toggle A1
//    PINC = bit (2); // toggle A2
//    PINC = bit (3); // toggle A3
//    PINC = bit (4); // toggle A4
//    PINC = bit (5); // toggle A5

//  Set WGM01 and COM0A0 in register TCCR0A.
//  ----------------------------------------
//    TCCR0A = bit (WGM01) | bit (COM0A0); // CTC mode, toggle OC0A on match
//  "or" in the bits you want, inside the "bit" macro because they are bits and not simple values.
//  
//  setting one bit:
//    TCCR0A |= bit (COM0A0);  // set toggle OC0A on match
//    bitSet (TCCR0A, COM0A0); // set toggle OC0A on match
//  
//  clearing one bit:
//    TCCR0A &= ~bit (COM0A0);   // clear toggle OC0A on match
//    bitClear (TCCR0A, COM0A0); // clear toggle OC0A on match
//  
//  testing one bit:
//    if (TIFR1 & bit (TOV1))      // has the timer overflowed?
//    if (bitRead (TIFR1, TOV1))   // has the timer overflowed?
