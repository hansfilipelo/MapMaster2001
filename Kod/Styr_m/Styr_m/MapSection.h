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
    
	// RFID
	void setRFID();
	
	// Driving
    void changeGear(char direction);
    void drive();
	void setSpeed(int);
	void setUserSpeed(int);
    void driveBackward(int speed);
    void moveTo(int xPos, int yPos);
	void rotateLeft();
	void rotateRight();
    void stopRotation();
	void robotRotated();
	void setRotateRightActive();
	bool getRotateRightActive();
	void setRotateLeftActive();
	bool getRotateLeftActive();
	
	// Putting sensordata in array
	void fwdLongValueIn(char fwd[3]);
	void bwdLongValueIn(char bwd[3]);
	void fwdShortValueIn(char fwdShort[3]);
    void bwdShortValueIn(char bwdShort[3]);
    void leftLongValueIn(char midLeft[3]);
	void rightFrontValueIn(char right[3]);
    void rightBackValueIn(char right[3]);
	void phiDotValueIn(char phi[3]);
    
	// SLAM (mapping, positioning)
	volatile int movedToNewPosition = 0;
	volatile int sensorDifference = 0;
	bool usingLong = false;
	void setFwdClosed();
	void setBwdClosed();
    void setRightClosed();
    void setLeftClosed();
    
    bool isWallRight();
    bool isCornerRight();
    bool isWallFwd();
    bool isWallLeft();
	bool isWallFwdClose();
    
    void changeDirection(char);
    char* getColAsChar(int col);
    
    int meanValueArray(char* inputArray, int iterations);
    void updateRobotPosition();
	void setFwdReference();
	void setBwdReference();
    
    int getRightDifference();
    int getUserSpeed();
	
	void waitForNewData();
	void backToStart();
    
    // Automatic control
    void turn(int pd); //Positive or negative value will decide left or right
    void adjustPosition();
    
    void setControlParameters(double inputKp, double inputKd, int inputRef, int inTrimLeft, int inTrimRight, int inFwdRefLong, int inBwdRefLong, int inFwdRefShort, int inBwdRefShort, int inRightCornerFront, int inRightCornerBack, int inRightWallFront, int inRightWallBack, int inHaltAfterSection);
	
	volatile bool newData = false;
    
protected:
    int getRightDistance();
	int getLeftDistance();
	int getFwdDistance();
	int getBwdDistance();
	
    
	int fwdLongSensor;
    int bwdLongSensor;
    int fwdShortSensor;
    int bwdShortSensor;
    int rightFrontSensor;
    int rightBackSensor;
    int leftMidSensor;
	
	int fwdDiff;
	int bwdDiff;
	
	bool rotateActive;
	
	char direction = 'f';
    char gear = 'f';
    char validSensor = 'N'; //No valid from start.
    
    int fwdReference;
    int bwdReference;
    char determineValidSensor();
    int leftFrontReference;
    int leftBackReference;
    int rightFrontReference;
    int rightBackReference;
    
    
    //Values concerning automatic control
    
    int movementSpeed;
    int Ref; //Reference value for control
    volatile double Kd; //Differentiation coeff.
    volatile double Kp; //Proportional coeff.
    
    int previousRightError = 0;
    int previousLeftError = 0;
	long int robotTempPd;
    
    MapSection* previousSection = NULL;
    Communication* commObj = NULL;
	
	// Parameters for detecting map sections
	int fwdRefLong;
	int bwdRefLong;
	int fwdRefShort;
	int bwdRefShort;
	
	//Paramater that determines if robot should stop after one segment.
	volatile bool haltAfterSection;
	
	//Paramaters for wall and corner detection
	int rightCornerFront;
	int rightCornerBack;
	int rightWallFront;
	int rightWallBack;
	
	int speed;
	int userSpeed;
    int trimLeft;
    int trimRight;
    char currentGear;
	
	bool rotateRightActive;
	bool rotateLeftActive;
	
    };

#endif