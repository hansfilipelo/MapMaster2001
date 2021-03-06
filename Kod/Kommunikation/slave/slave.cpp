/*
 * slave.cpp
 *
 * Created: 4/7/2014 3:21:49 PM
 *  Author: niker917 davha227
 */ 

#include "slave.h"

void Slave::SPI_Init()
{
	/* Set MISO output, all others input */
	DDRB = (1<<DDB6);
	/* Enable SPI */
	SPCR = (1<<SPE)|(1<<SPIE)|(1<<SPR0);
	SPCR &= ~(1<<MSTR);
	
	//PORTB |= (1<<PORTB4); //Enable pull-up resistor
	
	//dummy l�sning f�r att rensa registret
	SPDR=0x00;
	
	
	EIMSK |= (1<<INT0);
	EICRA |= (1<<ISC01)|(1<<ISC00);
	
	DDRB |= (1<<DDB2);
	
	DDRD &= ~(1<<DDD7);
	
	//sets interupt on pcint16
	PCMSK2 |= (1<<PCINT16);
	PCICR |= (1<<PCIE2);	
	DDRC |= (1<<DDC0);
	
	position = 0;
	return;
}

void Slave::SPI_Send(){
	position=0;
	SPDR = outDataArray[0];
	PORTB |= (1<<PORTB2);	//interrupt to master
	PORTB &= ~(1<<PORTB2);	//clear interrupt to master
	return;
}

/*
ISR(SPI_STC_vect){
	sensormodul.position++;
	SPDR = sensormodul.outDataArray[sensormodul.position];
	sensormodul.inDataArray[sensormodul.position-1] = SPDR;
	
	if ((sensormodul.position == (sensormodul.inDataArray[0]+1))&(sensormodul.inDataArray[0]!= 0)){
		PORTC |= (1<<PORTC0);
	}
}

ISR(PCINT2_vect){
	handleInDataArray();
}

*/