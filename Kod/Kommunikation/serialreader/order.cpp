//
//  commander.cpp
//  
//
//  Created by Jens Edhammer on 2014-04-08.
//
//
#include "order.h"

Order::Order(SerialPort *inPort){
    serport = inPort;
}

Order::~Order(){
}

/*Constructs the array that are to be sent to AVR processor.
 */

//void Order::setPID(int kp, int kd){
//    char data[27] = {4,'P',0,(char)kp,(char)kd,30,30,30,30,30,30,30,30,30,30,30,30,30,30,30,30,30,30,30,30,30,30};
//    serport->sendArray(data);
//}

void Order::rotateLeft(int speed){

    char data[27] = {3,'r',0,(char)speed,30,30,30,30,30,30,30,30,30,30,30,30,30,30,30,30,30,30,30,30,30,30,30};
    serport->sendArray(data);
}


void Order::rotateRight(int speed){

    char data[27] = {3,'r',1,(char)speed,30,30,30,30,30,30,30,30,30,30,30,30,30,30,30,30,30,30,30,30,30,30,30};
    serport->sendArray(data);
}

void Order::turnDone()
{
    char data[27] = {1,'G',30,30,30,30,30,30,30,30,30,30,30,30,30,30,30,30,30,30,30,30,30,30,30,30,30};
    serport->sendArray(data);
    std::cout << "Done" << std::endl;
}


void Order::halt(){
    
    char data[27] = {1,'h',30,30,30,30,30,30,30,30,30,30,30,30,30,30,30,30,30,30,30,30,30,30,30,30,30};
    serport->sendArray(data);
}


void Order::forward(int speed){

    char data[27] = {3,'f',30,(char)speed,30,30,30,30,30,30,30,30,30,30,30,30,30,30,30,30,30,30,30,30,30,30,30};
    serport->sendArray(data);
}


void Order::backward(int speed){

    char data[27] = {3,'b',30,(char)speed,30,30,30,30,30,30,30,30,30,30,30,30,30,30,30,30,30,30,30,30,30,30,30};
    serport->sendArray(data);
}

void Order::test(){
    
    char data[27] = {4,'B',30,30,30,30,30,30,30,30,30,30,30,30,30,30,30,30,30,30,30,30,30,30,30,30,30};
    serport->sendArray(data);
}

void Order::fetchMap(){

    char data[27] = {1,'F',30,30,30,30,30,30,30,30,30,30,30,30,30,30,30,30,30,30,30,30,30,30,30,30,30};
    serport->sendArray(data);
}

void Order::autonom(int speed){

    char data[27] = {3,'a',30,(char)speed,30,30,30,30,30,30,30,30,30,30,30,30,30,30,30,30,30,30,30,30,30,30,30};
    serport->sendArray(data);
}

void Order::setControlParameters(double Kp, double Kd, int Ref,int trimLeft,int trimRight,int fwdRefLong, int bwdRefLong,int fwdRefShort,int bwdRefShort, int rightCornerFront,int rightCornerBack, int rightWallFront, int rightWallBack,int haltAfterSection){


    
    char data[27] = {22,'P',0,(char)getTen(Kp),(char)getOne(Kp),(char)getTenth(Kp),(char)getHundreth(Kp),(char)getTen(Kd),(char)getOne(Kd),(char)getTenth(Kd),(char)getHundreth(Kd),(char)Ref,(char)trimLeft,char(trimRight),(char)fwdRefLong,(char)bwdRefLong,(char)fwdRefShort,(char)bwdRefShort, (char)rightCornerFront,(char)rightCornerBack,(char)rightWallFront,(char)rightWallBack,(char)haltAfterSection,30,30,30,30};
    serport->sendArray(data);
}

int Order::getTen(double number){
    return ((int)floor(number/10) %10);
}

int Order::getOne(double number){
    return ((int)floor(number) %10);
}

int Order::getTenth(double number){
    return ((int)floor(number*10) %10);
}

int Order::getHundreth(double number){
    return ((int)floor(number*100) %10);
}



