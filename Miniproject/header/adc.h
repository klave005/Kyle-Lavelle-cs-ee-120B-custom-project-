#ifndef ADC_H
#define ADC_H

void ADC_init_(){
   ADCSRA |= (1 << ADEN)|(1 << ADSC)|0x6;;//(1 << ADATE) |  | )no prescalar|0x6;//with /32 prescalar
}
void ACD_MUXx1(){
ADMUX=0x3<<6|0x10;//ADC0=pos,ADC1=neg,Vref =2.56V with capacitor at points.
}
/*void ACD_MUX1.1V(){
ADMUX=0x3<<6|0x1E;//ADC0=pos,ADC1=neg,Vref =2.56V with capacitor at points.
}*/
void ACD_MUXx10(){
ADMUX=0x3<<6|0x09;//ADC0=pos,ADC1=neg, Vref =2.56V with capacitor at points.x10 magnification
}
void ACD_MUXx200(){
ADMUX=0x3<<6|0x0B;//ADC0=pos,ADC1=neg Vref =2.56V with capacitor at points. x200 Maginfication
}
char ADC_conversion(char admux){//expected only 8 bits of
	ADMUX=(1<<ADLAR)|0x3<<6|admux;
	ADCSRA|=1<<ADSC;
	while(ADCSRA&(1<<ADSC));
	char data=ADCH;
		//data=~((~data)&0x3F+1)+1;//converts 10 bit 2's comp to 16 bit 2 comp;
	return data;
}
#endif 
