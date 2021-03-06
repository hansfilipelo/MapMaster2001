/*
 File: MapSection.h
 Purpose: Class working as the different segments that the map is made of. The Subclass robot works as both the segment of the map that contains the robot and as the class that controls the robot, it contains both PD-regulation algorithms as well as steer commands and exploring algorithms.
 */

#ifndef _MapSection_h
#define _MapSection_h

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
#if TESTING == 1
#include <iostream>
#endif

#include <stdio.h>
#include <string.h>
#include "Abstraction.h"
#include "Communication.h"
#include <math.h>

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
    
    virtual void cancer();
    
    virtual bool isUnexplored();
	bool isVisited = false;
    int detected = 0;
	
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
    
    bool hasBeenClosed = false;
};


// ------------------------------------------------
// Subclass Robot

class Robot : public MapSection
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
	void setFwdClosed();
	void setBwdClosed();
    void setRightClosed();
    void setLeftClosed();
    
    bool isWallRight();
    bool isCornerRight();
    bool isWallFwd();
	bool isWallFwdExplore();
    bool isWallLeft();
	bool isWallFwdClose();
    
    void changeDirection(char);
    void getColAsChar(int col);
	char colArray[25];
	
	
	bool isCornerPassed();
    void updateRobotPosition();
	void setFwdReference();
	void setBwdReference();
	void closeMap();
	void handleIsland();
	bool startExplore = false;
    
    int getRightDifference();
    int getUserSpeed();
	
	bool wheelHasTurned = false;
	bool foundIsland = false;
	volatile bool RFIDmode = false;
    
    // Automatic control
    void turn(int pd); //Positive or negative value will decide left or right
    void adjustPosition();
    
	// wait for new data before doing stuff
	void waitForNewData();
	
    void setControlParameters(double, double, int, int, int);
	
	volatile bool newData = false;
	volatile bool okayToClose = true;
    
    // Exploring in the middle
    int getFinishX();
    int getFinishY();
    void findFinishPos();
	int getFrontRightDistance();
    int getBwdDistance();
	
    void explore();
    bool lookForULeft();
    void goAcross();
	void followRight();
	bool stillUnexplored();
    int exploreX=0;
    int exploreY=0;
    bool isHome();

	
	
	
//functions and variables for island detection and exploration
	bool islandToLeft = false;
	bool exploringIsland = false;
	void exploreIsland();
	int timesMovedOnIsland = 0;
	int islandStartX = 0;
	int islandStartY = 0;
	void savePosition();
	bool islandMode = false;
    void goHome();
    int rotationCount = 0;
	
protected:
    int finishX;
    int finishY;
    int getRightDistance();
	int getLeftDistance();
	int getFwdDistance();
	
    
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
    // number of times a square has been attempted to be visited.
    
    
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
	bool haltAfterSection = false;
	
	//Paramaters for wall and corner detection
	int rightCornerFront = 30;
	int rightCornerBack = 30;
	int rightWallFront = 30;
	int rightWallBack = 30;
	
	int speed = 0;
	int userSpeed = 0;
    int trimLeft;
    int trimRight;
    char currentGear = 'f';
	
	bool rotateRightActive;
	bool rotateLeftActive;
	
    };

#endif