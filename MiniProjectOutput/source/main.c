
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
//#include <inavr.h>
//#include <avr/eeprom.h>
#include "io.h"
#include "usart.h"
#include "adc.h"

//#define __enable_interrupt()  __bis_SR_register(0x80)
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
void EEPROM_write(unsigned int uiAddress, unsigned char ucData){/* Wait for completion of previous write */
	cli();//diasable interupts
	while(EECR & (1<<EEPE));/* Set up address and Data Registers */	
		EEAR = uiAddress;
		EEDR = ucData;/* Write logical one to EEMPE */
		EECR |= (1<<EEMPE);/* Start eeprom write by setting EEPE*/
		EECR |= (1<<EEPE);//taken from manual
		delay_ms(4);
	sei(); //enable interupts
}
unsigned char EEPROM_read(unsigned int uiAddress){
	cli();
/* Wait for completion of previous write */while(EECR & (1<<EEPE));
/* Set up address register */EEAR = uiAddress;
/* Start eeprom read by writing EERE */EECR |= (1<<EERE);
/* Return data from Data Register *///taken from manual no extra delay needed
	sei(); 
return EEDR;
}
uint8_t  EEMEM NonVolatileCharmode;
uint8_t  EEMEM NonVolatileCharDATA;
char data;
unsigned char mode;
short sdata;
void statemachine(){
	EECR =0;
	static statewrite=0;
	mode=ADC_conversion(0x00)/85+1;//state machine inputs
	USART_Send(mode);
	LCD_ClearScreen();
	LCD_Cursor(1);	
	data=USART_Receive();
	/*unsigned char button1=~PINA&0x2;
	unsigned char button2=~PINA&0x4;
	if(statewrite&&button1){
		eeprom_update_byte(&NonVolatileCharmode, mode);
		eeprom_update_byte(&NonVolatileCharDATA, data);
		//EEPROM_write(0x03FF, mode);
		//EEPROM_write(0x030F, 0x32);
		//EEPROM_write(0x030F, 0x32);
		statewrite=1;
	}
	
	else{
		statewrite=0;
	};
	if(button2){
	//mode=eeprom_read_byte(0x20);
	data=eeprom_read_byte(&NonVolatileCharDATA);
	//writeout(eeprom_read_byte(0x20),eeprom_read_byte(0x2F));
	//writeout(0x02,0x32);
}
	else{
	
}*/
	writeout(mode,data);
	
	
}
void writeout(unsigned char mode,char data){
	if(mode!=1){
		data=-data;
	}
	
	if(data<0){
			LCD_WriteData(0x2D);
			data=-(data);
			
		}
	else{
		LCD_WriteData('+');
	}
	switch (mode) { // Transitions
        case 2:
		sdata=data*2*11/10;//256/128; up to 2.816
		//LCD_Cursor(1);
		LCD_WriteData('0'+sdata/100);
		LCD_WriteData('.');
		LCD_WriteData('0'+(sdata/10)%10);
		LCD_WriteData('0'+sdata%10);
		LCD_WriteData('V');
		LCD_WriteData(0x00);
	break;
	case 1:
		sdata=data*2*11/1;//256/128; up to 28.16
		//LCD_Cursor(1);
		LCD_WriteData('0'+sdata/1000);
		LCD_WriteData('0'+(sdata/100)%10);
		LCD_WriteData('.');
		LCD_WriteData('0'+sdata/10%10);
		LCD_WriteData('V');
		LCD_WriteData(0x00);
	break;
	case 3:
		sdata=data*2*11/200;//256/128; read up to .1408V semi accurate
		//LCD_Cursor(1);
		LCD_WriteData('.');
		LCD_WriteData('0'+sdata/100);
		LCD_WriteData('0'+(sdata/10)%10);
		LCD_WriteData('0'+sdata%10);
		LCD_WriteData('V');
		LCD_WriteData(0x00);
	break;
	default:
		 LCD_DisplayString(1,"shitters");
	break;
	}
}
int main() {
	DDRA=0x00; PORTA=0xFF;
	DDRB=0xFF; PORTB=0x00;
	DDRC=0xFF; PORTC=0x00;
	DDRD=0xFF; PORTD=0x00;
	TimerSet(500); // changing this changes the clock speed .05s
	TimerOn();
	LCD_init();
	initUSART();
	ADC_init_();
	CGRAMLOC();
	mode=2;
	while(1){
		statemachine();
	


		//LCD_DisplayString(1,"Hello World");
		while(!TimerFlag);
		TimerFlag=0;
		//PORTC=0xFF;
		//PORTD=0xFF;
	}
	return 0;
}

