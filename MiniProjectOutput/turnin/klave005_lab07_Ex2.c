
/*	Author: klave005
 *  Partner(s) Name: quinterra
 *	Lab Section:
 *	Assignment: Lab # 7 Exercise #1
 *	Exercise Description: [optional - include for your own benefit]
 * Create a synchSM to blink three LEDs connected to PB0, PB1, and PB2 in sequence, 1 second each. Implement that synchSM in C using the method defined in class. In addition to demoing your program, you will need to show that your code adheres entirely to the method with no variations. 


 *	I acknowledge all content contained herein, excluding template or example
 *	code, is my own original work.
 */

#include <avr/io.h>
#include <avr/interrupt.h>
#include "io.h"
/*	Author: klave005
 *  Partner(s) Name: quinterra
 *	Lab Section:
 *	Assignment: Lab # 6 Exercise #1
 *	Exercise Description: [optional - include for your own benefit]
 * Create a synchSM to blink three LEDs connected to PB0, PB1, and PB2 in sequence, 1 second each. Implement that synchSM in C using the method defined in class. In addition to demoing your program, you will need to show that your code adheres entirely to the method with no variations. 


 *	I acknowledge all content contained herein, excluding template or example
 *	code, is my own original work.
 */

//#include <avr/io.h>
//#include <avr/interrupt.h>

volatile unsigned char TimerFlag = 0; // TimerISR() sets this to 1. C programmer should clear to 0.

// Internal variables for mapping AVR's ISR to our cleaner TimerISR model.
unsigned long _avr_timer_M = 1; // Start count from here, down to 0. Default 1 ms.
unsigned long _avr_timer_cntcurr = 0; // Current internal count of 1ms ticks

void TimerOn() {
	// AVR timer/counter controller register TCCR1
	TCCR1B = 0x0B;// bit3 = 0: CTC mode (clear timer on compare)
			// bit2bit1bit0=011: pre-scaler /64
			// 00001011: 0x0B
			// SO, 8 MHz clock or 8,000,000 /64 = 125,000 ticks/s
			// Thus, TCNT1 register will count at 125,000 ticks/s

	// AVR output compare register OCR1A.
	OCR1A = 125;	// Timer interrupt will be generated when TCNT1==OCR1A
			// We want a 1 ms tick. 0.001 s * 125,000 ticks/s = 125
			// So when TCNT1 register equals 125,
			// 1 ms has passed. Thus, we compare to 125.
	// AVR timer interrupt mask register
	TIMSK1 = 0x02; // bit1: OCIE1A -- enables compare match interrupt

	//Initialize avr counter
	TCNT1=0;

	_avr_timer_cntcurr = _avr_timer_M;
	// TimerISR will be called every _avr_timer_cntcurr milliseconds

	//Enable global interrupts
SREG |= 0x80; // 0x80: 1000000
}

void TimerOff() {
	TCCR1B = 0x00; // bit3bit1bit0=000: timer off
}

void TimerISR() {
	TimerFlag = 1;
}

// In our approach, the C programmer does not touch this ISR, but rather TimerISR()
ISR(TIMER1_COMPA_vect) {
	// CPU automatically calls when TCNT1 == OCR1 (every 1 ms per TimerOn settings)
	_avr_timer_cntcurr--; // Count down to 0 rather than up to TOP
	if (_avr_timer_cntcurr == 0) { // results in a more efficient compare
		TimerISR(); // Call the ISR that the user uses
		_avr_timer_cntcurr = _avr_timer_M;
	}
}

// Set TimerISR() to tick every M ms
void TimerSet(unsigned long M) {
	_avr_timer_M = M;
	_avr_timer_cntcurr = _avr_timer_M;
}

//--------------------------------------------------------------
unsigned char i='5';

typedef enum states {first, second, third, forth, stopOn, stopOff } states;

unsigned char light = 0x00;

int Tick (int state) {
	int A0=(~PINA)&0x01;
    	switch (state) { // Transitions

        	case first: // start
			if(A0){
				state=stopOn;
				if(i>'0'){
					i--;
				}
			}
			else{
				state = second; //--> moves next: Tc2-s3_execute Ac3 - holding led3 
			}
			break;

		case second:
			if(A0){
				state=stopOn;
					if(i<'9'){i++;}
					if(i=='9'){i='F';}
			}
			else{
				state = third; //--> moves next: Tc3-s1_execute Ac1 - holding led2 
			}

			break;
			
		case third:
			if(A0){
				if(i>'0'){i--;}
				state=stopOn;
			}
			else{
				state = forth; //--> moves next: Tc3-s1_execute Ac1 - holding led2 
			}

			break;
		case forth:
			if(A0){
				state=stopOn;
				if(i<'9'){i++;}
				if(i=='9'){i='F';}
			}
			else{
				state = first; //--> moves next: Tc3-s1_execute Ac1 - holding led2 
			}

			break;
		case stopOn:
			if(A0){
				state=stopOn;
			}
			else{
				state = stopOff; //--> moves next: Tc3-s1_execute Ac1 - holding led2 
			}

			break;
		case stopOff:
			if(A0){
				state=first;
			}
			else{
				state = stopOff; //--> moves next: Tc3-s1_execute Ac1 - holding led2 
			}

			break;
		default:
			state = first;
			break;
	}

	switch (state) { // Actions
		case first:
			light = 0x01;
			break;	

		case second:
			light = 0x02;
			break;	

                case third:
			light = 0x04; // first state went to third state instead of the second. 
			break;
		case forth:
			light = 0x02;
			break;
		case stopOn:
			//light=light;
			break;
		case stopOff:
			//light=light;
			break;
	}

	PORTB = light;
	LCD_ClearScreen();
	LCD_WriteData(i);
	return state;
}

int main() {
	DDRA=0x00; PORTA=0xFF;
	DDRB=0xFF; PORTB=0x00;
	DDRC=0xFF; PORTC=0x00;
	DDRD=0xFF; PORTD=0x00;
	TimerSet(300); // changing this changes the clock speed ~.3
	TimerOn();
	LCD_init();
	LCD_Cursor(1);
	//typedef enum states{init,wait,increment,decrement,set_0}states;//using an action on state change
	states state = first;

	//unsigned char tmpB = 0x00;

	while(1) {
		state = Tick(state);
		// User code (i.e. synchSM calls)
		while (!TimerFlag);	// Wait 1 sec
		TimerFlag = 0;
		//state = Tick(state);
		// Note: For the above a better style would use a synchSM with TickSM()
		// This example just illustrates the use of the ISR and flag
	}
	return 0;
}

