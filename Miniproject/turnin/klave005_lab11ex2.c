
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

//--------Find GCD function --------------------------------------------------
int keypad(int state){
state = GetKeypadKey();
        switch (state) {
            case '\0': PORTB = 0x1F; break; // All 5 LEDs on
            case '1': PORTB = 0x01; break; // hex equivalent
            case '2': PORTB = 0x02; break;
            case '3': PORTB = 0x03; break; // All 5 LEDs on
            case '4': PORTB = 0x04; break; // hex equivalent
            case '5': PORTB = 0x05; break;
            case '6': PORTB = 0x06; break; // All 5 LEDs on
            case '7': PORTB = 0x07; break; // hex equivalent
            case '8': PORTB = 0x08; break;
	case '9': PORTB = 0x09; break;
            // . . . ***** FINISH *****
		case 'A': PORTB = 0x0A; break;
            case 'B': PORTB = 0x0B; break;
            case 'C': PORTB = 0x0C; break;
            case 'D': PORTB = 0x0D; break;
            case '*': PORTB = 0x0E; break;
            case '0': PORTB = 0x00; break;
            case '#': PORTB = 0x0F; break;
            default: PORTB = 0x1B; break;
}
return state;
}

//--------------------------------------------------------------
int paginate(int state){
	switch(state){
		case 0:
			state=1;
			LCD_DisplayString(1,"CS120B is Legend... wait for it");
			break;
		case 1:
			state=0;
			LCD_DisplayString(1,"DARY!");
			break;

}
return state;
}
void main() {
	DDRB = 0xFF; // Set port B to output
	PORTB = 0x00; // Init port B to 0s
	//TimerSet(1); // changing this changes the clock speed
	//TimerOn();
	DDRA=0xF0; PORTA=0xFF;
	DDRC = 0xFF; PORTC = 0x00;
	DDRD = 0xFF; PORTD = 0x00;
	
	
static _task task1, task2/*, task3, task4*/;
_task *tasks[] = { &task1, &task2/*, &task3, &task4*/ };
const unsigned short numTasks = 2; //sizeof(tasks)/sizeof(task*);

// Task 1 (pauseButtonToggleSM)
task1.state = '\0';//Task initial state.
task1.period = 50;//Task Period.
task1.elapsedTime = task1.period;//Task current elapsed time.
task1.TickFct = &keypad;//Function pointer for the tick.
// Task 2 (toggleLED0SM)
task2.state = 0;//Task initial state.
task2.period = 4000;//Task Period.
task2.elapsedTime = task2.period;//Task current elapsed time.
task2.TickFct = &paginate;//Function pointer for the tick.
// Task 3 (toggleLED1SM)
/*task3.state = start;//Task initial state.
task3.period = 1000;//Task Period.
task3.elapsedTime = task3.period; // Task current elasped time.
task3.TickFct = &toggleLED1SMTick; // Function pointer for the tick.
// Task 4 (displaySM)
task4.state = start;//Task initial state.
task4.period = 10;//Task Period.
task4.elapsedTime = task4.period; // Task current elasped time.
task4.TickFct = &displaySMTick; // Function pointer for the tick.
*/
unsigned short i;
unsigned long GCD = tasks[0]->period;
	for ( i = 1; i < numTasks; i++ ) { 
   	 	GCD = findGCD(GCD,tasks[i]->period);
	}
// Set the timer and turn it on
TimerSet(GCD);
TimerOn();
LCD_init();
//LCD_DisplayString(1,"CS120B is Legend... wait for it DARY!");
int state;
 // Scheduler for-loop iterator
while(1) { 
   for ( i = 0; i < numTasks; i++ ) { // Scheduler code
        if ( tasks[i]->elapsedTime == tasks[i]->period ) { // Task is ready to tick
            tasks[i]->state = tasks[i]->TickFct(tasks[i]->state); // Set next state 
            tasks[i]->elapsedTime = 0; // Reset the elapsed time for next tick.
        }
        tasks[i]->elapsedTime += GCD;
    }

	//state=keypad(state);
	//PORTB=0xFF;
   while(!TimerFlag);
        TimerFlag = 0;
}
return 0; // Error: Program should not exit!
}












