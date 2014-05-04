#ifndef _MapSection_h
#define _MapSection_h

#ifndef __AVR_ATmega1284P__
#define DEBUG 1
#else
#define DEBUG 0
#endif

#if DEBUG == 0

#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>

#endif

#if DEBUG == 1
#include <iostream>
#endif

#include <stdio.h>
#include <string.h>
#include "Abstraction.h"
#include "Communication.h"

class Map;
class Communication;

class MapSection{
public:
    MapSection(int x,int y, Map*);
    MapSection(int x,int y, Map*, char createType);
    
    virtual ~MapSection();
    
    virtual void setX(int coord);
    virtual void setY(int coord);
    virtual void setType(char newType);
    
    virtual int getX();
    virtual int getY();
    virtual char getType();
    
    virtual MapSection* getTop();
    virtual MapSection* getBottom();
    virtual MapSection* getLeft();
    virtual MapSection* getRight();
    
    /*
    virtual void setTop(MapSection*);
    virtual void setBottom(MapSection*);
    virtual void setLeft(MapSection*);
    virtual void setRight(MapSection*);
     */
    
    virtual bool isUnexplored();
    virtual int findUnexplored();
	
protected:
	int xCoord;
	int yCoord;
	int step;
    char type='u';
/* Availables types are:
     u = unexplored
     c = closed
	 r = robot
	 e = emptysection
	 f = fire
*/
	Map* mom;
    char next;
};


// ------------------------------------------------
// Subclass Robot

class Robot : protected MapSection
{
public:
    Robot(int xPos, int yPos, Map* inMom, Communication* inComm);
    ~Robot();
    
	// Driving
    void changeGear(char direction);
    void drive();
	void setSpeed(int);
    void driveBackward(int speed);
    void moveTo(int xPos, int yPos);
	void rotateLeft();
	void rotateRight();
    void stopRotation();
	
	// Putting sensordata in array
	void fwdValueIn(char fwd[3]);
	void bwdValueIn(char bwd[3]);
	void leftFrontValueIn(char left[3]);
    void leftBackValueIn(char left[3]);
    void leftLongValueIn(char left[3]);
	void rightFrontValueIn(char right[3]);
    void rightBackValueIn(char right[3]);
	void phiDotValueIn(char phi[3]);
    
	// SLAM (mapping, positioning)
	void setFwdClosed();
	void setBwdClosed();
    void setRightClosed();
    void setLeftClosed();
    
    void changeDirection(char);
    char* getColAsChar(int col);
    
    int meanValueArray(char* inputArray, int iterations);
    void updateRobotPosition();
	void setFwdReference();
	void setBwdReference();
    
    // Automatic control
    void turn(int pd); //Positive or negative value will decide left or right
    int getRightDifference();
    int getLeftDifference();
    void adjustPosition();
    
    void setControlParameters(double, double, int);
    
    volatile double Kd = 0; //Differentiation coeff.
    volatile double Kp = 0; //Proportional coeff.
	
    int getRightDistance();
    
	int fwdSensor;
    int bwdSensor;
    int leftFrontSensor;
    int leftBackSensor;
    int rightFrontSensor;
    int rightBackSensor;
    int leftLongSensor;
	
	int fwdDiff;
	int bwdDiff;
	
	bool rotateActive;
	
	char direction = 'f';
    char gear = 'f';
    
    int fwdReference;
    int bwdReference;
    int leftFrontReference;
    int leftBackReference;
    int rightFrontReference;
    int rightBackReference;
    
    //Values concerning automatic control
    
    int movementSpeed;
    int Ref=10; //Reference value for control
    int previousRightError = 0;
    int previousLeftError = 0;
	long int robotTempPd;
    
    MapSection* previousSection = NULL;
    Communication* commObj = NULL;
	
	int speed = 0;
    
    };

#endif