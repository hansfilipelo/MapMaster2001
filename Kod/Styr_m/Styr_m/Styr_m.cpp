/*
 File: Styr_m.cpp
 Purpose: Main file for steer module. Interrupts and main loop implemented here.
 */

#define F_CPU 14745600

#ifndef __AVR_ATmega1284P__
#define TESTING 1
#else
#define TESTING 0
#endif

#if TESTING == 0

#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>

#endif

#include <stdio.h>
#include <stdlib.h>
#include "./MapSection.h"
#include "./Map.h"
#include "Abstraction.h"
#include "Communication.h"
#include "slave.h"





// Intiating global variables, which wee need since they are the only variables accessible from interrupts.
// -----------------------------
Slave steerModuleSlave;
Slave* slavePointer = &steerModuleSlave;
Communication* abstractionObject = new Communication(slavePointer);
Map* mapPointer = new Map();
Robot* robotPointer = new Robot(16,1,mapPointer,abstractionObject);

#if TESTING == 0

// Interreupt for bus comm
// -----------------------------

ISR(BADISR_vect){
	robotPointer->setSpeed(0);
	robotPointer->drive();
	
	volatile int p;
	p++;
	asm("");	
}
//Gyro interrupd
ISR(INT0_vect){
	asm("");
	asm("");
	robotPointer->stopRotation();
	robotPointer->robotRotated();
	asm("");
	asm("");
}
//Wheel counter interrupt
ISR(INT1_vect){
	asm("");
	asm("");
	robotPointer->wheelHasTurned = true;
	robotPointer->rotationCount = 0;
	asm("");
	asm("");
}


// Interrupt for copying bus data to buffer
ISR(SPI_STC_vect){
	steerModuleSlave.position++;
	SPDR = steerModuleSlave.outDataArray[steerModuleSlave.position];
	steerModuleSlave.inDataArray[steerModuleSlave.position - 1] = SPDR;
	
	if ((steerModuleSlave.position == (steerModuleSlave.inDataArray[0]+1))&&(steerModuleSlave.inDataArray[0]!= 0)){
		steerModuleSlave.position = 0;
		PORTC |= (1<<PORTC0);
		PORTC &=~(1<<PORTC0);
	}
	if((steerModuleSlave.slaveSend && (steerModuleSlave.position == steerModuleSlave.outDataArray[0]+1))){ //When slave sends, position is not set to zero.
		steerModuleSlave.slaveSend = false;
		steerModuleSlave.position = 0;
	}
}

// Interrupt for handling bus data
ISR(PCINT2_vect){
	abstractionObject->handleData();
}

// ---------------------------------
// Initiates pwm for steer control
void pwm_init()
{
	OCR2A = 0;
	OCR2B = 0;
	
	TCCR2A |= (1<<WGM21)| (1<<WGM20) | (1<<COM2A1) | (0<<COM2A0) | (1<<COM2B1) | (0<<COM2B0);
	TCCR2B |= (0<<WGM22) | (0<<CS22) | (1<<CS21) | (1<<CS20);
	
	OCR2A = 0;
	OCR2B = 0;
	// Set Port 20 and 21 as outputs (for PWM)
	// Set port 18 and 19 as outputs (for choosing direction)
	// Set port 16 and 17 as inputs (for manual controllers)
	DDRD |= 0xF0;
	
	// Initiate gear as 00
	PORTD |= (0<<PORTD4) | (0<<PORTD5);
}

#endif

// ----------------------------------------
// Main

int main(void)
{
#if TESTING == 0
    // Set up interrupts
	cli();
	//MCUCR = 0b00000000;
	EIMSK = 0b0000011;
	EICRA = 0b00001111;
	//SMCR = 0x01;
	
	
    // Initiates PWM
	pwm_init();
	DDRD &= ~((1<<DDD2)|(1<<DDD3));
	
	
    // Initiate with gear forward (PWM default is backwards)
	abstractionObject->setRobot(robotPointer);
    robotPointer->changeDirection('f');
	robotPointer->changeGear('f');
	robotPointer->setSpeed(0);
	robotPointer->drive();
    // Set up bus comm
    steerModuleSlave.SPI_Init();
    
	sei();
	
#endif
	
    // Wait for new data and then mapping
	robotPointer->waitForNewData();
	robotPointer->waitForNewData();
	robotPointer->setFwdReference();
	robotPointer->setBwdReference();
    
    //-----------------------------------------------------
    //right wall following loop
	if((robotPointer->RFIDmode)&&(robotPointer->getBwdDistance() < 30)){
		robotPointer->setBwdClosed();
	}
	if((robotPointer->RFIDmode)&&(robotPointer->getFrontRightDistance() < 20)){
		robotPointer->setRightClosed();
	}
	else if(!robotPointer->RFIDmode){
		robotPointer->setFwdClosed();
	    robotPointer->setBwdClosed();
	    robotPointer->setRightClosed();
		robotPointer->setLeftClosed();
	}
    
    abstractionObject->sendMap();
    
	abstractionObject->reactivateWheelSensor();
	
	
	for(;;){		
		// Manual mode
		if (abstractionObject->getManual()) {
			asm("");
			if (robotPointer->getRotateRightActive())
			{
				robotPointer->rotateRight();
				
			}
			else if ( robotPointer->getRotateLeftActive() ){
				robotPointer->rotateLeft();
			}
		}
		
/*------------------ Right wall follower ----------------------
--------------------OuterWallMode-------------------------
-------------------------------------------------------*/
		
		
		else{
			while(!robotPointer->wheelHasTurned){
				robotPointer->followRight();
				if(abstractionObject->sendMapNow){
					asm("");
					abstractionObject->sendMapNow=false;
					abstractionObject->sendMap();
					asm("");
				}
			}
		}
		robotPointer->updateRobotPosition();
		
		if(robotPointer->islandMode)
		{
			asm("");
			_delay_ms(1000);
			break;
			asm("");
		}
	}
	
	
/*------------------Right wall follower ----------------------
--------------------IslandMode-------------------------
-------------------------------------------------------*/
	
	for (;;) {
		robotPointer->exploreIsland();
		if(robotPointer->isHome())
		{
			asm("");
			break;
			asm("");
		}
	}
            
		
    //----------------FINISHED MODE----------------------------//
	robotPointer->setSpeed(0);
	robotPointer->drive();
	
	mapPointer->fillUnexplored();
	abstractionObject->sendMap();
	
	//this loop exists to stop robot from reseting after completed algorithm
	//it also allows the robot to send the current map
	for(;;){
		 if(abstractionObject->sendMapNow){
			 asm("");
			 abstractionObject->sendMapNow=false;
			 abstractionObject->sendMap();
			 asm("");
		 }
	}
	/*--------------ExploreMode-----------------------------------------------------------------------*/
	//this loop was used for exploring unknown areas not connected to islands, it was also for after exploration
	//return the robot to its starting pos
	
	for(;;){        
        robotPointer->explore();
        if(!robotPointer->stillUnexplored()){
            abstractionObject->sendMap();
            break;
        }
    }
    
	for(;;){
        asm("");
        //return to home
        robotPointer->goHome();
        
        if(abstractionObject->sendMapNow){
            asm("");
            abstractionObject->sendMapNow=false;
            abstractionObject->sendMap();
            asm("");
        }
        asm("");
        //bombo! <-- Not implemented :(
	}
    
    return 0;
}
