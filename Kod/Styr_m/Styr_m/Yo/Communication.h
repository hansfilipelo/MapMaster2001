#ifndef _Communication_h
#define _Communication_h

#include <stdio.h>
#include "slave.h"

#include "MapSection.h"
#include "Abstraction.h"

class MapSection;
class Robot;
class Slave;

class Communication {
public:
    Communication(Slave*);
    
    void handleData();
    void setRobot(Robot*);
    
    void sendMap();
    void sendRotateRequest();
	void reactivateRFID();
    void initSendMap();
	
	
	void activateWheelSensor();
    
//flyttade fr�n protected
	unsigned char inData[27];
    unsigned char outData[27];
	volatile bool sendMapNow = false;
	int row = 0;
    double assembleDouble(char, char, char, char);
	bool getManual();
	bool wheelHasTurned=false;
	
	void time0();
	void time1();
    
    
protected:
    
    void sensorArrayToVariables();
    void sendRow(unsigned int);
	bool manual = true;
    int dataLength;
    Slave* slavePointer;
    Robot* robotPointer;
	volatile bool mapConfirmation = false;
	
};

#endif