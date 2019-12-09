
/*	Author: qwalt002
 *  Partner(s) Name: kyle
 *	Lab Section:
 *	Assignment: Lab # 6 Exercise #1
 *	Exercise Description: [optional - include for your own benefit]
 * Create a synchSM to blink three LEDs connected to PB0, PB1, and PB2 in sequence, 1 second each. Implement that synchSM in C using the method defined in class. In addition to demoing your program, you will need to show that your code adheres entirely to the method with no variations. 


 *	I acknowledge all content contained herein, excluding template or example
 *	code, is my own original work.
 */

#include <avr/io.h>
#include <avr/interrupt.h>
#include "timer.h"
#include "keypad.h"
#include "scheduler.h"
//#include "io.c"
#include "usart.h"
int main() {
	DDRB = 0xFF; // Set port B to output
	PORTB = 0x00; // Init port B to 0s
	//TimerSet(1); // changing this changes the clock speed
	//TimerOn();
	DDRA=0x00; PORTA=0xFF;
	DDRC = 0xFF; PORTC = 0x00;
	DDRD = 0xFF; PORTD = 0x00;
	

// Set the timer and turn it on
//TimerSet(1);
//TimerOn();
initUSART();
while(1){
	//PORTB=0xFF;
 // Scheduler for-loop iterator
	//PORTB=USART_Receive();
	//USART_Send(0xFF);
	USART_Send(~PINA);
	//PORTB=USART_Receive();
	//state=keypad(state);
	//PORTB=0xFF;
  // while(!TimerFlag);
    //    TimerFlag = 0;
}
return 0; // Error: Program should not exit!
}












