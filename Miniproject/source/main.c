
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
#include "io.h"
//#include "io.c"
#include "usart.h"
#include "adc.h"
//#define mux1 (((0x10);
//#define mux10  (0x09);
int main() {
	DDRA=0x00; PORTA=0xFF;
	DDRB=0xFF; PORTB=0x00;
	DDRC=0xFF; PORTC=0x00;
	DDRD=0xFF; PORTD=0x00;
	

// Set the timer and turn it on
//TimerSet(1);
//TimerOn();
LCD_init;
/*LCD_WriteCommand(0x38);
LCD_WriteCommand(0x0F);
LCD_WriteCommand(0x06);
LCD_WriteCommand(0x01);*/
delay_ms(100);
initUSART();
ADC_init_();

//ACD_MUXx1();
unsigned char data;
char mode;

while(1){
	
	
	/*if(data==64){
		LCD_DisplayString(1,"hello world"); 
	}*/
//*/
	mode =USART_Receive();//statemachine input
	PORTB=mode;
	if(mode==1){
		data= ADC_conversion(0x10);	
	}
	else if (mode==2){
		data= ADC_conversion(0x09);
	}
	else if (mode==3){
		data= ADC_conversion(0x0B);
		
	}
	//1x mult
	//PORTB=data;
	USART_Send(data);
	//USART_Send('0');
	//PORTB=USART_Receive();
	//USART_Send(0x34);
	//LCD_DisplayString(1,"hello world"); 
	
}
return 0; // Error: Program should not exit!
}












