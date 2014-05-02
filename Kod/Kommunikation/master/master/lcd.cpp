/*
 * lcd.cpp
 *
 * Created: 4/14/2014 11:13:42 AM
 *  Author: davha227
 *
 *  Virat kablage
 *  Rs -> PORTA5
 *  R/W -> PORTA6
 *  E -> PORTA7
 *  DB0 -> PORTC0
 *  DB1 -> PORTC1
 *  DB2 -> PORTD4
 *  DB3 -> PORTD5
 *  DB4 -> PORTD6
 *  DB5 -> PORTD7
 *  DB6 -> PORTC6
 *  DB7 -> PORTC7
 */

#include "lcd.h"

Lcd::Lcd(){
	init();
	command(0x0C); //turns off cursor
	drawSensorNames();
}

void Lcd::SetData(unsigned char var){
	shift = var & 0x01;
	if(shift == 0x01){
		PORTC |= (1<<PORTC0);
	}
	else{
		PORTC &= ~(1<<PORTC0);
	}
	shift = var >>1;
	shift = shift & 0x01;
	if(shift == 0x01){
		PORTC |= (1<<PORTC1);
	}
	else{
		PORTC &= ~(1<<PORTC1);
	}
	
	shift = var >>2;
	shift = shift & 0x01;
	
	if(shift == 0x01){
		PORTD |= (1<<PORTD4);
	}
	else{
		PORTD &= ~(1<<PORTD4);
	}
	
	shift = var >>3;
	shift = shift & 0x01;
	
	if(shift == 0x01){
		PORTD |= (1<<PORTD5);
	}
	else{
		PORTD &= ~(1<<PORTD5);
	}
    
	shift = var >>4;
	shift = shift & 0x01;
	if(shift == 0x01){
		PORTD |= (1<<PORTD6);
	}
	else{
		PORTD &= ~(1<<PORTD6);
	}
	
	shift = var >>5;
	shift = shift & 0x01;
	if(shift == 0x01){
		PORTD |= (1<<PORTD7);
	}
	else{
		PORTD &= ~(1<<PORTD7);
	}
	
	shift = var >>6;
	shift = shift & 0x01;
	
	if(shift == 0x01){
		PORTC |= (1<<PORTC6);
	}
	else{
		PORTC &= ~(1<<PORTC6);
	}
    
	shift = var >>7;
	shift = shift & 0x01;
	if(shift == 0x01){
		PORTC |= (1<<PORTC7);
	}
	else{
		PORTC &= ~(1<<PORTC7);
	}
}

void Lcd::init(){
    DDRC |= 0xC3; //portC pin 0,1,6,7 till utg�ngar
	DDRD |= 0xF0;	//portD pin 4-7 som utg�ngar
	DDRA |= 0xE0; //port A pin 5-7 s�tt till utg�ng
	PORTA |= (1<<PORTA7); // enable till 1
    
	SetData(0x38);
    //Function set: 2 Line, 8-bit, 5x8 dots
	PORTA &= ~(1<<PORTA5)|(1<<PORTA6);
	PORTA |= (1<<PORTA7); //enable
	PORTA &= ~(1<<PORTA7); //disable
	_delay_ms(100);
	
	SetData(0x0F);
	//Display on, Curson blinking command
	PORTA &= ~(1<<PORTA5)|(1<<PORTA6);
	PORTA |= (1<<PORTA7); //enable
	PORTA &= ~(1<<PORTA7); //disable
	_delay_ms(100);
	
	SetData(0x01);
	//Clear LCD
	PORTA &= ~(1<<PORTA5)|(1<<PORTA6);
	PORTA |= (1<<PORTA7); //enable
	PORTA &= ~(1<<PORTA7); //disable
	_delay_ms(100);
	
	SetData(0x06);
	//Entry mode, auto increment with no shift
	PORTA &= ~(1<<PORTA5)|(1<<PORTA6);
	PORTA |= (1<<PORTA7); //enable
	_delay_ms(100);
}
// f�r att uf�ra olika kommandon
void Lcd::command(unsigned char var){
	/*
 * lcd.cpp
 *
 * Created: 4/14/2014 11:13:42 AM
 *  Author: davha227
 *
 *  Virat kablage
 *  Rs -> PORTA5
 *  R/W -> PORTA6
 *  E -> PORTA7
 *  DB0 -> PORTC0
 *  DB1 -> PORTC1
 *  DB2 -> PORTD4
 *  DB3 -> PORTD5
 *  DB4 -> PORTD6
 *  DB5 -> PORTD7
 *  DB6 -> PORTC6
 *  DB7 -> PORTC7
 */
	
	DDRC |=(1<<PORTC0)|(1<<PORTC1)|(1<<PORTC6)|(1<<PORTC7);
	DDRD |=(1<<PORTD4)|(1<<PORTD5)|(1<<PORTD6)|(1<<PORTD7);
	SetData(var);
	
	PORTA &= ~(1<<PORTA6);
	PORTA &= ~(1<<PORTA5);
	_delay_us(1);
	PORTA |=(1<<PORTA7);
	_delay_us(1);
	PORTA &= ~(1<<PORTA7);
	_delay_us(1);
	PORTA |= (1<<PORTA7);
}

void Lcd::senddata(unsigned char var){
	SetData(var); //Function set: 2 Line, 8-bit, 5x7 dots
	
	PORTA &= ~(1<<PORTA6);
	PORTA |= (1<<PORTA5); //Rs till 1
	PORTA |= (1<<PORTA7); //enable
	PORTA &= ~(1<<PORTA7); //disable
}

void Lcd::draw(unsigned char location, unsigned char sign){
	
        if(ready() && moveToggle){
            command(location);
            moveToggle = 0;
			return;
		}
		if(ready() && (!moveToggle)){
            asm("");
			senddata(sign);
            moveToggle = 1;
			drawSucceded = true;
		}
}

void Lcd::firstDraw(unsigned char location, unsigned char sign){
    _delay_us(40);
    command(location);
    _delay_us(40);
    senddata(sign);
}

void Lcd::drawSensorNames(){
	
    //rad 1
	//prints S1
	firstDraw(0x87,0x53);
	firstDraw(0x88,0x31);
	//prints L1
	firstDraw(0x80,0x4c);
	firstDraw(0x81,0x31);
	
	//rad 2
	//prints S2
	firstDraw(0xc7,0x53);
	firstDraw(0xc8,0x32);
	//prints L2
	firstDraw(0xc0,0x4c);
	firstDraw(0xc1,0x32);
	
	//rad 3
	//prints S3
	firstDraw(0x97,0x53);
	firstDraw(0x98,0x33);
	//prints M1
	firstDraw(0x90,0x4d);
	firstDraw(0x91,0x31);
    
	//rad 4
	//prints S4
	firstDraw(0xd7,0x53);
	firstDraw(0xd8,0x34);
}

unsigned int Lcd::getNewPosition(int col, int row){
	unsigned int position;
	
	switch(row){
		case 0 :  position = 0x80;
			break;
		case 1 :  position = 0xc0;
			break;
		case 2 :  position = 0x90;
			break;
		case 3 :  position = 0xd0;
			break;
	} 
	if (col==3){
		position = position + 3;
	}
	else{
		position = position + 10;
	}
	return position;
}

void Lcd::update(){
	char nextChar;
	if (drawSucceded){
		drawSucceded=false;
		if (internalCounter == 0){
			row = getRow(sensorCounter);
			col = getCol(sensorCounter);
			position = getNewPosition(col,row);
		}
		else if ((internalCounter == 1) || (internalCounter == 2)){
			position = position + 1;
			col = col + 1;
		}
		else{
			internalCounter = 0;
			
			if(sensorCounter==7){
				sensorCounter = 0;
				drawSucceded = true;
				bufferWritten = true;
				return;
			}
			else{
			sensorCounter = sensorCounter + 1;
			drawSucceded=true;
			return;
			}
		}
		internalCounter++;
		nextChar = writeBuffer[row][col];
	}
	draw(position,nextChar);
}

void Lcd::insertSensorValuesToBuffer(unsigned char* inArray){
	//places sensorvalues in the correct position in the buffer
	int r = 0;
	int c = 0;

	for(unsigned int i = 1; i<8; i++){
		switch(i)
		{
			case 1 :	r = 0;
						c = 3;	
						break;
			case 2 :	r = 1;
						c = 3;
						break;
			case 3 :	r = 0;
						c = 10;
						break;
			case 4 :	r = 1;
						c = 10;
						break;
			case 5 :	r = 2;
						c = 10;
						break;
			case 6 :	r = 3;
						c = 10;
						break;
			case 7 :	r = 2;
						c = 3;
						break;
		}
		writeBuffer[r][c] = inArray[i*3];
		writeBuffer[r][c+1] = inArray[i*3+1];
		writeBuffer[r][c+2] = inArray[i*3+2];
	}
}

int Lcd::getCol(int sensor){
    int col = 0;
    if(sensor <= 1){
        col = 3;
    }
    else{
        col = 10;
    }
    return col;
}

int Lcd::getRow(int sensor){
    int row;
    if((sensor == 0) or (sensor == 2)){
        row = 0;
    }
    else if((sensor == 1) or (sensor == 3)){
        row = 1;
    }
    else if((sensor == 4) or (sensor == 6)){
        row = 2;
    }
    else{
        row = 3;
    }
    return row;
}

bool Lcd::ready(){

/*
 * lcd.cpp
 *
 * Created: 4/14/2014 11:13:42 AM
 *  Author: davha227
 *
 *  Virat kablage
 *  Rs -> PORTA5
 *  R/W -> PORTA6
 *  E -> PORTA7
 *  DB0 -> PORTC0
 *  DB1 -> PORTC1
 *  DB2 -> PORTD4
 *  DB3 -> PORTD5
 *  DB4 -> PORTD6
 *  DB5 -> PORTD7
 *  DB6 -> PORTC6
 *  DB7 -> PORTC7
 */
	PORTA &=~(1<<PORTA5);
	PORTA |=(1<<PORTA6);
	_delay_us(1);
	//Make it possible to read from DB0-DB7
	DDRC &=~((1<<PORTC0)|(1<<PORTC1)|(1<<PORTC6)|(1<<PORTC7));
	DDRD &=~((1<<PORTD4)|(1<<PORTD5)|(1<<PORTD6)|(1<<PORTD7));

	PORTA |=(1<<PORTA7);
	_delay_us(1);
	//Data should now be ready to read from PORTC7.
	
	volatile bool temp = PINC & 0x80;
	
	
	DDRC |=(1<<PORTC0)|(1<<PORTC1)|(1<<PORTC6)|(1<<PORTC7);
	DDRD |=(1<<PORTD4)|(1<<PORTD5)|(1<<PORTD6)|(1<<PORTD7);
	
	if(temp){
		PORTA &=~(1<<PORTA7);
		return false;
	}
	else{
		PORTA &=~(1<<PORTA7);
		return true;
	}
	
}
