#ifndef _Map_h
#define _Map_h

#define F_CPU 14745600

#ifndef __AVR_ATmega1284P__
#define TESTING 1
#else
#define TESTING 0
#endif

#if TESTING == 1
#include <iostream>
#endif

#include "Abstraction.h"
#include <string.h>
#include <stdio.h>

class MapSection;
class Robot;

class Map
{
public:
    // Construct
    Map();
    ~Map();
	
	void setSection(int, int, MapSection*);
	void convertSection(int,int,char);
	char* getColAsChar(int);
	
	MapSection* getPos(int,int);
    
#if TESTING == 1
    void printMap();
#endif
	
protected:
	MapSection* mapArea[32][17];
};

#endif
