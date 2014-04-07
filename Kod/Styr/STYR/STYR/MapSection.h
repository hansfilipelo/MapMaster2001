#ifndef _MapSection_h
#define _MapSection_h

#include <stdio.h>
#include <string.h>
#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>
#include "Abstraction.h"

class Map;

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
    
    virtual void setTop(MapSection*);
    virtual void setBottom(MapSection*);
    virtual void setLeft(MapSection*);
    virtual void setRight(MapSection*);
    
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
	Map* mom = NULL;
    char next;
};


// ------------------------------------------------
// Subclass Robot

class Robot : protected MapSection
{
public:
    Robot(int xPos, int yPos, Map* inMom);
    
	// Driving
    void changeDirection(char direction);
    void drive(int speed);
    void moveTo(int xPos, int yPos);
	void rotateLeft();
	void rotateRight();
	void stopRotation();
	
	// Putting sensordata in array
	void fwdValueIn(int);
	void bwdValueIn(int);
	void leftValueIn(int);
	void rightValueIn(int);
	void phiDotValueIn(int);
    
	// SLAM (mapping, positioning)
	void setFwdClosed();
	void setBwdClosed();
    
    
protected:
    int* fwdSensor = new int[100];
    int* bwdSensor = new int[100];
    int* leftSensor = new int[100];
    int* rightSensor = new int[100];
    int* phiDotSensor = new int[100];
	
	int fwdDiff;
	int bwdDiff;
	
	bool rotateActive;
	
	char direction;
    
    };

#endif